#pragma once

#include "../util.h"
#include "fs.hpp"

template <typename T>
class File {
public:
    // DO NOT initialise outside FS unless you definitely know what you're doing
    File(uint32_t size);

    uint32_t getSize();

private:
    uint8_t* start; 
    uint8_t* end;   

    // Allow FS to access start and end even though they're private
    friend class FS;
};