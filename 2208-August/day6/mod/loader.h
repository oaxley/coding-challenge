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
 * @brief	MOD Tracker file loader
 */
#ifndef MOD_LOADER_H
#define MOD_LOADER_H

#include <string>
#include <memory>

#include "../slMacros.h"



BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(file)

class Loader
{
    public:
        Loader(std::string& filename);
        virtual ~Loader();

        bool isValidFile();
        void load();
#if DEBUG
        void printHeader();
#endif

    private:
        struct OpaqueData;
        std::unique_ptr<OpaqueData> data_;
};

END_NAMESPACE(file)
END_NAMESPACE(mod)

#endif // MOD_LOADER_H