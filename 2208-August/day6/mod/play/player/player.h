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
 * @brief	Main include header
 */
#ifndef MOD_PLAY_PLAYER_H
#define MOD_PLAY_PLAYER_H

#include <vector>
#include <cstdint>

#include "../../../slMacros.h"
#include "../../../audio/audio.h"
#include "../../constants.h"
#include "../../structures.h"

#include "channel.h"

BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)

// player structure
struct Player
{
    //----- members
    audio::IDriver* pDriver = nullptr;
    const mod::Song* pSong = nullptr;
    const audio::Parameters* pParams = nullptr;

    bool is_playing = false;
    bool show_rows = true;      // display the pattern rows while playing

    int bpm = 0;
    int speed = 0;
    int tick = 0;
    int row = 0;
    int order = 0;
    int pattern = 0;

    // fractional number of frames per tick (rate * 5 / (2 * bpm))
    int tick_remainder = 0;

    bool pattern_jump = false;
    bool pattern_break = false;
    bool pattern_loop_jump = false;

    int pattern_jump_value = 0;
    int pattern_break_value = 0;
    int pattern_loop_target = 0;

    int pattern_delay = 0;      // number of remaining repetitions of the row
    bool pattern_delay_active = false;  // the current row is a repetition

    int mix_gain = 0;           // master gain (8.8 fixed point)

    Channel channels[kMaxChannels];

    //----- audio buffers
    std::vector<int32_t> mix_buffer;
    std::vector<int16_t> audio_buffer;


    //----- methods
    bool create(audio::IDriver*, const mod::Song*);
    void destroy();

    // process one tick (sequencing + mixing + output); false at the end of the song
    bool processTick();

private:
    void updateNote();
    void updateEffect();
    bool nextRow();

    //----- replay functions
    int framesForTick();
    void mix(int frames);
    void mixDeclick(int32_t* pOut, int frames, Channel& channel);
    void triggerNote(int ch);
    void stopVoice(Channel& channel);

    //----- resampling functions
    void setFrequency(int ch, int period);
    int amigaPeriod(int note, int finetune);

    //----- volume functions
    void setVolume(int ch, int value);
    void slideVolume(int ch, int value);
    void volumeSlide(int ch, int parameters);

    //----- vibrato / portamento effects
    int waveformDelta(int waveform, int pos);
    void doVibrato(int ch);
    void doPortamento(int ch);
    void doTremolo(int ch);
};

END_NAMESPACE(play)
END_NAMESPACE(mod)

#endif // MOD_PLAY_PLAYER_H
