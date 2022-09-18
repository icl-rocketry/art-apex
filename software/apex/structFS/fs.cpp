#include "fs.hpp"
#include "hardware/flash.h"

uint32_t round_page(uint32_t val);

FS::FS(uint32_t size) {
    const uint32_t flash_start = round_page(__flash_binary_end);

    this->available_flash_start = (uint8_t *)flash_start;
    this->flash_end = (uint8_t *)(flash_start + size);
}

bool FS::AddFile(File& file) { 
    // Check if there's enough space to create a file
    uint32_t flash_start = (uint32_t)this->available_flash_start;
    const uint32_t flash_end = (uint32_t)this->flash_end;
    const uint32_t size = file.getSize();
    uint32_t remaining_space = flash_end - flash_start;
    if (size > remaining_space) {
        return false;
    }

    file.start = this->available_flash_start;

    // Files must have a whole number of pages
    file.end = (uint8_t*)round_page(flash_start + size);
    this->available_flash_start = file.end;
    return true;
}

/*
    We can only start writing at 4kb sector boundaries, we must round up to the next sector
    boundary. This bit trick first adds (4kb - 1b) to __flash_binary_end and then rounds down to
    the closest 4kb.
    https://stackoverflow.com/a/1766566
*/
uint32_t round_page(uint32_t val) {
    const uint32_t sector_minus_1 = FLASH_PAGE_SIZE - 1;
    return val + sector_minus_1 & ~sector_minus_1;
}