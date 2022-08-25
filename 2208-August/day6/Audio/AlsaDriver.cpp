/* -*- coding: utf-8 -*-
 * vim: ft=cpp
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	Alsa Audio Driver
 */

//----- includes
#include <alsa/asoundlib.h>

#include "AlsaDriver.h"
#include "except.h"


BEGIN_NAMESPACE(audio)

//----- class structure
struct AlsaDriver::OpaqueData
{
    // members
    snd_pcm_t* device;
    Parameters* params;

    // methods
    void create();
    void destroy();
};

// initialize the structure
void AlsaDriver::OpaqueData::create()
{
    device = nullptr;
    params = nullptr;
}

// destroy the structure
void AlsaDriver::OpaqueData::destroy()
{ }


//----- functions

/* constructor */
AlsaDriver::AlsaDriver() :
    data_(new OpaqueData)
{
    if (data_ == nullptr) {
        throw AudioError("Unable to allocate memory during data initialization.");
    }
    data_->create();
}

/* destructor */
AlsaDriver::~AlsaDriver()
{
    data_->destroy();
}

/* open alsa backend */
void AlsaDriver::open(Parameters* pParams)
{
    // keep track of the pointer
    data_->params = pParams;

    // open the device
    int err = snd_pcm_open(&data_->device, data_->params->name, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        throw AudioError("Unable to open the Audio Interface.");
    }
}

/* close alsa backend */
void AlsaDriver::close()
{
    int error;

    // drain the audio interface
    if (snd_pcm_drain(data_->device) < 0) {
        throw AudioError("Unable to drain the Audio Interface during closing.");
    }

    // close the audio interface
    if (snd_pcm_close(data_->device) < 0) {
        throw AudioError("Unable to close the Audio Interface.");
    }
}

/* setup alsa backend */
void AlsaDriver::setup()
{
    int error;

    // set the parameters
    error = snd_pcm_set_params(data_->device,
                               SND_PCM_FORMAT_FLOAT,
                               SND_PCM_ACCESS_RW_INTERLEAVED,
                               data_->params->channels,
                               data_->params->rate,
                               1,
                               500'000
    );

    if (error < 0) {
        throw AudioError("Unable to setup the Audio Interface.");
    }

    // prepare the device for playing
    if (snd_pcm_prepare(data_->device) < 0) {
        throw AudioError("Unable to prepare the Audio Interface.");
    }
}

/* push data to Alsa */
void AlsaDriver::push(char* buffer, int frames, int count)
{
    while(count--) {
        int retval = snd_pcm_writei(data_->device, buffer, frames);

        if (retval != frames) {
            throw AudioError("Unable to play all the frames.");
        }
    }
}

END_NAMESPACE(audio)