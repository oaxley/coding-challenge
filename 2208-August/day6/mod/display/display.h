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
 * @brief	Functions to display MOD information
 */
#ifndef MOD_DISPLAY_H
#define MOD_DISPLAY_H

#include "../../slMacros.h"
#include "../structures.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(display)


// function to display track information
void track(int track, const mod::Song* pSong);

// function to display header information
void header(const mod::Song* pSong);


END_NAMESPACE(display)
END_NAMESPACE(mod)

#endif // MOD_DISPLAY_H