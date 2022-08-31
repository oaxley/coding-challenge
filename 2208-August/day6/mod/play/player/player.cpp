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
 */

//----- includes
#include "player.h"

#include <math.h>
#include <string.h>

#include <iostream>
#include <iomanip>

#include "../../display/display.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)


//----- private methods
void Player::createVolTable()
{
    for (int i = 0; i < kMaxVolume; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            volume_table[i][j] = (i*j) >> 8;
        }
    }
}

float Player::bpmToFrequency(int bpm)
{
    return (2.0 * bpm) / 5.0;
}

void Player::setPanning(int ch)
{
    int panning[] = {
        audio::PanEnum::kLeft,
        audio::PanEnum::kRight,
        audio::PanEnum::kRight,
        audio::PanEnum::kLeft,
    };

    channels[ch].panning = panning[ch % 4];
}


//----- replay functions
void Player::playInstrument()
{
    // number of bytes needed in this tick for the audio
    int bytes_needed = samples_per_tick * pParams->channels;

    // check if we have enough space in the buffer
    if (bytes_needed > audio_size) {
        std::cerr << "Error: not enough memory available in the buffer!" << std::endl;
        is_playing = false;
        return;
    }

    // clear audio buffer
    memset((char*)&audio_buffer[audio_ptr], 0, bytes_needed);

    // mix all the channels
    for (int ch = 0; ch < pSong->header->channels; ch++)
    {
        if (!channels[ch].instrument) {
            continue;
        }

        // set the writer ptr at the correct location
        int ptr = audio_ptr + channels[ch].panning;

        // compute the samples for this channel
        int position = channels[ch].position;
        float accumulator = 0;
        float step = channels[ch].step;
        int volume = channels[ch].volume;

        int length = channels[ch].instrument->length;
        char *pData = channels[ch].instrument->pData;
        bool instr_is_looped = (channels[ch].instrument->loop_length > 0);

        int value;
        for (int i = 0; i < samples_per_tick; i++)
        {
            // perform resampling
            accumulator = accumulator + step;
            if (accumulator > 1.0) {

                while (accumulator > 1.0) {
                    accumulator = accumulator - 1.0;
                    position = position + 1;
                }

                // detect looped instruments
                if ((position >= length) && instr_is_looped) {
                    position = position - channels[ch].instrument->loop_length;
                }
            }

            if (position >= length) {
                value = 0;
            } else {
                value = pData[position] + 128;
            }

            value = volume_table[volume][value];

            // write the data to the audio buffer and move to next byte
            audio_buffer[ptr] += value;
            ptr = ptr + pParams->channels;
        }

        // save the parameters for this channel
        channels[ch].position = position;
    }

    // play the audio-buffer
    pDriver->push((char*)&audio_buffer[audio_ptr], bytes_needed, 1);
}

//----- resampling functions
void Player::setFrequency(int ch, int period)
{
    // convert the amiga period to amiga frequency
    float frequency = amigaFrequency(period);
    channels[ch].step = frequency / (1.0f * pParams->rate);
}

float Player::amigaFrequency(int period)
{
    return round(kDefaultPeriodFreq / (period * 2.0f));
}

float Player::amigaPeriod(int note, int finetune)
{
    uint16_t amiga_period = kPeriodTable[note];
    return (8363.0L * kPeriodTable[note]) / kFineTune2Hz[finetune];
}


//--- volume functions
void Player::setVolume(int ch, int value)
{
    channels[ch].volume = value;

    if (channels[ch].volume > 64) {
        channels[ch].volume = 64;
    }

    if (channels[ch].volume < 0) {
        channels[ch].volume = 0;
    }
}

void Player::slideVolume(int ch, int value)
{
    setVolume(ch, channels[ch].volume + value);
}


//----- vibrato / portamento effects
void Player::doVibrato(int ch)
{
    int delta;
    int temp = channels[ch].vibrato_pos & 31;

    switch (channels[ch].waveform & 3)
    {
        case 0:     // sinewave
        case 3:     // random
            delta = kSineTable[temp];
            break;

        case 1:     // ramp down
            temp <<= 3;
            if (channels[ch].vibrato_pos < 0) {
                temp = 255 - temp;
            }
            delta = temp;
            break;

        case 2:     // square
            delta = 255;
            break;
    };

    delta = delta * channels[ch].vibrato_depth;
    delta = delta / 128;

    if (channels[ch].vibrato_pos >= 0) {
        setFrequency(ch, channels[ch].freq + delta);
    } else {
        setFrequency(ch, channels[ch].freq - delta);
    }

    channels[ch].vibrato_pos += channels[ch].vibrato_speed;
    if (channels[ch].vibrato_pos > 31) {
        channels[ch].vibrato_pos -= 64;
    }
}

