#include "fs.hpp"
#include "hardware/flash.h"

uint32_t round_up_power_of_2(uint32_t val, uint32_t to);

FS::FS(uint32_t size) {
    available_flash_start = round_up_power_of_2((uint32_t)&__flash_binary_end, FLASH_SECTOR_SIZE);
    flash_end = available_flash_start + size;
}

bool FS::AddFile(File& file) { 
    // Check if there's enough space to create a file
    const uint32_t size = file.getSize();
    uint32_t remaining_space = flash_end - available_flash_start;
    
    if (size > remaining_space) {
        return false;
    }

    // Files must have a whole number of sectors
    uint32_t end = round_up_power_of_2(available_flash_start + size, FLASH_SECTOR_SIZE);
    file.configureFlash(available_flash_start, end);
    available_flash_start = end;
    return true;
}