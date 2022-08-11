/* -*- coding: utf-8 -*-
 * vim: set ft=c
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Play a single tone with PulseAudio server
 */

//----- includes
#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pulse/simple.h>
#include <pulse/error.h>


//----- typedefs
typedef unsigned char bool;


//----- constants
const bool false = 0;
const bool true = 1;

const int channels = 1;             // number of channels
const int default_duration = 1;     // duration in seconds
const int sampling_freq = 48000;    // sampling frequency
const int buffer_length = 48000;    // buffer length for PulseAudio, should be equal to sampling frequency to avoid "click" sound


//----- globals
pa_simple* hndPulse = NULL;


//----- functions

/* print help */
void help() {
    puts("pTone - Generate a tone with PulseAudio server");
    puts("Syntax:");
    puts("    pTone [--help] --freq/-f <frequency> [--duration-d <duration>]");
    puts("");
    puts("--freq/-f: generate a tone at the selected frequency");
    puts("--duration/-d: duration for the tone (default: 1s)");
    puts("");
}

/* print an error message */
void printError(char* message, int errnum) {
    fprintf(stderr, "Error: %s\n", message);
    if (errnum < 0) {
        fprintf(stderr, "%s\n", pa_strerror(errnum));
    }
}

/* open PulseAudio backend
 *
 * Returns:
 *      true if the backend has been correctly opened, false otherwise
 */
bool openPulse() {
    // sample format to use
    pa_sample_spec ss = {
        .format = PA_SAMPLE_FLOAT32,
        .rate = sampling_freq,
        .channels = channels
    };

    // open the backend
    int error;
    hndPulse = pa_simple_new(NULL, "pTone", PA_STREAM_PLAYBACK, NULL, "pTone", &ss, NULL, NULL, &error);
    if (hndPulse == NULL) {
        printError("cannot open connection to PulseAudio server.", error);
        return false;
    }

    return true;
}

/* close PulseAudio backend */
bool closePulse() {
    int error;

    if (hndPulse == NULL)
        return false;

    // drain the audio interface
    if (pa_simple_drain(hndPulse, &error) < 0) {
        printError("unable to drain the audio interface.", error);
        return false;
    }

    // close pulse connection
    pa_simple_free(hndPulse);
    hndPulse = NULL;

    return true;
}

/* push data to PulseAudio backend
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
    int error;

    const int bufsize = 1024;
    unsigned char u8Buffer[bufsize];

#ifdef DEBUG
    // retrieve the latency
    pa_usec_t latency;
    if ((latency = pa_simple_get_latency(hndPulse, &error)) == (pa_usec_t)-1) {
        printError("failed to retrieve latency!", error);
    } else {
        printf("Current latency = %0.0f usec\n", (float)latency);
    }
#endif

    for (int i = 0; i < count; i++) {
        retval = pa_simple_write(hndPulse, buffer, frames, &error);

        // not all the frames were received
        if (retval < 0) {
            printError("unable to play audio", error);
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


//----- main entry point
int main(int argc, char* argv[]) {

    float frequency = -1.0;
    int duration = default_duration;

    // define options
    struct option long_options[] = {
        {"freq",     required_argument, 0, 'f'},
        {"duration", required_argument, 0, 'd'},
        {0,          0,                 0,  0}
    };

    // process command line arguments
    int long_index = 0;
    int opt;
    while((opt = getopt_long(argc, argv, "f:d:", long_options, &long_index)) != -1) {
        switch(opt) {
            case 'f':
                frequency = atof(optarg);
                break;
            case 'd':
                duration = atoi(optarg);
                break;
            default:
                help();
                exit(0);
        }
    }

    if (frequency < 0) {
        puts("Error: please specify a frequency. Use --help/-h for help.");
        exit(1);
    }

    // intialize PulseAudio connection
    if (!openPulse()) {
        return 1;
    }

    // play the tone
    playFrequency(frequency, duration);

    // drain & close
    closePulse();

    // end
    return 0;
}