void Player::doPortamento(int ch)
{
    // slide pitch down
    if (channels[ch].freq < channels[ch].porta_period) {
        channels[ch].freq += channels[ch].porta_speed;
        if (channels[ch].freq > channels[ch].porta_period) {
            channels[ch].freq = channels[ch].porta_period;
        }
    }

    // slide pitch up
    if (channels[ch].freq > channels[ch].porta_period) {
        channels[ch].freq -= channels[ch].porta_speed;
        if (channels[ch].freq < channels[ch].porta_period) {
            channels[ch].freq = channels[ch].porta_period;
        }
    }

    setFrequency(ch, channels[ch].freq);
}

void Player::doTremolo(int ch)
{
    int delta;
    int temp = channels[ch].tremolo_pos & 31;

    switch ((channels[ch].waveform>>4) & 3)
    {
        case 0:     // sinewave
        case 3:     // random
            delta = kSineTable[temp];
            break;

        case 1:     // ramp down
            temp <<= 3;
            if (channels[ch].tremolo_pos < 0) {
                temp = 255 - temp;
            }
            delta = temp;
            break;

        case 2:     // square
            delta = 255;
            break;
    };

    delta = delta * channels[ch].tremolo_depth;
    delta = delta / 64;

    if (channels[ch].tremolo_pos >= 0) {
        setVolume(ch, channels[ch].volume + delta);
    } else {
        setVolume(ch, channels[ch].volume - delta);
    }

    channels[ch].tremolo_pos += channels[ch].tremolo_speed;
    if (channels[ch].tremolo_pos > 31) {
        channels[ch].tremolo_pos -= 64;
    }
}

//----- public methods

bool Player::create(audio::IDriver* driver, const mod::Song* song, int count, int size)
{
    // set variables
    pDriver = driver;
    pSong = song;
    pParams = driver->getParams();

    // initialize fields
    bpm = song->header->bpm;
    speed = song->header->speed;
    pattern = song->header->order[0];

    samples_per_tick = (int)(1.0 * pParams->rate / bpmToFrequency(bpm));

    // create volume table
    createVolTable();

    // create the audio buffer
    audio_block_count = count;
    audio_block_size = size;
    audio_size = count * size;
    audio_buffer = new uint8_t[audio_size];
    if (audio_buffer == nullptr) {
        std::cerr << "Error: unable to allocate memory for audio buffer" << std::endl;
        return false;
    }

    return true;
}

void Player::destroy()
{
    // release memory from audio buffer
    delete [] audio_buffer;
}

