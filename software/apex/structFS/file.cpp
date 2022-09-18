#include "file.hpp"

File::File(uint32_t size) {
    start = 0;
    curr = 0;
    end = size;

    buffer_start = new uint8_t[BUFFER_SIZE];
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
    if (cursor + size > BUFFER_SIZE) {
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
    if (memset(buffer_start + cursor, 0, BUFFER_SIZE - cursor) == nullptr) {
        return false;
    }

    // It's rude to interrupt
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(curr - XIP_BASE, buffer_start, BUFFER_SIZE);
    restore_interrupts (ints);
    cursor = 0;
    curr += BUFFER_SIZE;
    return true;
}

bool File::read(size_t offset, void* dest, size_t len) {
    return memcpy(dest, (const void*)(start+offset), len) != nullptr;
}