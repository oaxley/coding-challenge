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
 * @brief	MOD Player Constants
 */
#ifndef MOD_CONSTANTS_H
#define MOD_CONSTANTS_H

//----- includes
#include <cstdint>

#include "../slMacros.h"

BEGIN_NAMESPACE(mod)

// max number of samples in a MOD tracker file
const uint8_t kMaxNumberSamples = 31;

// max number of patterns in a MOD tracker file
const uint8_t kMaxNumberPattern = 128;

// max number of characters in the song title
const uint8_t kSongTitleLength = 21;

// max number of characters in the sample name
const uint8_t kSampleNameLength = 23;

// marker position in a Tracker file
const uint16_t kMarkerPosition = 1080;

// number of rows in a pattern
const uint8_t kPatternMaxRows = 64;

// amiga period table
const uint16_t kPeriodTable[] = {
   1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016, 960, 906,
    858,  808,  762,  720,  678,  640,  604,  570,  538,  508, 480, 453,     // C-1 to B-1
    428,  404,  381,  360,  339,  320,  302,  285,  269,  254, 240, 226,     // C-2 to B-2
    214,  202,  190,  180,  170,  160,  151,  143,  135,  127, 120, 113,     // C-3 to B-3
    107,  101,   95,   90,   85,   80,   75,   71,   67,   63,  60,  56,
      0,   0,
};

// default note value (C-2 / 428)
const uint16_t kDefaultNote = 24;

// default frequency for base period computations
const float kDefaultPeriodFreq = 7159090.5;


END_NAMESPACE(mod)

#endif // MOD_CONSTANTS_H