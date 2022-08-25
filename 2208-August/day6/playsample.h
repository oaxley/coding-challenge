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
 * @brief	Play a MOD sample
 */
#ifndef PLAY_SAMPLE_H
#define PLAY_SAMPLE_H

//----- includes
#include "mod/structures.h"
#include "Audio/idriver.h"

void playSample(int sample, const mod::file::Song* pSong, audio::IDriver* pDriver);


#endif // PLAY_SAMPLE_H