void Player::updateNote()
{
    // local vars
    int n_channels = pSong->header->channels;
    int eparam, xparam, yparam;

    // reset pattern jump / break flags
    pattern_break = false;
    pattern_jump = false;

    // current position in the pattern data
    char *pData = pSong->patterns[pattern];
    pData = pData + (row * n_channels * sizeof(mod::Note));

    // display current row information
    std::cerr << " " << std::setw(2) << std::setfill('0') << std::hex;
    std::cerr << std::uppercase << row << std::dec << " | ";

    // retrieve the notes for all the channels
    for (int ch = 0; ch < n_channels; ch++ )
    {
        // activate the channel, and set its panning
        setPanning(ch);

        // display the current note
        mod::display::note(pData);
        mod::Note* pNote = (mod::Note*)pData;

        // retrieve effect parameter
        channels[ch].effect = pNote->effect;
        channels[ch].parameters = pNote->parameters;

        eparam = pNote->parameters;
        xparam = pNote->parameters >> 4;
        yparam = pNote->parameters & 0x0F;

        // instrument is present in the line
        if (pNote->instrument) {
            channels[ch].last_instrument = pNote->instrument - 1;
            channels[ch].instrument = pSong->samples[channels[ch].last_instrument];
            channels[ch].finetune = channels[ch].instrument->finetune;

            if (!((pNote->effect == 0x0E) && (xparam == 0x0D))) {
                channels[ch].volume = channels[ch].instrument->volume;
            }
        }

        // note is present in the line
        if (pNote->note < kNoteOff) {
            channels[ch].last_period = amigaPeriod(pNote->note, channels[ch].finetune);

            // trigger vibrato + tremolo waveforms
            if ((channels[ch].waveform & 0x0F) < 4) {
                channels[ch].vibrato_pos = 0;
            }

            if ((channels[ch].waveform >> 4) < 4) {
                channels[ch].tremolo_pos = 0;
            }

            // change frequency only when portamento is not in effect
            if ((pNote->effect != 0x03) && (pNote->effect != 0x05)) {
                channels[ch].freq = channels[ch].last_period;
                channels[ch].position = 0;
            }
        }

        if ((pNote->effect != 0) || (pNote->parameters != 0))
        {
            // process Tick 0 effects
            switch (pNote->effect)
            {
                case 0x03:  // Portamento to Note
                    if (eparam) {
                        channels[ch].porta_speed = eparam;
                        channels[ch].porta_period = channels[ch].last_period;
                    }
                    break;

                case 0x04:  // Vibrato
                    {
                        if (xparam) {
                            channels[ch].vibrato_speed = xparam;
                        }

                        if (yparam) {
                            channels[ch].vibrato_depth = yparam;
                        }
                    }
                    break;

                case 0x05:  // Portamento + Volume Slide
                    {
                        channels[ch].porta_period = channels[ch].last_period;
                    }
                    break;

                case 0x06:  // Vibrato + Volume Slide
                    // not processed on Tick 0
                    break;

                case 0x07:  // Tremolo
                    {
                        if (xparam) {
                            channels[ch].tremolo_speed = xparam;
                        }

                        if (yparam) {
                            channels[ch].tremolo_depth = yparam;
                        }
                    }
                    break;

                case 0x08:  // Pan
                    // not supported
                    break;

                case 0x09:  // Sample offset
                    {
                        if (eparam == 0) {
                            channels[ch].position = channels[ch].sample_offset;
                        } else {
                            channels[ch].sample_offset = eparam << 8;
                            if (channels[ch].sample_offset > channels[ch].instrument->length) {
                                channels[ch].sample_offset = channels[ch].instrument->length;
                            }
                            channels[ch].position = channels[ch].sample_offset;
                        }
                    }
                    break;

                case 0x0B:  // Jump to Pattern
                    {
                        if (pattern_jump == false) {
                            pattern_jump = true;
                            pattern_jump_value = eparam;
                        }
                    }
                    break;

                case 0x0C:  // volume
                    {
                        setVolume(ch, eparam);
                    }
                    break;

                case 0x0D:  // Pattern Break
                    {
                        if (pattern_break == false) {
                            pattern_break = true;

                            pattern_break_value = (xparam * 10) + yparam;
                            if (pattern_break_value >= kPatternMaxRows) {
                                pattern_break_value = 0;
                            }
                        }
                    }
                    break;

                case 0x0E:  // Effects
                    switch (xparam)
                    {
                        case 0x00:  // Set Filter
                            break;

                        case 0x01:  // Fine Portamento Up
                            {
                                channels[ch].freq -= yparam;
                            }
                            break;

                        case 0x02:  // Fine Portamento Down
                            {
                                channels[ch].freq += yparam;
                            }
                            break;

                        case 0x03:  // glissando
                            break;

                        case 0x04:  // Set Vibrato Waveform
                            {
                                channels[ch].waveform &= 0xF0;
                                channels[ch].waveform |= yparam;
                            }
                            break;

                        case 0x05:  // Set Finetune
                            {
                                channels[ch].finetune = yparam;
                                channels[ch].freq = amigaPeriod(pNote->note, yparam);
                            }
                            break;

                        case 0x06:  // Pattern Loop
                            {
                                if (yparam == 0) {
                                    channels[ch].pattern_loop_row = row;
                                } else {
                                    if (channels[ch].pattern_loop == 0) {
                                        channels[ch].pattern_loop = yparam;
                                    } else {
                                        channels[ch].pattern_loop -= 1;
                                    }

                                    if (channels[ch].pattern_loop) {
                                        row = channels[ch].pattern_loop_row;
                                        // ensure we go back to the previous row
                                        tick = -1;
                                        return;
                                    }
                                }
                            }
                            break;

                        case 0x07:  // Set Tremolo Waveform
                            {
                                channels[ch].waveform &= 0x0F;
                                channels[ch].waveform |= (yparam << 4);
                            }
                            break;

                        case 0x08:  // 16 Position Panning
                            break;

                        case 0x0A:  // Fine Volume Slide Up
                            {
                                slideVolume(ch, yparam);
                            }
                            break;

                        case 0x0B:  // Fine Volume Slide Down
                            {
                                slideVolume(ch, -yparam);
                            }
                            break;

                        case 0x0E:  // Pattern Delay
                            {
                                if (pattern_delay == 0) {
                                    pattern_delay = yparam;
                                }
                            }
                            break;

                        case 0x0F:  // Invert Loop
                            break;
                    }
                    break;

                case 0x0F:  // Set Speed / Tempo
                    {
                        if (eparam < 0x20) {
                            speed = eparam;
                        } else {
                            bpm = eparam;
                            samples_per_tick = (int)(1.0 * pParams->rate / bpmToFrequency(bpm));
                            fnSetTimer();
                        }
                    }
                    break;
            }
        }

        // set sample frequency
        if (channels[ch].freq > 0) {
            setFrequency(ch, channels[ch].freq);
        }

        // move pointer to next note
        pData += sizeof(mod::Note);
    }

    // mix the audio
    std::cerr << std::endl;
    playInstrument();
}

