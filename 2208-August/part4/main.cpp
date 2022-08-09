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
#include <math.h>
#include "AlsaDriver.h"
#include "audiodriver/except.h"


//----- constants
const int channels = 1;
const int rate = 48'000;
const int buffer_length = rate;
const int duration = 1;


//----- functions
void playFrequency(Audio::IDriver* pAudio, float frequency, int duration) {

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
int main()
{
    Audio::Parameters params = {
        .name = "default",
        .channels = channels,
        .format = 0,        // not in use
        .rate = rate,
    };

    try
    {
        Audio::IDriver* audio = new AlsaDriver();

        audio->open(&params);
        audio->setup();

        playFrequency(audio, 440, 3);

        audio->close();
    }
    catch (Audio::AudioError e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}