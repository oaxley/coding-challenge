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
 * @brief	Main Entry Point
 */

//----- includes
#include <getopt.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

#include "Audio/Audio.h"


//----- functions
/* print help */
void help()
{
    std::cout << "modplayer - Play a MOD tracker file using Alsa or PulseAudio" << std::endl;
    std::cout << "Syntax:" << std::endl;
    std::cout << "    modplayer [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "    --file/-f   : path to the mod tracker file" << std::endl;
    std::cout << "    --driver/-d : audio driver (default: pulse)" << std::endl;
    std::cout << "    --help/-h   : this help" << std::endl;
    std::cout << std::endl;
}

/* print a standardized error message
 *
 * Args:
 *      message: the error message to display
 */
void printError(const char* message)
{
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
}


//----- main entry point
int main(int argc, char* argv[])
{
    Audio::DriverEnum driver = Audio::PULSE_DRIVER;
    std::string filename = "";

    // command line options
    struct option long_options[] = {
        {"driver", required_argument, 0, 'd'},
        {"file",   required_argument, 0, 'f'},
        {"help",   no_argument,       0, 'h'},
        {0       , 0,                 0, 0}
    };

    // process command line arguments
    int long_index = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "f:d:h", long_options, &long_index)) != -1) {
        switch (opt)
        {
        case 'd':
            if (std::string(optarg) == std::string("alsa")) {
                driver = Audio::ALSA_DRIVER;
            }
            break;

        case 'f':
            filename = std::string(optarg);
            break;

        default:
            help();
            exit(EXIT_SUCCESS);
        }
    }

    // check if filename is not empty
    if (filename.length() == 0) {
        printError("Error: please specify a filename!");
    }

    // check if the filename exists
    if (!std::filesystem::exists(filename)) {
        printError("Error: file does not exist.");
    }

    try
    {
        // read the MOD Tracker file here
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    // end
    return 0;
}