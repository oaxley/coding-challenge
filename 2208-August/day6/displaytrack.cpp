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

//----- includes
#include "displaytrack.h"

#include <iostream>
#include <iomanip>

#include "mod/loader.h"
#include "mod/constants.h"


//----- globals
// mapping table Period <-> Note
const char* kNoteName[] = {
    "C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2", "A-2", "A-2", "B-2",
    "C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3", "A-3", "A-3", "B-3",
    "C-4", "C#4", "D-4", "D#4", "E-4", "F-4", "F#4", "G-4", "G#4", "A-4", "A-4", "B-4",
    "C-5", "C#5", "D-5", "D#5", "E-5", "F-5", "F#5", "G-5", "G#5", "A-5", "A-5", "B-5",
    "C-6", "C#6", "D-6", "D#6", "E-6", "F-6", "F#6", "G-6", "G#6", "A-6", "A-6", "B-6",
    "...", "...",
};


//----- functions
void displayTrack(int track, const mod::file::Song* pSong)
{
    if ((track < 0) || (track > pSong->header->max_pattern)) {
        std::cerr << "Error: track value should be between 0 and " << (int)pSong->header->max_pattern;
        std::cerr << std::endl;
        return;
    }

    // retrieve the track information
    auto pTrack = pSong->patterns[track];

    for (int row = 0; row < mod::file::kPatternMaxRows; ++row)
    {
        std::cout << " " << std::setw(2) << std::setfill('0') << std::hex;
        std::cout << std::uppercase << row << std::dec << " | ";

        for (int ch = 0; ch < pSong->header->channels; ++ch)
        {
            mod::file::Note* pNote = (mod::file::Note*)pTrack;

            const char* note_name = kNoteName[(int)pNote->note];
            int sample = (int)pNote->sample;
            int effect = (int)pNote->effect;
            int params = (int)pNote->parameters;


            std::cout << note_name << ".";

            if (sample == 0) {
                std::cout << "..";
            }
            else {
                std::cout << std::setw(2) << std::setfill('.');
                std::cout << std::hex << std::uppercase << sample << std::dec;
            }
            std::cout << "..";

            if (effect == 0) {
                std::cout << ".";
            } else {
                std::cout << std::hex << effect << std::dec;
            }

            if (params == 0) {
                std::cout << "..";
            } else {
                std::cout << std::setw(2) << std::setfill('0') << std::hex;
                std::cout << params << std::dec;
            }

            std::cout << " | ";

            pTrack += 4;
        }

        std::cout << std::endl;
    }
}