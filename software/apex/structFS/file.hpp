#pragma once

#include "../util.h"
#include "hardware/flash.h"
#include "fs.hpp"

// 1024 byte buffer
#define BUFFER_SIZE (FLASH_PAGE_SIZE * 4)

template <typename T>
class File {
public:
    // DO NOT initialise outside FS unless you definitely know what you're doing
    File(uint32_t size);

    bool append(T val);

    uint32_t getSize();
    bool created();

private:
    uint8_t* start;
    uint8_t* end;  
    uint8_t[BUFFER_SIZE] buffer; //TODO: Maybe heap allocate this instead?

    // Allow FS to access start and end even though they're private
    friend class FS;
};