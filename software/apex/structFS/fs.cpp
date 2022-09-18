#include "fs.hpp"
#include "hardware/flash.h"

uint32_t round_page(uint32_t val);

FS::FS(uint32_t size) {
    const uint32_t flash_start = round_page(__flash_binary_end);

    available_flash_start = (uint8_t *)flash_start;
    flash_end = (uint8_t *)(flash_start + size);
}

bool FS::AddFile(File& file) { 
    // Check if there's enough space to create a file
    uint32_t flash_start = (uint32_t)available_flash_start;
    const uint32_t flash_end = (uint32_t)flash_end;
    const uint32_t size = file.getSize();
    uint32_t remaining_space = flash_end - flash_start;
    if (size > remaining_space) {
        return false;
    }

    // Files must have a whole number of pages
    file.start = available_flash_start;
    file.curr = flash_start;
    file.end = (uint8_t*)round_page(flash_start + size);
    available_flash_start = file.end;
    return true;
}

/*
    We can only start writing at 256B page boundaries, we must round up to the next page
    boundary. This bit trick first adds 255B to __flash_binary_end and then rounds down to
    the closest 256B.
    https://stackoverflow.com/a/1766566
*/
uint32_t round_page(uint32_t val) {
    const uint32_t sector_minus_1 = FLASH_PAGE_SIZE - 1;
    return val + sector_minus_1 & ~sector_minus_1;
}