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

#include "../../../slMacros.h"
#include "../../structures.h"

BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)

struct Channel
{
    //----- generic variables
    int volume = 0;
    int panning = 0;

    // note attributes
    int note = 0;               // index in amiga PeriodTable
    int period = 0;             // PeriodTable[note]
    int effect = 0;
    int parameters = 0;

    int finetune = 0;
    int last_instrument = 0;
    int last_period = 0;

    int freq = 0;

    mod::Sample* instrument = nullptr;

    //----- resampling variables
    int position = 0;
    float step = 0;

    //----- effect variables
    int sample_offset = 0;

    int last_porta = 0;
    int last_volume_slide = 0;

    int porta_note = 0;
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
};

END_NAMESPACE(play)
END_NAMESPACE(mod)

#endif // MOD_PLAY_PLAYER_CHANNEL_H