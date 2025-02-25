// MIT License
//
// Copyright (c) 2022-2025 James Smith of OrangeFox86
// https://github.com/OrangeFox86/DreamcastControllerUsbPico
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "hal/MapleBus/MaplePacket.hpp"
#include "DreamcastPeripheralFunction.hpp"
#include "dreamcast_constants.h"

#include "hal/System/SystemMemory.hpp"

#include <memory>
#include <string.h>

#define U16_TO_UPPER_HALF_WORD(val) ((static_cast<uint32_t>(val) << 24) | ((static_cast<uint32_t>(val) << 8) & 0x00FF0000))
#define U16_TO_LOWER_HALF_WORD(val) (((static_cast<uint32_t>(val) << 8) & 0x0000FF00) | ((static_cast<uint32_t>(val) >> 8) & 0x000000FF))

namespace client
{
class DreamcastStorage : public DreamcastPeripheralFunction
{
public:
    //! Default constructor
    DreamcastStorage(std::shared_ptr<SystemMemory> systemMemory, uint32_t memoryOffset);

    //! Formats the storage
    //! @returns true iff format writes completed
    bool format();

    //! Handle packet meant for this peripheral function
    //! @param[in] in  The packet read from the Maple Bus
    //! @param[out] out  The packet to write to the Maple Bus when true is returned
    //! @returns true iff the packet was handled
    virtual bool handlePacket(const MaplePacket& in, MaplePacket& out) final;

    //! Called when player index changed or timeout occurred
    virtual void reset() final;

    //! @returns the function definition for this peripheral function
    virtual uint32_t getFunctionDefinition() final;

private:
    //! Sets 16-bit address value into FAT area and decrements pointer, when necessary
    //! @param[in, out] fatBlock  Pointer to next 16-bit FAT entry
    //! @param[in, out] lower  True when lower 16-bit to be set, false for upper
    //! @param[in] value  The FAT address value to write
    static void setFatAddr(uint32_t*& fatBlock, bool& lower, uint16_t value);

    //! Read a block of data, blocks until all data read
    //! @param[in] blockNum  The block number to read
    //! @returns pointer to a full block of data
    const uint8_t* readBlock(uint16_t blockNum);

    //! Write a block of data
    //! @param[in] blockNum  The block number to write
    //! @param[in, out] data  The block of data to write (system block may be overridden)
    //! @returns true if successful or false if a failure occurred
    bool writeBlock(uint16_t blockNum, void* data);

    //! Sets default media info words to out
    //! @param[out] out  Pointer to where media info will be written
    //! @param[in] infoOffset  Media info word offset
    //! @param[in] infoLen  Number of media info words to copy
    void setDefaultMediaInfo(uint32_t* out, uint8_t infoOffset = 0, uint8_t infoLen = 6);

    //! Sets default media info words to out with output word bytes flipped
    //! @param[out] out  Pointer to where media info will be written
    //! @param[in] infoOffset  Media info word offset
    //! @param[in] infoLen  Number of media info words to copy
    void setDefaultMediaInfoFlipped(uint32_t* out, uint8_t infoOffset = 0, uint8_t infoLen = 6);

    //! Flips the endianness of a word
    //! @param[in] word  Input word
    //! @returns output word
    static inline uint32_t flipWordBytes(const uint32_t& word)
    {
        return MaplePacket::flipWordBytes(word);
    }

public:
    static const uint16_t NUMBER_OF_PARTITIONS = 1;
    static const uint16_t BYTES_PER_BLOCK = 512;
    static const uint8_t WORD_SIZE = sizeof(uint32_t);
    static const uint16_t WORDS_PER_BLOCK = BYTES_PER_BLOCK / WORD_SIZE;
    static const uint8_t WRITES_PER_BLOCK = 4;
    static const uint8_t READS_PER_BLOCK = 1;
    static const bool IS_REMOVABLE = false;
    static const bool CRC_NEEDED = false;
    static const uint32_t MEMORY_SIZE_BYTES = 128 * 1024;
    static const uint32_t MEMORY_WORD_COUNT = MEMORY_SIZE_BYTES / WORD_SIZE;
    static const uint16_t NUM_BLOCKS = MEMORY_WORD_COUNT / WORDS_PER_BLOCK;
    static const uint32_t BYTES_PER_WRITE = (WRITES_PER_BLOCK > 0) ? (BYTES_PER_BLOCK / WRITES_PER_BLOCK) : 0;
    static const uint16_t SYSTEM_BLOCK_NO = (NUM_BLOCKS - 1);
    static const uint16_t NUM_SYSTEM_BLOCKS = 1;
    static const uint16_t MEDIA_INFO_WORD_OFFSET = 16;
    static const uint16_t SAVE_AREA_MEDIA_INFO_OFFSET = 4;
    static const uint16_t FAT_BLOCK_NO = SYSTEM_BLOCK_NO - NUM_SYSTEM_BLOCKS;
    static const uint16_t NUM_FAT_BLOCKS = 1;
    static const uint16_t FILE_INFO_BLOCK_NO = FAT_BLOCK_NO - NUM_FAT_BLOCKS;
    static const uint16_t NUM_FILE_INFO_BLOCKS = 13;
    static const uint16_t SAVE_AREA_BLOCK_NO = 31;
    static const uint16_t NUM_SAVE_AREA_BLOCKS = 200;

protected:
    //! The system memory object where data is to be written
    std::shared_ptr<SystemMemory> mSystemMemory;
    //! Byte offset into memory
    uint32_t mMemoryOffset;
    //! Storage for a single block of data for read or write purposes
    uint8_t mDataBlock[BYTES_PER_BLOCK];
};
}
