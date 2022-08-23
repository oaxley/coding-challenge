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
 * @brief	MOD Tracker file loader - Implementation
 */

#include <fstream>
#include <iostream>
#include <memory>

#include "constants.h"
#include "structures.h"
#include "exceptions.h"
#include "loader.h"


BEGIN_NAMESPACE(mod)
BEGIN_NAMESPACE(file)

//----- internal structure
struct Loader::OpaqueData
{
    // members
    std::ifstream handle_;
    std::string marker_;
    Song* pSong_;

    // methods
    void create(std::string& filename);
    void destroy();

    uint16_t readWord();
    uint8_t readByte();
    void readBytes(uint16_t count, char* buffer);

#if DEBUG
    void printHeader();
#endif
};

/* initialize the internal structure
 *
 * Args:
 *      filename: the name of the tracker MOD file
 */
void Loader::OpaqueData::create(std::string& filename)
{
    // open the file and retrieve the marker
    handle_ = std::ifstream(filename.c_str(), std::ios::binary);

    // retrieve the marker
    char buffer[5] = {0};
    handle_.seekg(kMarkerPosition, std::ios::beg);
    handle_.read(&buffer[0], 4);
    handle_.seekg(0, std::ios::beg);

    marker_ = std::string(buffer);

    // allocate memory for the song structure
    pSong_ = new Song;
    if (pSong_ == nullptr) {
        throw OutOfMemoryError("Unable to allocate memory for the song structure.");
    }
}

/* destroy the internal structure */
void Loader::OpaqueData::destroy()
{
    delete pSong_;
}

/* read a 16-bit word */
uint16_t Loader::OpaqueData::readWord()
{
    char buffer[2] = {0};
    handle_.read(&buffer[0], 2);

    uint8_t msb = static_cast<uint8_t>(buffer[0]);
    uint8_t lsb = static_cast<uint8_t>(buffer[1]);

    return static_cast<uint16_t>((msb << 8) + lsb);
}

/* read a 8-bit byte */
uint8_t Loader::OpaqueData::readByte()
{
    char value;
    handle_.read(&value, 1);
    return static_cast<uint8_t>(value);
}

/* read multiple single bytes
 *
 * Args:
 *      count: the number of bytes to read
 *      buffer: the destination buffer
 */
void Loader::OpaqueData::readBytes(uint16_t count, char* buffer)
{
    handle_.read(buffer, count);
}

//----- class
/* constructor
 *
 * Args:
 *      filename: the name of the MOD tracker file
 */
Loader::Loader(std::string& filename) :
    data_(new OpaqueData)
{
    if (data_ == nullptr) {
        throw OutOfMemoryError("Unable to allocate memory during data initialization.");
    }

    data_->create(filename);
}

/* destructor */
Loader::~Loader()
{
    data_->destroy();
}


END_NAMESPACE(file)
END_NAMESPACE(mod)