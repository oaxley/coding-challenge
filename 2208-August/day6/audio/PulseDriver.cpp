/* -*- coding: utf-8 -*-
 * vim: set ft=cpp
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	PulseAudio Driver Implementation
 */

//----- includes
#include <pulse/simple.h>
#include <pulse/error.h>

#include "except.h"
#include "PulseDriver.h"

BEGIN_NAMESPACE(audio)

//----- class structure
struct PulseDriver::OpaqueData
{
    // members
    pa_simple* hndPulse;

    // methods
    void create();
    void destroy();
};

// initialize the structure
void PulseDriver::OpaqueData::create()
{
    hndPulse = nullptr;
}

// destroy the structure
void PulseDriver::OpaqueData::destroy()
{ }


//----- functions

/* constructor */
PulseDriver::PulseDriver() :
    data_(new OpaqueData)
{
    if (data_ == nullptr) {
        throw AudioError("Unable to allocate memory during data initialization.");
    }
    data_->create();
}

/* destructor */
PulseDriver::~PulseDriver()
{
    data_->destroy();
}

/* open pulse backend */
void PulseDriver::open(Parameters* pParams)
{
    // keep track of the pointer
    pParams_ = pParams;

    // sample format specifications
    pa_sample_spec ss = {
        .format = PA_SAMPLE_U8,        // manually set for now
        .rate = static_cast<unsigned int>(pParams->rate),
        .channels = static_cast<unsigned char>(pParams->channels)
    };

    // open pulse
    int error;
    data_->hndPulse = pa_simple_new(nullptr, pParams->name, PA_STREAM_PLAYBACK, nullptr, pParams->name, &ss, nullptr, nullptr, &error);
    if (data_->hndPulse == nullptr) {
        throw AudioError("Unable to open the Audio Interface.");
    }
}

/* close pulse backend */
void PulseDriver::close()
{
    if (data_->hndPulse == nullptr) {
        throw AudioError("PulseDriver not initialized!");
    }

    // drain the audio interface
    int error;
    if (pa_simple_drain(data_->hndPulse, &error) < 0) {
        throw AudioError("Unable to drain the Audio Interface during closing.");
    }

    // close pulse connection
    pa_simple_free(data_->hndPulse);
    data_->hndPulse = nullptr;
}

void PulseDriver::setup()
{
    // nothing to be done here
}

void PulseDriver::push(char* buffer, int frames, int count)
{
    int error;
    while (count--) {
        int retval = pa_simple_write(data_->hndPulse, buffer, frames, &error);
        if (retval < 0) {
            throw AudioError("Unable to play all the frames.");
        }
    }
}

END_NAMESPACE(audio)
