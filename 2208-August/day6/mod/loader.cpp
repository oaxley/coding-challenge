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

#include <memory>
#include <fstream>
#include <iomanip>
#include <iostream>

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
    void readString(uint16_t count, char* buffer);

    uint8_t loadChannelCount();
    uint8_t loadSampleInformation();

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
    // delete the samples in the list
    for (Sample* pSample : pSong_->samples)
        delete pSample;

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

/* read a null-terminated string and remove non printable characters
 *
 * Args:
 *      count: the number of bytes to read
 *      buffer: the destination buffer
 */
void Loader::OpaqueData::readString(uint16_t count, char* buffer)
{
    // read the string
    handle_.read(buffer, count - 1);

    // remove non printable characters
    for (int i = 0; i < count - 2; i++)
    {
        if (buffer[i] < 32) {
            buffer[i] = ' ';
        }
    }

    // properly terminate the string
    buffer[count - 1] = '\0';
}

/* determine the number of channels in thr file
 *
 * Returns:
 *      The number of channels found in the file
 */
uint8_t Loader::OpaqueData::loadChannelCount()
{
    uint8_t channels = 0;

    if ((marker_ == std::string("M.K.")) || (marker_ == std::string("4CHN"))) {
        channels = 4;
    } else {
        if (marker_ == std::string("6CHN")) {
            channels = 6;
        } else {
            if (marker_ == std::string("8CHN")) {
                channels = 8;
            } else {
                throw InvalidModParameter("Invalid number of channels.");
            }
        }
    }

    return channels;
}

/* load samples information from the file
 *
 * Returns:
 *      The number of samples found in the file
 */
uint8_t Loader::OpaqueData::loadSampleInformation()
{
    uint8_t samples = 0;

    for (int i = 0; i < kMaxNumberSamples; i++)
    {
        // allocate memory for a new sample header
        Sample* pSample = new Sample;
        if (pSample == nullptr) {
            throw OutOfMemoryError("Unable to allocate memory for Sample element.");
        }

        // data will be setup later
        pSample->pdata = nullptr;

        // read the sample name and remove non printable chars
        readString(kSampleNameLength, (char*)pSample->name);

        // sample length in bytes (stored as number of words)
        pSample->length = readWord() * 2;
        if (pSample->length > 0) {
            samples++;
        }

        // finetune is between -8 and +7
        char value = readByte();
        pSample->finetune = (value > 7) ? (value - 16) : value;

        // volume
        pSample->volume = readByte();

        // loops in bytes (stored as number of words)
        pSample->loop_start = readWord() * 2;
        pSample->loop_length = readWord() * 2;

        // add the sample to the list
        pSong_->samples.push_back(pSample);
    }

    return samples;
}


/* print debug information */
#if DEBUG
void Loader::OpaqueData::printHeader()
{
    std::cout << "====== Music MOD Information ======" << std::endl;
    std::cout << "Song Name   : " << pSong_->header->title << std::endl;
    std::cout << "Song Length : " << static_cast<int>(pSong_->header->length) << std::endl;
    std::cout << "Speed / BPM : " << static_cast<int>(pSong_->header->speed);
    std::cout << " / " << static_cast<int>(pSong_->header->bpm) << std::endl;
    std::cout << "Channels    : " << static_cast<int>(pSong_->header->channels) << std::endl;
    std::cout << "Max Pattern : " << static_cast<int>(pSong_->header->max_pattern) << std::endl;

    std::cout << "Order       : ";

    std::cout << "[ ";
    for (int i = 0; i < pSong_->header->length; i++) {
        std::cout << static_cast<int>(pSong_->header->order[i]) << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Max Samples : " << static_cast<int>(pSong_->header->max_samples) << std::endl;
    int counter = 1;
    for (Sample* pSample : pSong_->samples)
    {
        if (pSample->length > 0) {
            std::cout << std::setw(2) << counter << " | ";
            std::cout << std::setw(22) << pSample->name << " | ";
            std::cout << std::setw(10) << static_cast<int>(pSample->length) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->finetune) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->volume) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->loop_start) << " | ";
            std::cout << std::setw(7) << static_cast<int>(pSample->loop_length) << " | ";
            std::cout << std::endl;
        }

        counter++;
    }
}
#endif


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

/* check if the file is a valid MOD tracker file
 *
 * Returns:
 *      true if the file is valid, false otherwise
 */
bool Loader::isValidFile()
{
    // check for this markers
    std::string valid[] = { "M.K.", "4CHN", "6CHN", "8CHN" };

    for (auto& marker : valid)
    {
        if (data_->marker_ == marker)
            return true;
    }

    return false;
}

/* load the tracker song in memory */
void Loader::load()
{
    // instantiate a new header
    Header* hdr = new Header();
    if (hdr == nullptr) {
        throw OutOfMemoryError("Unable to allocate memory for MODHeader.");
    }

    // intialize the default values
    hdr->bpm = 125;
    hdr->speed = 6;

    // number of channels
    hdr->channels = data_->loadChannelCount();

    // read the song name (and remove non printable chars)
    data_->readString(kSongTitleLength, (char*)hdr->title);

    // read the samples information
    hdr->max_samples = data_->loadSampleInformation();

    // song length
    hdr->length = data_->readByte();

    // unused
    data_->readByte();

    // pattern order information
    uint8_t max_value = 0;
    for (int i = 0; i < kMaxNumberPattern; i++)
    {
        uint8_t value = data_->readByte();
        hdr->order[i] = value;
        if (value > max_value) {
            max_value = value;
        }
    }
    hdr->max_pattern = max_value;

    // transfer ownership of header to song object
    data_->pSong_->header = std::unique_ptr<Header>(hdr);
}

#if DEBUG
void Loader::printHeader()
{
    data_->printHeader();
}
#endif

END_NAMESPACE(file)
END_NAMESPACE(mod)