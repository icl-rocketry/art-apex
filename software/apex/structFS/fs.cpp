#include "fs.hpp"
#include "hardware/flash.h"

uint32_t round_power_of_2(uint32_t val, uint32_t to);

FS::FS(uint32_t size) {
    available_flash_start = round_power_of_2((uint32_t)&__flash_binary_end, FLASH_SECTOR_SIZE);
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
    file.start = available_flash_start;
    file.curr = available_flash_start;
    file.end = round_power_of_2(available_flash_start + size, FLASH_SECTOR_SIZE);
    available_flash_start = file.end;
    return true;
}

// Erase the flash, because otherwise you can't write to it
void FS::MakeWritable(File& file) {
    // It's rude to interrupt
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(file.start - XIP_BASE, file.end - file.start);
    restore_interrupts (ints);
}

/*
    We can only start writing at 4kB page boundaries, we must round up to the next page
    boundary. This bit trick first adds (4kB - 1B) to __flash_binary_end and then rounds down to
    the closest 4kB.
    https://stackoverflow.com/a/1766566
*/
uint32_t round_power_of_2(uint32_t val, uint32_t to) {
    const uint32_t sector_minus_1 = to - 1;
    return val + sector_minus_1 & ~sector_minus_1;
}