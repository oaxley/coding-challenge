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
 * @brief	Audio Driver Interface
 */

//----- guard
#ifndef IAUDIO_DRIVER_H
#define IAUDIO_DRIVER_H

//----- includes
#include "params.h"

// Audio Namespace
namespace Audio
{

//----- classes
class IDriver
{
    public:     // public methods
        IDriver()
        { }

        virtual ~IDriver()
        { }

        virtual void open(Audio::Parameters* pParams) = 0;
        virtual void close() = 0;
        virtual void setup() = 0;
        virtual void push(float* buffer, int frames, int count) = 0;

    private:    // private members
};

};  // Audio Namespace

#endif // IAUDIO_DRIVER_H