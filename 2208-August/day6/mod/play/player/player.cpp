/* -*- coding: utf-8 -*-
 * vim: filetype=cpp
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	MOD Player implementation
 *
 * The player is driven by the audio output: each call to processTick()
 * computes exactly one tick of audio (rate * 2.5 / bpm frames) and pushes it
 * to the driver. The blocking driver write paces the song, so the music can
 * never drift against the sound card clock nor create gaps in the stream.
 *
 * Mixing is done in signed 32-bit integers and converted to signed 16-bit:
 *  - silence is 0, so starting / stopping voices does not create DC steps;
 *  - volume changes are ramped over a few milliseconds;
 *  - cut voices fade out quickly instead of jumping to 0 ("declick").
 */

//----- includes
#include "player.h"

#include <math.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <iomanip>

#include "../../display/display.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)


//----- constants
namespace {

// period boundaries
const int kMinPeriod = 56;
const int kMaxPeriod = 3424;

// amiga clock used to convert a period to a frequency (Hz = clock / period)
const double kAmigaClock = kDefaultPeriodFreq / 2.0;

// number of frames used to ramp the volume (~3 ms at 22050 Hz)
const int kRampFrames = 64;

// declick tail precision (fixed point) and end threshold
const int kTailShift = 8;
const int kTailThreshold = 1 << kTailShift;

// default panning (Amiga LRRL)
const int kPanning[] = {
    audio::PanEnum::kLeft,
    audio::PanEnum::kRight,
    audio::PanEnum::kRight,
    audio::PanEnum::kLeft,
};

}


//----- replay functions

/* number of frames to generate for the current tick
 *
 * A tick lasts 2.5 / bpm seconds. The fractional part is carried over to the
 * next tick so the tempo is exact on the long run.
 */
int Player::framesForTick()
{
    int numerator = pParams->rate * 5 + tick_remainder;
    int denominator = 2 * bpm;

    tick_remainder = numerator % denominator;
    return numerator / denominator;
}

/* render the decaying tail of a stopped voice */
void Player::mixDeclick(int32_t* pOut, int frames, Channel& channel)
{
    int value = channel.declick;
    if (value == 0) {
        return;
    }

    for (int i = 0; i < frames; i++)
    {
        *pOut += value >> kTailShift;
        pOut += 2;

        // exponential decay (~1/16 per frame)
        value -= value / 16;
        if ((value < kTailThreshold) && (value > -kTailThreshold)) {
            value = 0;
            break;
        }
    }

    channel.declick = value;
}

/* stop a voice, fading out the last value it produced */
void Player::stopVoice(Channel& channel)
{
    if (channel.active) {
        channel.declick += channel.last_out * (1 << kTailShift);
    }

    channel.active = false;
    channel.last_out = 0;
}

