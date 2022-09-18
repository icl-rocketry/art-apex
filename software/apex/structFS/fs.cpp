#include "fs.hpp"

#include "pico/stdlib.h"
#include "hardware/flash.h"

uint32_t round_page(uint32_t val);

FS::FS(uint32_t size) {
    const uint32_t flash_start = round_page(__flash_binary_end);

    this->available_flash_start = (uint8_t *)flash_start;
    this->flash_end = (uint8_t *)(flash_start + size);
}

template <typename T>
File<T> FS::AddFile(uint32_t size) { 
    
}

/*
    We can only start writing at 4kb sector boundaries, we must round up to the next sector
    boundary. This bit trick first adds (4kb - 1b) to __flash_binary_end and then rounds down to
    the closest 4kb.
    https://stackoverflow.com/a/1766566
*/
uint32_t round_page(uint32_t val) {
    const uint32_t sector_minus_1 = FLASH_SECTOR_SIZE - 1;
    return val + sector_minus_1 & ~sector_minus_1;
}