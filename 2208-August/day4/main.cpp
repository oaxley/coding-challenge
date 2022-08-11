/* -*- coding: utf-8 -*-
 * vim: ft=cpp
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Main entry point
 */

//----- includes
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <getopt.h>

#include "audiodriver/except.h"
#include "AlsaDriver.h"
#include "PulseDriver.h"


//----- constants
const int channels = 1;
const int rate = 48'000;
const int buffer_length = rate;
const int default_duration = 1;

typedef enum {
    ALSA,
    PULSEAUDIO
} driver_t;


//----- functions
/* print help */
void help()
{
    std::cout << "sound - Generate a tone with either Alsa or PulseAudio" << std::endl;
    std::cout << "Syntax:" << std::endl;
    std::cout << "    sound [--help] --freq/-f <frequency> ";
    std::cout << "[--time/-t <duration>] ";
    std::cout << "[--driver/-d <driver>]";
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "--freq/-f   : generate a sound at the selected frequency" << std::endl;
    std::cout << "--time/-t   : duration for playing the sound (default: 1s)" << std::endl;
    std::cout << "--driver/-d : select alsa (Alsa) / pulse (PulseAudio) driver for output (default: alsa)" << std::endl;
    std::cout << std::endl;
}


void playFrequency(Audio::IDriver* pAudio, float frequency, int duration)
{
    // print information
    std::cout << "Playing frequency " << frequency << " Hz ";
    std::cout << "for " << duration << " seconds." << std::endl;

    // create the tone in the audio buffer
    float max_value, min_value;
    float buffer[buffer_length];
    float w = 2 * M_PI * frequency / rate;
    float amplitude = 1;

    for (int n = 0; n < buffer_length; n++) {
        // compute a new sample and record it
        float value = amplitude * sin(w * n);
        buffer[n] = value;

        // keep track of min/max for later normalization
        if (n == 0) {
            max_value = value;
            min_value = value;
        } else {
            if (value > max_value)
                max_value = value;

            if (value < min_value)
                min_value = value;
        }
    }

    // retrieve the maximum value for a sample
    // either positive or negative
    if (fabs(min_value) > max_value)
        max_value = fabs(min_value);

    // do the normalization
    float volume = 0.25;
    for (int n = 0; n < buffer_length; n++) {
        buffer[n] = volume * (buffer[n] / max_value);
    }

    // number of samples needed to cover the whole duration
    int nb_samples = rate * channels * duration;

    // number of times we need to play the buffer to cover the duration
    // and eventual remainder
    int nb_times = nb_samples / buffer_length;
    int nb_rem = nb_samples % buffer_length;

    // play the audio
    pAudio->push(buffer, buffer_length, nb_times);

    if (nb_rem)
        pAudio->push(buffer, nb_rem, 1);

}

//----- main entry point
int main(int argc, char* argv[])
{
    float frequency = -1.0;
    int duration = default_duration;
    driver_t driver = ALSA;

    // command line options
    struct option long_options[] = {
        {"freq",   required_argument, 0, 'f'},
        {"time",   required_argument, 0, 't'},
        {"help",   no_argument,       0, 'h'},
        {"driver", required_argument, 0, 'd'},
        {0,        0,                 0,  0}
    };

    // process command line arguments
    int long_index = 0;
    int opt;
    while((opt = getopt_long(argc, argv, "f:t:hd:", long_options, &long_index)) != -1) {
        switch(opt) {
            case 'f':
                frequency = atof(optarg);
                break;
            case 't':
                duration = atoi(optarg);
                break;
            case 'd':
                if (std::string(optarg) == std::string("pulse")) {
                    driver = PULSEAUDIO;
                }
                break;
            case 'h':
            default:
                help();
                exit(0);
        }
    }

    // no frequency selected by the user -> error
    if (frequency < 0) {
        std::cerr << "Error: please specify a frequency." << std::endl;
        std::cerr << "User --help/-h for help." << std::endl;
        exit(EXIT_FAILURE);
    }

    // audio parameters
    Audio::Parameters params = {
        .name = "default",
        .channels = channels,
        .format = 0,        // not in use
        .rate = rate,
    };

    try
    {
        Audio::IDriver* audio = nullptr;

        // select the corresponding driver
        if (driver == ALSA) {
            std::cout << "Selecting Alsa driver." << std::endl;
            audio = new AlsaDriver();
        } else {
            if (driver == PULSEAUDIO) {
                std::cout << "Selecting PulseAudio driver." << std::endl;
                audio = new PulseDriver();
            }
        }

        // open and setup the audio
        audio->open(&params);
        audio->setup();

        // play the frequency selected by the user
        playFrequency(audio, frequency, duration);

        // close the audio
        audio->close();
    }
    catch (Audio::AudioError e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}