/* mix all the channels for this tick and send the audio to the driver */
void Player::mix(int frames)
{
    int n_channels = pSong->header->channels;
    int32_t* pMix = mix_buffer.data();

    std::fill(pMix, pMix + frames * 2, 0);

    for (int ch = 0; ch < n_channels; ch++)
    {
        Channel& c = channels[ch];
        int32_t* pOut = pMix + c.panning;

        // volume ramp (16.16 fixed point) from the previous tick volume
        int target = c.mix_volume;
        int start = (c.ramp_volume < 0) ? target : c.ramp_volume;
        c.ramp_volume = target;

        int rendered = 0;
        if (c.active && c.step)
        {
            const int8_t* pData = c.instrument->pData;
            bool looped = c.instrument->loop_length > 0;
            uint32_t loop_start = c.instrument->loop_start;
            uint32_t loop_length = c.instrument->loop_length;
            uint32_t end = looped ? (loop_start + loop_length) : c.instrument->length;

            uint32_t pos = c.position;
            uint32_t frac = c.frac;
            uint32_t step = c.step;

            int ramp = (start != target) ? std::min(frames, kRampFrames) : 0;
            int32_t vol = start * 65536;
            int32_t vol_inc = ramp ? ((target - start) * 65536) / ramp : 0;

            int out = c.last_out;
            int32_t* p = pOut;
            for (int i = 0; i < frames; i++)
            {
                // linear interpolation between two consecutive samples
                int s0 = pData[pos];
                int s1;
                if (pos + 1 < end) {
                    s1 = pData[pos + 1];
                } else {
                    s1 = looped ? pData[loop_start] : s0;
                }
                int sample = s0 + (((s1 - s0) * (int)frac) >> 16);

                // volume (with ramp on the first frames)
                int volume = target;
                if (i < ramp) {
                    vol += vol_inc;
                    volume = vol >> 16;
                }

                out = sample * volume;
                *p += out;
                p += 2;
                rendered++;

                // move forward in the sample
                frac += step;
                pos += frac >> 16;
                frac &= 0xFFFF;

                if (pos >= end) {
                    if (looped) {
                        pos = loop_start + (pos - end) % loop_length;
                    } else {
                        break;
                    }
                }
            }

            c.position = pos;
            c.frac = frac;
            c.last_out = out;

            // the sample reached its end: the tail starts where the voice stops
            if (pos >= end) {
                mixDeclick(pOut, rendered, c);
                stopVoice(c);
                mixDeclick(pOut + rendered * 2, frames - rendered, c);
                continue;
            }
        }

        mixDeclick(pOut, frames, c);
    }

    // convert to signed 16-bit with the master gain
    int16_t* pAudio = audio_buffer.data();
    int samples = frames * 2;
    if (pParams->channels == 1) {
        for (int i = 0; i < frames; i++) {
            int value = ((pMix[2 * i] + pMix[2 * i + 1]) * mix_gain) >> 9;
            pAudio[i] = (int16_t)std::clamp(value, -32768, 32767);
        }
        samples = frames;
    } else {
        for (int i = 0; i < samples; i++) {
            int value = (pMix[i] * mix_gain) >> 8;
            pAudio[i] = (int16_t)std::clamp(value, -32768, 32767);
        }
    }

    pDriver->push((char*)pAudio, samples * sizeof(int16_t), 1);
}

/* (re)start the instrument of a channel from its beginning */
void Player::triggerNote(int ch)
{
    Channel& c = channels[ch];

    // fade out what the channel was playing
    stopVoice(c);

    if ((c.last_instrument < 0) || (c.last_instrument >= (int)pSong->samples.size())) {
        return;
    }

    c.instrument = pSong->samples[c.last_instrument];
    c.position = 0;
    c.frac = 0;
    c.ramp_volume = -1;     // no volume ramp at the start of a note
    c.active = (c.instrument->length > 0) && (c.instrument->pData != nullptr);
}


//----- resampling functions
void Player::setFrequency(int ch, int period)
{
    if (period <= 0) {
        channels[ch].step = 0;
        return;
    }

    period = std::clamp(period, kMinPeriod, kMaxPeriod);

    // 16.16 fixed point increment per output frame
    double step = (kAmigaClock * 65536.0) / ((double)period * pParams->rate);
    channels[ch].step = (uint32_t)step;
}

int Player::amigaPeriod(int note, int finetune)
{
    note = std::clamp(note, 0, (int)kNoteOff - 1);
    return (int)lround((8363.0 * kPeriodTable[note]) / kFineTune2Hz[finetune & 0x0F]);
}


//--- volume functions
void Player::setVolume(int ch, int value)
{
    channels[ch].volume = std::clamp(value, 0, 64);
    channels[ch].mix_volume = channels[ch].volume;
}

void Player::slideVolume(int ch, int value)
{
    setVolume(ch, channels[ch].volume + value);
}

/* Axy / 5xy / 6xy: x slides up and has priority over y */
void Player::volumeSlide(int ch, int parameters)
{
    int xparam = parameters >> 4;
    int yparam = parameters & 0x0F;

    if (xparam) {
        slideVolume(ch, xparam);
    } else {
        slideVolume(ch, -yparam);
    }
}


//----- vibrato / portamento effects

/* signed value of the vibrato / tremolo waveforms (-255..255) */
int Player::waveformDelta(int waveform, int pos)
{
    int temp = pos & 31;
    int delta = 0;

    switch (waveform & 3)
    {
        case 0:     // sinewave
        case 3:     // random (not supported: sinewave)
            delta = kSineTable[temp];
            break;

        case 1:     // ramp down
            delta = temp << 3;
            if (pos >= 32) {
                delta = 255 - delta;
            }
            break;

        case 2:     // square
            delta = 255;
            break;
    }

    return (pos < 32) ? delta : -delta;
}

void Player::doVibrato(int ch)
{
    Channel& c = channels[ch];

    int delta = (waveformDelta(c.waveform, c.vibrato_pos) * c.vibrato_depth) / 128;
    setFrequency(ch, c.period + delta);

    c.vibrato_pos = (c.vibrato_pos + c.vibrato_speed) & 63;
}

