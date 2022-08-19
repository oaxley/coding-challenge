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

//----- includes
#include <cstdint>

#include "slMacros.h"


BEGIN_NAMESPACE(Mod)

// max number of samples in a MOD tracker file
const uint8_t MAX_NUMBER_SAMPLES = 31;

// max number of patterns in a MOD tracker file
const uint8_t MAX_NUMBER_PATTERNS = 128;

// max number of characters in the song title
const uint8_t SONG_TITLE_LENGTH = 22;

// max number of characters in the sample name
const uint8_t SAMPLE_NAME_LENGTH = 24;

END_NAMESPACE(Mod)