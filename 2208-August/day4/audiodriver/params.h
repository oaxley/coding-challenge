/* -*- coding: utf-8 -*-
 * vim: set ft=cpp
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Audio Parameters structure
 */

//----- guards
#ifndef AUDIO_PARAMETERS_H
#define AUDIO_PARAMETERS_H

// namespace
namespace Audio
{

typedef struct
{
    const char* name;
    int channels;
    int format;             // not in use
    int rate;
} Parameters;

}; // Audio Namespace

#endif // AUDIO_PARAMETERS_H
