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
 * @brief	Audio Exceptions
 */

//----- guards
#ifndef AUDIO_EXCEPT_H
#define AUDIO_EXCEPT_H


//----- includes
#include <exception>
#include <string>

#include "../slMacros.h"


//----- classes
BEGIN_NAMESPACE(audio)

// base exception
class BaseExceptionError : public std::exception
{
    public:
        explicit BaseExceptionError(const char* message) :
            message_(message)
        { }

        virtual const char* what() const throw()
        {
            return message_;
        }

    private:
        const char *message_{nullptr};
};

// generic driver exception
class AudioError : public BaseExceptionError
{
    public:
        explicit AudioError(const char* message) :
            BaseExceptionError(message)
        { }
};

END_NAMESPACE(audio)

#endif // AUDIO_EXCEPT_H