void Player::doPortamento(int ch)
{
    Channel& c = channels[ch];

    if (c.porta_period == 0) {
        return;
    }

    if (c.period < c.porta_period) {
        c.period = std::min(c.period + c.porta_speed, c.porta_period);
    } else if (c.period > c.porta_period) {
        c.period = std::max(c.period - c.porta_speed, c.porta_period);
    }

    setFrequency(ch, c.period);
}

void Player::doTremolo(int ch)
{
    Channel& c = channels[ch];

    // tremolo only changes the mixed volume, not the channel volume
    int delta = (waveformDelta(c.waveform >> 4, c.tremolo_pos) * c.tremolo_depth) / 64;
    c.mix_volume = std::clamp(c.volume + delta, 0, 64);

    c.tremolo_pos = (c.tremolo_pos + c.tremolo_speed) & 63;
}


//----- public methods

bool Player::create(audio::IDriver* driver, const mod::Song* song)
{
    // set variables
    pDriver = driver;
    pSong = song;
    pParams = driver->getParams();

    // initialize fields
    bpm = song->header->bpm;
    speed = song->header->speed;
    tick = 0;
    row = 0;
    order = 0;
    pattern = song->header->order[0];
    tick_remainder = 0;

    // channels initialization
    int n_channels = song->header->channels;
    for (int ch = 0; ch < n_channels; ch++) {
        channels[ch] = Channel();
        channels[ch].panning = kPanning[ch % 4];
    }

    // master gain: a full scale voice is 127 * 64; never clip when all the
    // voices of one side are at full scale
    int per_side = std::max(1, n_channels / 2);
    mix_gain = (256 * 32767) / (per_side * 128 * 64);

    // the longest tick happens with the lowest bpm (32)
    int max_frames = (pParams->rate * 5) / (2 * 32) + 1;
    mix_buffer.assign(max_frames * 2, 0);
    audio_buffer.assign(max_frames * 2, 0);

    is_playing = true;
    return true;
}

void Player::destroy()
{
    mix_buffer.clear();
    audio_buffer.clear();
}

/* compute and play one tick
 *
 * Returns:
 *      false when the song is finished
 */
bool Player::processTick()
{
    if (!is_playing) {
        return false;
    }

    // pattern_delay_active: the row is repeated, notes are not triggered again
    if ((tick == 0) && !pattern_delay_active) {
        updateNote();
    } else {
        updateEffect();
    }

    mix(framesForTick());

    tick = tick + 1;
    if (tick >= speed) {
        tick = 0;
        if (!nextRow()) {
            is_playing = false;
        }
    }

    return is_playing;
}

/* move the song to the next row
 *
 * Returns:
 *      false when the end of the song is reached
 */
bool Player::nextRow()
{
    const mod::Header* hdr = pSong->header.get();

    // pattern delay: repeat the current row
    if (pattern_delay > 0) {
        pattern_delay -= 1;
        pattern_delay_active = true;
        return true;
    }
    pattern_delay_active = false;

    // pattern loop (E6x) has priority and stays in the same pattern
    if (pattern_loop_jump) {
        pattern_loop_jump = false;
        pattern_jump = false;
        pattern_break = false;
        row = pattern_loop_target;
        return true;
    }

    int next_order = order;
    if (pattern_jump || pattern_break) {
        next_order = pattern_jump ? pattern_jump_value : order + 1;
        row = pattern_break ? pattern_break_value : 0;

        // jumping outside of the song restarts it
        if (pattern_jump && (next_order >= hdr->length)) {
            next_order = 0;
        }

        pattern_jump = false;
        pattern_break = false;
    } else {
        row = row + 1;
        if (row < kPatternMaxRows) {
            return true;
        }
        row = 0;
        next_order = order + 1;
    }

    // end of the song
    if (next_order >= hdr->length) {
        return false;
    }

    // a new pattern resets the pattern loop
    if (next_order != order) {
        for (int ch = 0; ch < hdr->channels; ch++) {
            channels[ch].pattern_loop_row = 0;
            channels[ch].pattern_loop = 0;
        }
    }

    order = next_order;
    pattern = hdr->order[order];
    return true;
}

