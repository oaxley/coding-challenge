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
 * @brief	Generate a basic note with Alsa Driver
 */

//----- includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generics.h"
#include "alsa_helper.h"


//----- constants
const int default_duration = 1;     // duration in seconds


//----- functions
/* print help */
void help() {
    puts("note - Plays a note");
    puts("Syntax:");
    puts("    note [--help] <note> [duration]");
    puts("");
    puts("note     : selected note from A0 (21) to C8 (108)");
    puts("duration : selected duration (default: 1s)");
    puts("");
}


//----- main entry point
int main(int argc, char* argv[]) {

    int note = 69;
    int duration = default_duration;

    //--- check for values on the command line
    // no arguments
    if (argc == 1) {
        help();
        exit(0);
    }

    // --help / frequency
    if (argc >= 2) {
        if (strncmp(argv[1], "--help", 6) == 0) {
            help();
            exit(0);
        } else {
            note = atoi(argv[1]);
        }
    }

    // duration
    if ((argc == 3) && ((duration = atoi(argv[2])) == 0)) {
        printError("unable to convert duration value.", 0);
        exit(1);
    }
    //--- end


    // print alsa version
    printVersion();

    // open alsa device
    if (!openAlsa(NULL)) {
        return 1;
    }

    // set the interface parameters
    if (!setupAlsa()) {
        return 1;
    }

    // play the note
    playNote(note, duration);

    // drain and close
    closeAlsa();

    return 0;
}