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
 * @brief	MOD Player exceptions
 */
//----- guards
#ifndef MOD_EXCEPT_H
#define MOD_EXCEPT_H

//----- includes
#include <exception>
#include <string>

#include "../slMacros.h"


BEGIN_NAMESPACE(mod)

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
        const char* message_{nullptr};
};

// not enough memory error
class OutOfMemoryError : public BaseExceptionError
{
    public:
        explicit OutOfMemoryError(const char* message) :
            BaseExceptionError(message)
        { }
};

// invalid mod header/parameters
class InvalidModParameter : public BaseExceptionError
{
    public:
        explicit InvalidModParameter(const char* message) :
            BaseExceptionError(message)
        { }
};

END_NAMESPACE(mod)

#endif // MOD_EXCEPT_H