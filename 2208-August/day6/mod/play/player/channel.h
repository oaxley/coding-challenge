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
 * @brief	Individual Channel structure
 */
#ifndef MOD_PLAY_PLAYER_CHANNEL_H
#define MOD_PLAY_PLAYER_CHANNEL_H

#include <cstdint>

#include "../../../slMacros.h"
#include "../../structures.h"

BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)

struct Channel
{
    //----- generic variables
    int volume = 0;             // base volume (0..64) set by the pattern
    int mix_volume = 0;         // volume used for the current tick (tremolo, ...)
    int ramp_volume = 0;        // volume used at the end of the previous tick
    int panning = 0;            // 0 = left, 1 = right (offset in the stereo frame)

    // note attributes
    int note = 0;               // index in amiga PeriodTable
    int period = 0;             // current amiga period (modified by slides)
    int effect = 0;
    int parameters = 0;

    int finetune = 0;
    int last_instrument = -1;   // index of the last instrument used on this channel
    int last_period = 0;        // period of the last note (without slides)

    const mod::Sample* instrument = nullptr;

    //----- resampling variables (16.16 fixed point)
    bool active = false;        // the voice produces sound
    uint32_t position = 0;      // integer position in the sample
    uint32_t frac = 0;          // fractional position (16 bits)
    uint32_t step = 0;          // 16.16 increment per output frame

    //----- anti-click variables
    int last_out = 0;           // last value produced by the voice
    int declick = 0;            // decaying value added after a discontinuity

    //----- effect variables
    int sample_offset = 0;

    int porta_speed = 0;
    int porta_period = 0;

    int vibrato_pos = 0;
    int vibrato_speed = 0;
    int vibrato_depth = 0;

    int tremolo_pos = 0;
    int tremolo_speed = 0;
    int tremolo_depth = 0;

    int waveform = 0;

    int pattern_loop = 0;
    int pattern_loop_row = 0;

    bool has_delayed = false;   // a note is waiting for EDx
    mod::Note delayed = {};     // the delayed note
};

END_NAMESPACE(play)
END_NAMESPACE(mod)

#endif // MOD_PLAY_PLAYER_CHANNEL_H