void Player::updateEffect()
{
    for (int ch = 0; ch < pSong->header->channels; ch++)
    {
        int xparam = channels[ch].parameters >> 4;
        int yparam = channels[ch].parameters & 0x0F;
        int eparam = channels[ch].parameters;

        // no frequency, nothing to do
        if (!channels[ch].freq) {
            continue;
        }

        switch (channels[ch].effect)
        {
            case 0x00:  // Arpeggio
                {
                    if (eparam > 0) {
                        switch (tick % 3)
                        {
                            case 0:
                                setFrequency(ch, channels[ch].freq);
                                break;

                            case 1:
                                {
                                    int period = channels[ch].last_period + xparam;
                                    if (period >= kNoteOff) {
                                        period = 56;
                                    }
                                    setFrequency(ch, period);
                                }
                                break;

                            case 2:
                                {
                                    int period = channels[ch].last_period - yparam;
                                    if (period < 0) {
                                        period = 0;
                                    }
                                    setFrequency(ch, period);
                                }
                                break;
                        };
                    }
                }
                break;

            case 0x01:  // Portamento Up
                {
                    channels[ch].freq -= eparam;
                    if (channels[ch].freq < 56) {
                        channels[ch].freq = 56;
                    }
                    setFrequency(ch, channels[ch].freq);
                }
                break;

            case 0x02:  // Portamento Down
                {
                    channels[ch].freq += eparam;
                    setFrequency(ch, channels[ch].freq);
                }
                break;

            case 0x03:  // Portamento to Note
                {
                    doPortamento(ch);
                }
                break;

            case 0x04:  // Vibrato
                {
                    doVibrato(ch);
                }
                break;

            case 0x05:  // Portamento + Volume Slide
                {
                    doPortamento(ch);
                    if ((xparam > 0) && (yparam > 0)) {
                        break;
                    }
                    slideVolume(ch, xparam - yparam);
                }
                break;

            case 0x06:  // Vibrato + Volume Slide
                {
                    doVibrato(ch);
                    if ((xparam > 0) && (yparam > 0)) {
                        break;
                    }
                    slideVolume(ch, xparam - yparam);
                }
                break;

            case 0x07:  // Tremolo
                {
                    doTremolo(ch);
                }
                break;

            case 0x08:  // Pan
                break;

            case 0x0A:  // Volume Slide
                {
                    slideVolume(ch, xparam - yparam);
                }
                break;

            case 0x0E:  // Effects
                switch (xparam)
                {
                    case 0x09:  // Retrigger Note
                        {
                            if (tick % yparam == 0) {
                                channels[ch].position = 0;
                                channels[ch].freq = channels[ch].last_period;
                                channels[ch].volume = channels[ch].instrument->volume;
                            }
                        }
                        break;

                    case 0x0C:  // Cut Note
                        {
                            if (tick == yparam) {
                                setVolume(ch, 0);
                            }
                        }
                        break;

                    case 0x0D:  // Delay Note
                        break;
                }
                break;
        }
    }

    playInstrument();
}

END_NAMESPACE(play)
END_NAMESPACE(mod)