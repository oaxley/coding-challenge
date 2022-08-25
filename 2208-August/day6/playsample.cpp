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
 * @brief	Play a MOD sample
 */

//----- includes
#include "playsample.h"

#include <unistd.h>

#include <iostream>
#include <cstring>

#include "mod/constants.h"
#include "mod/structures.h"
#include "Audio/idriver.h"



//----- function
void playSample(int sample, const mod::file::Song* pSong, audio::IDriver* pDriver)
{
    // check boundaries
    if ((sample <= 0) || (sample > pSong->header->max_samples)) {
        std::cerr << "Error: sample value should be between 1 and " << (int)pSong->header->max_samples;
        std::cerr << std::endl;
        return;
    }

    // retrieve a pointer to the audio data
    mod::file::Sample *sample_hdr = pSong->samples[sample - 1];
    if (sample_hdr->length == 0) {
        std::cout << "Nothing to play as sample is empty." << std::endl;
        return;
    }

    // audio buffer
    const int bufsize = 1024;
    uint8_t* pBuffer = new uint8_t[bufsize];
    memset(pBuffer, 0, bufsize);

    // retrieve the audio parameters
    const audio::Parameters* pParams = pDriver->getParams();

    // compute the sample rate and its total length
    float sample_rate = mod::file::kFineTuneHertz[sample_hdr->finetune];
    float duration = sample_hdr->length / sample_rate;
    std::cout << "Sample: rate = " << sample_rate << " ";
    std::cout << "duration = " << duration << std::endl;

    // compute the max length to play this sample at our current playing rate
    int max_length = (int)(pParams->rate * duration);
#if DEBUG
    std::cerr << "max_length = " << max_length << std::endl;
#endif

    // resampling increment
    float incr = sample_rate / pParams->rate;
#if DEBUG
    std::cerr << "resampling incr = " << incr << std::endl;
#endif

    // sample data and audio buffer counter
    char* pData = sample_hdr->pData;
    int bufcount = 0;
    for (int i = 0; i < max_length; i++)
    {
        int value;
        int pos = (int)(i*incr);

        if (pos > sample_hdr->length) {
            value = 0;
        } else {
            value = pData[pos] + 128;
        }

        // adjust the value with the volume
        value = (64 * value) / 256;

        // fill the buffer depending on the number of audio channels
        switch (pParams->channels)
        {
            case 1:     // MONO
                pBuffer[bufcount++] = value;
                break;
            case 2:     // STEREO
                pBuffer[bufcount++] = value;        // LEFT channel
                pBuffer[bufcount++] = value;        // RIGHT channel
                break;
        }

        // we reached the available space in the buffer
        if (bufcount == bufsize) {
            pDriver->push((char*)pBuffer, bufsize, 1);

            bufcount = 0;
            memset(pBuffer, 0, bufsize);
        }
    }

    if (bufcount > 0) {
        pDriver->push((char*)pBuffer, bufsize, 1);
    }

    delete [] pBuffer;

}
