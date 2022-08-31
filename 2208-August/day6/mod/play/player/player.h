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
 * @brief	Main include header
 */
#ifndef MOD_PLAY_PLAYER_H
#define MOD_PLAY_PLAYER_H

#include "../../../slMacros.h"
#include "../../../audio/audio.h"
#include "../../constants.h"
#include "../../structures.h"

#include "channel.h"

BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)

// player structure
struct Player
{
    //----- members
    audio::IDriver* pDriver = nullptr;
    const mod::Song* pSong = nullptr;
    const audio::Parameters* pParams = nullptr;

    bool (*fnSetTimer)() { nullptr };

    bool is_playing = false;

    int bpm = 0;
    int speed = 0;
    int tick = 0;
    int row = 0;
    int order = 0;
    int pattern = 0;
    int samples_per_tick = 0;

    bool pattern_jump = false;
    bool pattern_break = false;

    int pattern_jump_value = 0;
    int pattern_break_value = 0;

    int pattern_delay = 0;

    Channel channels[kMaxChannels];
    int volume_table[kMaxVolume][256];

    //----- audio buffer
    uint8_t* audio_buffer = nullptr;
    uint16_t audio_ptr;
    uint16_t audio_block_size;
    uint16_t audio_block_count;
    uint16_t audio_size;


    //----- methods
    bool create(audio::IDriver*, const mod::Song*, int, int);
    void destroy();

    void updateNote();
    void updateEffect();

private:
    void createVolTable();
    float bpmToFrequency(int bpm);
    void setPanning(int ch);

    //----- replay functions
    void playInstrument();

    //----- resampling functions
    void setFrequency(int ch, int period);
    float amigaFrequency(int period);
    float amigaPeriod(int note, int finetune);

    //----- volume functions
    void setVolume(int ch, int value);
    void slideVolume(int ch, int value);

    //----- vibrato / portamento effects
    void doVibrato(int ch);
    void doPortamento(int ch);
    void doTremolo(int ch);
};

END_NAMESPACE(play)
END_NAMESPACE(mod)

#endif // MOD_PLAY_PLAYER_H