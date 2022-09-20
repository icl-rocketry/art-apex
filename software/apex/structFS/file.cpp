#include "file.hpp"

// If the buffer_size isn't a multiple of 256, it'll be rounded up
File::File(uint32_t file_size, uint32_t buffer_size = DEFAULT_BUFFER_SIZE) {
    start = 0;
    curr = 0;
    end = file_size;

    if (buffer_size == 0) {
        buffer_size = DEFAULT_BUFFER_SIZE;
    } else if (buffer_size % FLASH_PAGE_SIZE != 0) {
        buffer_size = round_up_power_of_2(buffer_size, FLASH_PAGE_SIZE);
    }
    this->buffer_size = buffer_size;
    buffer_start = new uint8_t[buffer_size];
    cursor = 0;
}

File::~File() {
    delete buffer_start;
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

    size_t size = sizeof(T);
    // If the buffer is full, write to flash
    if (cursor + size > buffer_size) {
        if (!flush()) {
            return false;
        }
    }

    if (memcpy(buffer_start + cursor, (const void*) &val, size) == nullptr) {
        return false;
    }

    cursor += size;
    return true;
}

bool File::flush() {
    //set any remaining bytes to 0
    if (memset(buffer_start + cursor, 0, buffer_size - cursor) == nullptr) {
        return false;
    }

    // It's rude to interrupt
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(curr - XIP_BASE, buffer_start, buffer_size);
    restore_interrupts (ints);
    cursor = 0;
    curr += buffer_size;
    return true;
}

bool File::read(size_t offset, void* dest, size_t len) {
    return memcpy(dest, (const void*)(start+offset), len) != nullptr;
}