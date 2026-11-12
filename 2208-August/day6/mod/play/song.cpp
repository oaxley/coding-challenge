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
 *
 * The song is no longer driven by a SIGALRM timer: a timer is never exactly in
 * sync with the sound card clock, so the audio buffer was either starving
 * (gaps = clicks) or growing. Instead each tick is rendered and written to the
 * (blocking) audio driver, which naturally paces the playback.
 */

//----- includes
#include "player/player.h"

#include <signal.h>

#include <iostream>

#include "../../slMacros.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)


//----- globals
namespace {

volatile sig_atomic_t stop_requested = 0;

// SIGINT handler
void sigintHandler(int signum)
{
    stop_requested = 1;
}

}


//----- functions
void song(const mod::Song* pSong, audio::IDriver* pDriver)
{
    // create the player structure
    std::cout << "Creating player ..." << std::endl;
    Player* player = new Player;
    if (!player->create(pDriver, pSong)) {
        delete player;
        return;
    }

    // add SIGINT signal handler
    stop_requested = 0;
    auto previous = signal(SIGINT, sigintHandler);

    // mainloop: each tick blocks until the audio driver accepts the data
    std::cout << "Starting mainloop ..." << std::endl;
    while (!stop_requested && player->processTick())
    { }

    std::cout << std::endl;
    signal(SIGINT, previous);

    // release memory
    std::cout << "Releasing memory ..." << std::endl;
    player->destroy();
    delete player;
}


END_NAMESPACE(play)
END_NAMESPACE(mod)
