#pragma once

#include "file.hpp"
#include "../util.h"

/*
Filled out by linker - this is the memory address of the first free byte in flash to write to
See: https://kevinboone.me/picoflash.html?i=1
I'm ignoring the part that says everything must be at a 4kb boundary beacuse it doesn't make sense
*/
extern uint32_t __flash_binary_end;

class FS {
public:
    FS(uint32_t size);

    // Returns true if the file could be added
    bool AddFile(File& file);

private:
    uint8_t* available_flash_start;
    uint8_t* flash_end;
};
