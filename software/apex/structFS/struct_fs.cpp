#include "struct_fs.hpp"

#include "pico/stdlib.h"
#include "hardware/flash.h"

FS::FS(uint32_t size) {
    /*
    We can only start writing at 4kb sector boundaries, we must round up to the next sector
    boundary. This bit trick first adds (4kb - 1b) to __flash_binary_end and then rounds down to
    the closest 4kb.
    https://stackoverflow.com/a/1766566
    */
    const uint32_t sector_minus_1 = FLASH_SECTOR_SIZE - 1;
    const uint32_t flash_start = __flash_binary_end + sector_minus_1 & ~sector_minus_1;

    this->available_flash_start = (uint8_t *)flash_start;
    this->flash_end = (uint8_t *)(flash_start + size);
}

