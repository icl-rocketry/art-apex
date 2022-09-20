#pragma once

#include "../util.h"
#include "hardware/flash.h"
#include "fs.hpp"

// 4096 byte buffer
#define DEFAULT_BUFFER_SIZE FLASH_SECTOR_SIZE

class File {
public:
    // DO NOT initialise outside FS unless you definitely know what you're doing
    File(uint32_t file_size, uint32_t buffer_size);
    ~File();

    template <typename T>
    bool append(T val);
    bool flush();

    bool read(size_t offset, void* dest, size_t len);

    uint32_t getSize();
    bool created();

private:
    size_t cursor;
    size_t buffer_size;
    uint8_t* buffer_start;

    // Start and end of the file in flash
    uint32_t start;
    uint32_t end;
    uint32_t curr;

    // Allow FS to access start, curr and end even though they're private
    friend class FS;
};