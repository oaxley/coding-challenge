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
 * @brief	MOD Song Player logic
 */

//----- includes
#include "player/player.h"

#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include <iostream>

#include "../../slMacros.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)


//----- globals
Player* player;


//----- functions

// SIGINT handler
void sigintHandler(int signum)
{
    player->is_playing = false;
}

// SIGALRM handler
void sigalrmHandler(int signum)
{

    if (player->tick == 0) {
        if (player->pattern_delay == 0) {
            // if (player->row == 0) {
            //     std::cerr << "Pattern " << player->pattern << std::endl;
            // }
            player->updateNote();
        }
    } else {
        player->updateEffect();
    }

    player->tick = player->tick + 1;
    if (player->tick >= player->speed) {

        player->tick = 0;

        if (player->pattern_delay > 0) {
            player->pattern_delay -= 1;
            if (player->pattern_delay > 0) {
                return;
            }
        }

        // pattern jump
        if (player->pattern_jump) {
            player->row = 0;
            player->order = player->pattern_jump_value;
            if (player->order > player->pSong->header->length) {
                player->order = 0;
            }
            player->pattern = player->pSong->header->order[player->order];
        }

        // pattern break
        if (player->pattern_break) {
            if (player->pattern_jump == false) {
                player->order += 1;
                player->pattern = player->pSong->header->order[player->order];
            }

            player->row = player->pattern_break_value;
        }

        // reset the jump/break flags
        if (player->pattern_jump || player->pattern_break) {
            player->pattern_jump = false;
            player->pattern_break = false;

            // leave early (so we don't process the remaining part)
            return;
        }

        // next row
        player->row = player->row + 1;
        if (player->row >= kPatternMaxRows) {

            player->row = 0;

            // next pattern in the order
            player->order = player->order + 1;

            // end of the song
            if (player->order >= player->pSong->header->length) {
                player->is_playing = false;
                return;
            }

            // retrieve the next pattern
            player->pattern = player->pSong->header->order[player->order];
        }
    }
}

bool setTimerSpeed()
{
    // compute the time period corresponding to the BPM frequency
    float time = 5.0 / (2.0 * player->bpm);
    int sec = (int)time;
    int usec = (int)((time - sec) * 1'000'000);

    // set the timer interval
    struct itimerval timer;

    timer.it_value.tv_sec = sec;
    timer.it_value.tv_usec = usec;
    timer.it_interval = timer.it_value;

    if (setitimer(ITIMER_REAL, &timer, nullptr) == -1) {
        std::cerr << "Error: unable to set the timer value to ";
        std::cerr << time << " s." << std::endl;
        return false;
    }

    // set the alarm signal
    if (signal(SIGALRM, sigalrmHandler) == SIG_ERR) {
        std::cerr << "Error: unable to set the timer callback." << std::endl;
        return false;
    }

    return true;
}

void cancelTimer()
{
    // reset the values to zero
    struct itimerval timer;

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval = timer.it_value;

    if (setitimer(ITIMER_REAL, &timer, nullptr) == -1) {
        std::cerr << "Error: unable to cancel the timer" << std::endl;
    }

    // set the alarm signal
    if (signal(SIGALRM, SIG_DFL) == SIG_ERR) {
        std::cerr << "Error: unable to cancel the timer callback." << std::endl;
    }
}

void song(const mod::Song* pSong, audio::IDriver* pDriver)
{
    // create the player structure
    std::cout << "Creating player ..." << std::endl;
    player = new Player;
    if (!player->create(pDriver, pSong, 8, 512)) {
        return;
    }

    // register the timer speed function to the player
    player->fnSetTimer = setTimerSpeed;


    // add SIGINT signal handler
    signal(SIGINT, sigintHandler);

    // set the timer
    std::cout << "Starting timer interrupt ..." << std::endl;
    if (!setTimerSpeed()) {
        player->destroy();
        return;
    }

    // mainloop
    std::cout << "Starting mainloop ..." << std::endl;
    player->is_playing = true;
    while (player->is_playing)
    {
        // avoid busy looping
        usleep(200);
    }

    // cancel timer
    std::cout << "Cancelling timer interrupt ..." << std::endl;
    cancelTimer();

    // release memory
    std::cout << "Releasing memory ..." << std::endl;
    player->destroy();
}


END_NAMESPACE(play)
END_NAMESPACE(mod)

