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
BEGIN_NAMESPACE(file)

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
    858, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,     // C-1 to B-1
    428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,     // C-2 to B-2
    214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,     // C-3 to B-3
      0,   0,
};

// FineTune vs Hertz convertion table
const uint16_t kFineTuneHertz[] = {
    8363, 8413, 8463, 8529, 8581, 8651, 8723, 8757,
    7895, 7941, 7985, 8046, 8107, 8169, 8232, 8280
};

// default note value
const uint16_t kDefaultNote = kFineTuneHertz[0];


END_NAMESPACE(file)
END_NAMESPACE(mod)

#endif // MOD_CONSTANTS_H