/* -*- coding: utf-8 -*-
 * vim: set ft=cpp
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Alsa helper functions
 */

//----- guard
#ifndef H_ALSA_HELPER
#define H_ALSA_HELPER

//----- includes
#include <math.h>
#include <alsa/asoundlib.h>

#include "generics.h"


//----- constants
const int channels = 1;             // number of channels
const int sampling_freq = 48000;    // sampling frequency
const int buffer_length = 48000;    // buffer length for Alsa, should be equal to sampling frequency to avoid "click" sound


//----- globals
snd_pcm_t* device = NULL;           // global variable >_<


//----- functions

/* print an error message
 *
 * Args:
 *      messsage: the error message
 *      errnum: the error number value from Alsa
 */
void printError(char* message, int errnum) {
    printf("Error: %s\n", message);
    if (errnum < 0) {
        printf("%s\n", strerror(errnum));
    }
}

/* open Alsa backend
 *
 * Args:
 *      name: the alsa device name (optional)
 *
 * Returns:
 *      true if the backend has been correctly opened, false otherwise
 */
bool openAlsa(char* name) {
    int err;

    // set the name by default if none has been provided by the user
    if (name == NULL) {
        name = "default";
    }

    // open the device
    if ((err = snd_pcm_open(&device, name, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printError("cannot open audio device.", err);
        return false;
    }

    return true;
}

/* close Alsa backend */
bool closeAlsa() {
    int err;

    // drain the audio interface
    if ((err = snd_pcm_drain(device)) < 0) {
        printError("unable to drain the audio interface.", err);
        return false;
    }

    // close the audio device
    if ((err = snd_pcm_close(device)) < 0) {
        printError("unable to close the audio device.", err);
        return false;
    }

    return true;
}

/* setup Alsa parameters */
bool setupAlsa() {
    int err;

    err = snd_pcm_set_params(device,
                             SND_PCM_FORMAT_FLOAT,
                             SND_PCM_ACCESS_RW_INTERLEAVED,
                             channels,
                             sampling_freq,
                             1,
                             500000
    );

    if (err < 0) {
        printError("unable to set interface parameters.", err);
        return false;
    }

    // prepare the audio device
    if ((err = snd_pcm_prepare(device)) < 0 ) {
        printError("unable to prepare audio interface.", err);
        return false;
    }

    return true;
}

/* push data to Alsa backend
 *
 * Args:
 *      buffer: the audio buffer (float values)
 *      frames: the number of frames in the buffer
 *      count: the number of time we should play this buffer
 *
 * Returns:
 *      true if data has been sent, false otherwise.
 */
void pushData(float *buffer, int frames, int count) {
    int retval;
    for (int i = 0; i < count; i++) {
        retval = snd_pcm_writei(device, buffer, frames);

        // not all the frames were received
        if (retval != frames) {
            char *string;
            sprintf(string, "only %d frames played, out of %d planned.", retval, frames);
            printError(string, 0);
            break;
        }
    }
}

/* play a single tone frequency
 *
 * Args:
 *      frequency: the frequency to play
 *      duration: the duration of the play
 */
void playFrequency(float frequency, int duration) {

    // print information
    printf("Playing frequency %.02f Hz for %d seconds.\n", frequency, duration);

    // create the tone in the audio buffer
    float max_value, min_value;
    float buffer[buffer_length];
    float w = 2 * M_PI * frequency / sampling_freq;
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
    int nb_samples = sampling_freq * channels * duration;

    // number of times we need to play the buffer to cover the duration
    // and eventual remainder
    int nb_times = nb_samples / buffer_length;
    int nb_rem = nb_samples % buffer_length;

    // play the audio
    pushData(buffer, buffer_length, nb_times);

    if (nb_rem)
        pushData(buffer, nb_rem, 1);

}

#endif  // H_ALSA_HELPER