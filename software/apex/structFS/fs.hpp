#pragma once

#include "file.hpp"

#define uint32_t unsigned int // Just getting the vscode extension to shut up

/*
Filled out by linker - this is the memory address of the first free byte in flash to write to
See: https://kevinboone.me/picoflash.html?i=1
*/
extern uint32_t __flash_binary_end;

class FS {
public:
    FS(uint32_t size);

    //size is the number of bytes that the file should have
    template <typename T>
    File<T> AddFile(uint32_t size);

private:
    uint8_t* available_flash_start;
    uint8_t* flash_end;
};
