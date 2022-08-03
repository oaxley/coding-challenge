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
 * @brief	Generate a basic tone with Alsa driver
 */

//----- includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>


//----- typedefs
typedef unsigned char bool;


//----- constants
const bool false = 0;
const bool true = 1;

const int channels = 1;             // number of channels
const int default_duration = 1;     // duration in seconds
const int sampling_freq = 44100;    // sampling frequency
const int buffer_length = 48000;    // buffer length for Alsa


//----- globals
snd_pcm_t* device = NULL;           // global variable >_<


//----- functions

/* print help */
void help() {
    puts("tone - Generate a tone at the frequency");
    puts("Syntax:");
    puts("    tone [--help] <frequency> [duration]");
    puts("");
    puts("frequency: selected frequency");
    puts("duration : selected duration (default: 1s)");
    puts("");
}

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

//----- main entry point
int main(int argc, char* argv[]) {

    int frequency = 0;
    int duration = default_duration;

    //--- check for values on the command line
    // no arguments
    if (argc == 1) {
        help();
        exit(0);
    }

    // --help
    if ((argc == 2) && (strncmp(argv[1], "--help", 6) == 0)) {
        help();
        exit(0);
    }

    // frequency only
    if ((argc == 2) && ((frequency = atoi(argv[1])) == 0)) {
        printError("unable to convert frequency value.", 0);
        exit(1);
    }

    // duration
    if ((argc == 3) && ((duration = atoi(argv[2])) == 0)) {
        printError("unable to convert duration value.", 0);
        exit(1);
    }
    //--- end


    // print alsa version
    printf("ALSA library version: %s\n", SND_LIB_VERSION_STR);

    // open alsa device
    if (!openAlsa(NULL)) {
        return 1;
    }

    // set the interface parameters
    if (!setupAlsa()) {
        return 1;
    }

    // drain and close
    closeAlsa();

    return 0;
}