void Player::updateNote()
{
    // local vars
    int n_channels = pSong->header->channels;

    // current position in the pattern data
    char *pData = pSong->patterns[pattern];
    pData = pData + (row * n_channels * sizeof(mod::Note));

    // display current row information
    if (show_rows) {
        std::cout << " " << std::setw(2) << std::setfill('0') << std::hex;
        std::cout << std::uppercase << row << std::dec << " | ";
    }

    // retrieve the notes for all the channels
    for (int ch = 0; ch < n_channels; ch++, pData += sizeof(mod::Note))
    {
        Channel& c = channels[ch];

        // display the current note
        if (show_rows) {
            mod::display::note(pData);
        }
        const mod::Note* pNote = (const mod::Note*)pData;

        // retrieve effect parameter
        c.effect = pNote->effect;
        c.parameters = pNote->parameters;

        int eparam = pNote->parameters;
        int xparam = pNote->parameters >> 4;
        int yparam = pNote->parameters & 0x0F;

        // note delay: the note is processed later in updateEffect()
        if ((c.effect == 0x0E) && (xparam == 0x0D) && (yparam != 0)) {
            c.has_delayed = true;
            c.delayed = *pNote;
            continue;
        }
        c.has_delayed = false;

        bool is_porta = (c.effect == 0x03) || (c.effect == 0x05);

        // instrument is present in the line (ignore invalid instruments)
        if (pNote->instrument && (pNote->instrument <= pSong->samples.size())) {
            c.last_instrument = pNote->instrument - 1;
            const mod::Sample* pSample = pSong->samples[c.last_instrument];
            c.finetune = pSample->finetune;
            setVolume(ch, pSample->volume);
        }

        // set finetune is applied before the note period is computed
        if ((c.effect == 0x0E) && (xparam == 0x05)) {
            c.finetune = yparam;
        }

        // note is present in the line
        if (pNote->note < kNoteOff) {
            int period = amigaPeriod(pNote->note, c.finetune);

            if (is_porta) {
                // portamento: the note becomes the target, it is not triggered
                c.porta_period = period;
            } else {
                c.note = pNote->note;
                c.period = period;
                c.last_period = period;

                // reset vibrato + tremolo waveforms
                if ((c.waveform & 0x04) == 0) {
                    c.vibrato_pos = 0;
                }
                if ((c.waveform & 0x40) == 0) {
                    c.tremolo_pos = 0;
                }

                triggerNote(ch);

                // sample offset only applies to a triggered note
                if (c.effect == 0x09) {
                    if (eparam) {
                        c.sample_offset = eparam << 8;
                    }

                    if (c.active) {
                        const mod::Sample* pSample = c.instrument;
                        uint32_t offset = c.sample_offset;
                        if (offset < pSample->length) {
                            c.position = offset;
                        } else if (pSample->loop_length > 0) {
                            c.position = pSample->loop_start;
                        } else {
                            c.active = false;
                        }
                    }
                }
            }
        }

        // process Tick 0 effects
        switch (c.effect)
        {
            case 0x03:  // Portamento to Note
                if (eparam) {
                    c.porta_speed = eparam;
                }
                break;

            case 0x04:  // Vibrato
                if (xparam) {
                    c.vibrato_speed = xparam;
                }
                if (yparam) {
                    c.vibrato_depth = yparam;
                }
                break;

            case 0x07:  // Tremolo
                if (xparam) {
                    c.tremolo_speed = xparam;
                }
                if (yparam) {
                    c.tremolo_depth = yparam;
                }
                break;

            case 0x0B:  // Jump to Pattern
                pattern_jump = true;
                pattern_jump_value = eparam;
                break;

            case 0x0C:  // volume
                setVolume(ch, eparam);
                break;

            case 0x0D:  // Pattern Break
                pattern_break = true;
                pattern_break_value = (xparam * 10) + yparam;
                if (pattern_break_value >= kPatternMaxRows) {
                    pattern_break_value = 0;
                }
                break;

            case 0x0E:  // Effects
                switch (xparam)
                {
                    case 0x01:  // Fine Portamento Up
                        c.period = std::max(c.period - yparam, kMinPeriod);
                        break;

                    case 0x02:  // Fine Portamento Down
                        c.period = std::min(c.period + yparam, kMaxPeriod);
                        break;

                    case 0x04:  // Set Vibrato Waveform
                        c.waveform = (c.waveform & 0xF0) | yparam;
                        break;

                    case 0x06:  // Pattern Loop
                        if (yparam == 0) {
                            c.pattern_loop_row = row;
                        } else {
                            if (c.pattern_loop == 0) {
                                c.pattern_loop = yparam;
                            } else {
                                c.pattern_loop -= 1;
                            }

                            if (c.pattern_loop) {
                                pattern_loop_jump = true;
                                pattern_loop_target = c.pattern_loop_row;
                            }
                        }
                        break;

                    case 0x07:  // Set Tremolo Waveform
                        c.waveform = (c.waveform & 0x0F) | (yparam << 4);
                        break;

                    case 0x0A:  // Fine Volume Slide Up
                        slideVolume(ch, yparam);
                        break;

                    case 0x0B:  // Fine Volume Slide Down
                        slideVolume(ch, -yparam);
                        break;

                    case 0x0C:  // Cut Note (at tick 0)
                        if (yparam == 0) {
                            setVolume(ch, 0);
                        }
                        break;

                    case 0x0E:  // Pattern Delay
                        if (!pattern_delay_active && (pattern_delay == 0)) {
                            pattern_delay = yparam;
                        }
                        break;
                }
                break;

            case 0x0F:  // Set Speed / Tempo
                if (eparam == 0) {
                    // F00 (stop) is ignored
                } else if (eparam < 0x20) {
                    speed = eparam;
                } else {
                    bpm = eparam;
                }
                break;
        }

        // tick 0 always plays the base volume / frequency
        c.mix_volume = c.volume;
        setFrequency(ch, c.period);
    }

    if (show_rows) {
        std::cout << '\n';
    }
}

