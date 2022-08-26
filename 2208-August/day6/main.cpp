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

#include <string>
#include <iostream>
#include <filesystem>

#include "audio/audio.h"
#include "mod/mod.h"


//----- globals

// oeration types
typedef enum {
    kPlaySong,
    kPlaySample,
    kDisplayTrack,
    kDisplayInfo,
} OperationEnum;


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
    std::cout << "    --sample/-s : play sample with audio driver" << std::endl;
    std::cout << "    --track/-t  : display track information" << std::endl;
    std::cout << "    --info/-i   : display song information" << std::endl;
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
    audio::DriverEnum driver = audio::kPulseAudio;
    OperationEnum operation = kPlaySong;        // default operation
    std::string filename = "";
    int opvalue;

    // command line options
    struct option long_options[] = {
        {"driver", required_argument, 0, 'd'},
        {"file",   required_argument, 0, 'f'},
        {"sample", required_argument, 0, 's'},
        {"track",  required_argument, 0, 't'},
        {"info",   no_argument,       0, 'i'},
        {"help",   no_argument,       0, 'h'},
        {0       , 0,                 0, 0}
    };

    // process command line arguments
    int long_index = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "f:d:s:t:ih", long_options, &long_index)) != -1) {
        switch (opt)
        {
        case 'd':
            if (std::string(optarg) == std::string("alsa")) {
                driver = audio::kAlsa;
            }
            break;

        case 'f':
            filename = std::string(optarg);
            break;

        case 's':   // play sample
            operation = kPlaySample;
            opvalue = std::stoi(optarg);
            break;

        case 't':   // display track
            operation = kDisplayTrack;
            opvalue = std::stoi(optarg);
            break;

        case 'i':   // display information
            operation = kDisplayInfo;
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
        // initialize audio driver
        audio::IDriver* audio_driver = nullptr;
        switch (driver)
        {
            case audio::kAlsa:
                // std::cout << "selecting Alsa audio driver." << std::endl;
                // audio_driver = new audio::AlsaDriver();
                printError("Error: AlsaDriver not supported.");
                break;
            case audio::kPulseAudio:
                std::cout << "selecting Pulse Audio driver." << std::endl;
                audio_driver = new audio::PulseDriver();
                break;
        }

        // audio parameters
        audio::Parameters params = {
            .name = "modplayer",
            .channels = 2,
            .format = 0,        // unused
            .rate = 22050
        };

        // open the audio driver connection
        audio_driver->open(&params);
        audio_driver->setup();

        // create a new instance of the loader
        mod::file::Loader loader = mod::file::Loader(filename);
        if (!loader.isValidFile()) {
            printError("Error: file is not a valid MOD Tracker song");
        }

        // load the file
        loader.load();

        // execute the proper function
        switch (operation)
        {
            case kDisplayInfo:
                mod::display::header(loader.getSong());
                break;

            case kDisplayTrack:
                mod::display::track(opvalue, loader.getSong());
                break;

            case kPlaySample:
                mod::play::sample(opvalue, loader.getSong(), audio_driver);
                break;

            case kPlaySong:
            default:
                mod::play::song(loader.getSong(), audio_driver);
                break;
        }

        // close the connection
        audio_driver->close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    // end
    return 0;
}