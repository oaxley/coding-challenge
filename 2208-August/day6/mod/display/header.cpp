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
 * @brief	Function to display header information
 */

//----- includes
#include "display.h"

#include <iostream>
#include <iomanip>

#include "../constants.h"
#include "../structures.h"

BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(display)


void header(const mod::Song* pSong)
{
    std::cout << "====== Music MOD Information ======" << std::endl;
    std::cout << "Song Name   : " << pSong->header->title << std::endl;
    std::cout << "Song Length : " << static_cast<int>(pSong->header->length) << std::endl;
    std::cout << "Speed / BPM : " << static_cast<int>(pSong->header->speed);
    std::cout << " / " << static_cast<int>(pSong->header->bpm) << std::endl;
    std::cout << "Channels    : " << static_cast<int>(pSong->header->channels) << std::endl;
    std::cout << "Max Pattern : " << static_cast<int>(pSong->header->max_pattern) << std::endl;

    std::cout << "Order       : ";

    std::cout << "[ ";
    for (int i = 0; i < pSong->header->length; i++) {
        std::cout << static_cast<int>(pSong->header->order[i]) << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Max Samples : " << static_cast<int>(pSong->header->max_samples) << std::endl;
    int counter = 1;
    for (mod::Sample* pSample : pSong->samples)
    {
        if (pSample->length > 0) {
            int finetune = static_cast<int>(pSample->finetune);
            std::cout << std::setw(2) << counter << " | ";
            std::cout << std::setw(22) << pSample->name << " | ";
            std::cout << std::setw(10) << static_cast<int>(pSample->length) << " | ";
            std::cout << std::setw(7) <<  ((finetune > 7) ? finetune - 16 : finetune) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->volume) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->loop_start) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->loop_length) << " | ";
            std::cout << std::endl;
        }

        counter++;
    }
}


END_NAMESPACE(display)
END_NAMESPACE(mod)
