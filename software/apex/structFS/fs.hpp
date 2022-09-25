#pragma once

#include "file.hpp"
#include <Arduino.h>
#include "../util/uint.hpp"

/*
Filled out by linker - this is the memory address of the first free byte in flash to write to
See: https://kevinboone.me/picoflash.html?i=1
*/
extern uint32_t __flash_binary_end;

class FS {
public:
    FS(uint32_t size);

    // Returns true if the file could be added
    bool AddFile(File& file);

private:
    uint32_t available_flash_start;
    uint32_t flash_end;
};
