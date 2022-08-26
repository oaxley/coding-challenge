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


// function to play a sample
void sample(int sample, const mod::Song* pSong, audio::IDriver* pDriver);

END_NAMESPACE(play)
END_NAMESPACE(mod)

#endif // MOD_PLAY_H