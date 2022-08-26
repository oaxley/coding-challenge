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
 * @brief	Functions to play MOD samples/tracks
 */
#ifndef MOD_PLAY_H
#define MOD_PLAY_H

#include "../../slMacros.h"
#include "../structures.h"
#include "../../audio/audio.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)

// compute the corresponding amiga period from a base note and a finetune
uint16_t amigaPeriod(uint8_t note, uint8_t finetune);

// compute the amiga frequency from an amiga period value
uint16_t amigaFrequency(uint16_t period);

// function to play a sample
void sample(int sample, const mod::Song* pSong, audio::IDriver* pDriver);

// function to play a song
void song(const mod::Song* pSong, audio::IDriver* pDriver);


END_NAMESPACE(play)
END_NAMESPACE(mod)

#endif // MOD_PLAY_H