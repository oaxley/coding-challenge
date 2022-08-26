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
 * @brief	Helper functions
 */

//----- includes
#include "play.h"

#include <cmath>


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)


// compute the corresponding amiga period from a base note and a finetune
uint16_t amigaPeriod(uint8_t note, uint8_t finetune)
{
    uint16_t amiga_period = kPeriodTable[note];
    float value = round(amiga_period* pow(2.0, -finetune/12.0/8.0));
    return (uint16_t)value;
}

// compute the amiga frequency from an amiga period value
uint16_t amigaFrequency(uint16_t period)
{
    float value = round(kDefaultPeriodFreq / (period * 2));
    return (uint16_t)value;
}

END_NAMESPACE(play)
END_NAMESPACE(mod)
