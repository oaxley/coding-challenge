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
 * @brief	Function to display information on a particular track
 */
#ifndef DISPLAY_TRACK_H
#define DISPLAY_TRACK_H

//----- includes
#include "mod/structures.h"

//----- functions
void displayTrack(int track, const mod::file::Song* pSong);

#endif // DISPLAY_TRACK_H