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
 * @brief	MOD Player Structures
 */

//----- includes
#include <cstdint>

#include "slMacros.h"
#include "modConst.h"


BEGIN_NAMESPACE(Mod)

// MOD Tracker header
typedef struct
{
    uint8_t title[SONG_TITLE_LENGTH];       // the song name
    uint8_t channels;                       // the number of channels in the file
    uint8_t bpm;                            // Beat Per Minutes
    uint8_t speed;                          // Speed
    uint8_t length;                         // song length in number of orders
    uint8_t max_pattern;                    // max number of pattern in the song
    uint8_t max_samples;                    // max number of samples in the song
    uint8_t order[MAX_NUMBER_PATTERNS];     // order in which the patterns will be played
} Header;

// MOD Tracker sample
typedef struct {
    uint8_t name[SAMPLE_NAME_LENGTH];       // sample name
    uint16_t length;                        // sample length
    int8_t finetune;                        // finetune value
    uint8_t volume;                         // sample volume
    uint16_t loop_start;                    // loop start
    uint16_t loop_length;                   // loop length

    uint8_t* pdata;                         // a pointer to the data in memory
} Sample;

END_NAMESPACE(Mod)