void Player::updateEffect()
{
    for (int ch = 0; ch < pSong->header->channels; ch++)
    {
        Channel& c = channels[ch];

        int xparam = c.parameters >> 4;
        int yparam = c.parameters & 0x0F;
        int eparam = c.parameters;

        switch (c.effect)
        {
            case 0x00:  // Arpeggio
                if (eparam > 0) {
                    switch (tick % 3)
                    {
                        case 0:
                            setFrequency(ch, c.period);
                            break;
                        case 1:
                            setFrequency(ch, amigaPeriod(c.note + xparam, c.finetune));
                            break;
                        case 2:
                            setFrequency(ch, amigaPeriod(c.note + yparam, c.finetune));
                            break;
                    }
                }
                break;

            case 0x01:  // Portamento Up
                c.period = std::max(c.period - eparam, kMinPeriod);
                setFrequency(ch, c.period);
                break;

            case 0x02:  // Portamento Down
                c.period = std::min(c.period + eparam, kMaxPeriod);
                setFrequency(ch, c.period);
                break;

            case 0x03:  // Portamento to Note
                doPortamento(ch);
                break;

            case 0x04:  // Vibrato
                doVibrato(ch);
                break;

            case 0x05:  // Portamento + Volume Slide
                doPortamento(ch);
                volumeSlide(ch, eparam);
                break;

            case 0x06:  // Vibrato + Volume Slide
                doVibrato(ch);
                volumeSlide(ch, eparam);
                break;

            case 0x07:  // Tremolo
                doTremolo(ch);
                break;

            case 0x0A:  // Volume Slide
                volumeSlide(ch, eparam);
                break;

            case 0x0E:  // Effects
                switch (xparam)
                {
                    case 0x09:  // Retrigger Note
                        if ((yparam != 0) && (tick % yparam == 0)) {
                            triggerNote(ch);
                        }
                        break;

                    case 0x0C:  // Cut Note
                        if (tick == yparam) {
                            setVolume(ch, 0);
                        }
                        break;

                    case 0x0D:  // Delay Note
                        if (c.has_delayed && (tick == yparam)) {
                            c.has_delayed = false;
                            const mod::Note& n = c.delayed;

                            if (n.instrument && (n.instrument <= pSong->samples.size())) {
                                c.last_instrument = n.instrument - 1;
                                c.finetune = pSong->samples[c.last_instrument]->finetune;
                                setVolume(ch, pSong->samples[c.last_instrument]->volume);
                            }

                            if (n.note < kNoteOff) {
                                c.note = n.note;
                                c.period = amigaPeriod(n.note, c.finetune);
                                c.last_period = c.period;
                                triggerNote(ch);
                                setFrequency(ch, c.period);
                            }
                        }
                        break;
                }
                break;
        }
    }
}

END_NAMESPACE(play)
END_NAMESPACE(mod)
