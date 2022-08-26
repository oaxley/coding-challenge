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
 * @brief	Main header file for Audio namespace
 */
#ifndef AUDIO_H
#define AUDIO_H

//----- includes
#include "../slMacros.h"

#include "idriver.h"
#include "except.h"
#include "AlsaDriver.h"
#include "PulseDriver.h"


BEGIN_NAMESPACE(audio)

typedef enum {
    kAlsa,
    kPulseAudio,
} DriverEnum;

END_NAMESPACE(audio)

#endif // AUDIO_H