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
 * @brief	Function to play a sample
 */

//----- includes
#include "play.h"

#include <iostream>
#include <vector>

#include "../constants.h"
#include "../structures.h"
#include "../../audio/idriver.h"

BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(play)


void sample(int sample, const mod::Song* pSong, audio::IDriver* pDriver)
{
    // check boundaries
    if ((sample <= 0) || (sample > kMaxNumberSamples)) {
        std::cerr << "Error: sample value should be between 1 and " << (int)kMaxNumberSamples;
        std::cerr << std::endl;
        return;
    }

    // retrieve a pointer to the audio data
    const mod::Sample *sample_hdr = pSong->samples[sample - 1];
    if ((sample_hdr->length == 0) || (sample_hdr->pData == nullptr)) {
        std::cout << "Nothing to play as sample is empty." << std::endl;
        return;
    }

    // retrieve the audio parameters
    const audio::Parameters* pParams = pDriver->getParams();
    const int channels = pParams->channels;

    // compute the sample rate (C-2 with the sample finetune) and its duration
    double period = (8363.0 * kPeriodTable[kDefaultNote]) / kFineTune2Hz[sample_hdr->finetune & 0x0F];
    double sample_rate = kDefaultPeriodFreq / (period * 2.0);
    double duration = sample_hdr->length / sample_rate;
    std::cout << "Sample: rate = " << (int)sample_rate << " ";
    std::cout << "duration = " << duration << std::endl;

    // 16.16 fixed point resampling increment
    uint32_t step = (uint32_t)((sample_rate * 65536.0) / pParams->rate);

    // audio buffer
    const int frames_per_block = 1024;
    std::vector<int16_t> buffer(frames_per_block * channels);

    const int8_t* pData = sample_hdr->pData;
    uint64_t pos = 0;                   // 16.16 position in the sample
    uint64_t end = (uint64_t)sample_hdr->length << 16;
    int count = 0;
    while (pos < end)
    {
        // sample value scaled to 16-bit with the sample volume
        int value = pData[pos >> 16] * sample_hdr->volume * 4;

        for (int c = 0; c < channels; c++) {
            buffer[count++] = (int16_t)value;
        }

        pos += step;

        // we reached the available space in the buffer
        if (count == (int)buffer.size()) {
            pDriver->push((char*)buffer.data(), count * sizeof(int16_t), 1);
            count = 0;
        }
    }

    // flush the remaining frames only
    if (count > 0) {
        pDriver->push((char*)buffer.data(), count * sizeof(int16_t), 1);
    }
}

END_NAMESPACE(play)
END_NAMESPACE(mod)
