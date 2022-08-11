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
 * @brief	PulseAudio Driver
 */

//----- guards
#ifndef PULSE_DRIVER_H
#define PULSE_DRIVER_H

//----- includes
#include <memory>
#include "audiodriver/idriver.h"

//----- class
class PulseDriver : public Audio::IDriver
{
    public:     // public methods
        PulseDriver();
        virtual ~PulseDriver();

        virtual void open(Audio::Parameters* pParams);
        virtual void close();
        virtual void setup();
        virtual void push(float* buffer, int frames, int count);

    private:    // private members
        struct OpaqueData;
        std::unique_ptr<OpaqueData> data_;
};

#endif // PULSE_DRIVER_H