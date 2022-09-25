#include "file.hpp"
#include "../util/Profiling.h"

// If the buffer_size isn't a multiple of 256, it'll be rounded up
File::File(uint32_t file_size, uint32_t buffer_size = DEFAULT_FILE_BUFFER_SIZE) {
    start = 0;
    curr = 0;
    end = file_size;

    if (buffer_size == 0) {
        buffer_size = DEFAULT_FILE_BUFFER_SIZE;
    } else if (buffer_size % FLASH_PAGE_SIZE != 0) {
        buffer_size = round_up_power_of_2(buffer_size, FLASH_PAGE_SIZE);
    }
    buffer = new Buffer(buffer_size);
}

File::~File() {
    delete buffer;
}

void File::configureFlash(uint32_t start, uint32_t end) {
    this->start = start;
    this->curr = start; 
    this->end = end;
}

// Erase the flash, because otherwise you can't write to it
void File::makeWriteable() {
    // It's rude to interrupt
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(start - XIP_BASE, end - start);
    restore_interrupts (ints);
}

uint32_t File::getSize() {
    return end - start;
}

bool File::created() {
    return start != 0;
}

template <typename T>
bool File::append(T val) {
    if (!created() || curr >= end) {
        return false;
    }

    if (!buffer->append(val)) {
        if (!flush()) {
            return false;
        }
        buffer->append(val);
    }
    return true;
}

bool File::flush() {
    PROFILE_FUNCTION()
    //set any remaining bytes to 0
    buffer->clear();

    size_t buffer_size = buffer->getBufferSize();

    // It's rude to interrupt
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(curr - XIP_BASE, buffer->getBufferStart(), buffer_size);
    restore_interrupts (ints);
    
    buffer->reset();
    curr += buffer_size;
    return true;
}

bool File::read(size_t offset, void* dest, size_t len) {
    return memcpy(dest, (const void*)(start+offset), len) != nullptr;
}