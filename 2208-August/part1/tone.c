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


//----- typedefs
typedef unsigned char bool;


//----- constants
const bool false = 0;
const bool true = 1;

const int default_duration = 1;


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
 */
void printError(char* message, int errnum) {
    printf("Error: %s\n", message);
}


//----- main entry point
int main(int argc, char* argv[]) {

    int frequency = 0;
    int duration = default_duration;

    // check for value on the command line

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

    return 0;
}