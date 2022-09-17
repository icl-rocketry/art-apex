#pragma once

#include <Arduino.h>

/*
Filled out by linker - this is the memory address of the first free byte in flash to write to
See: https://kevinboone.me/picoflash.html?i=1
*/
extern uint32_t __flash_binary_end;

template <typename T>
class File {

};

class FS {
public:
    FS(uint32_t size);

    template <typename T>
    File<T> AddFile(uint32_t size);

private:
    uint8_t* available_flash_start;
    uint8_t* flash_end; 
};
