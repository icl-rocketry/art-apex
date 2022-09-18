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
    if (!created()) {
        return false;
    }

    size_t size = sizeof(T);
    // If the buffer is full, write to flash
    if (cursor + size > BUFFER_SIZE) {
        //set any remaining bytes to 0
        while (cursor < BUFFER_SIZE) {
            buffer_start[cursor] = 0;
        }

        flash_range_program(curr, buffer_start, BUFFER_SIZE);
        cursor = 0;
    }

    if (memcpy(buffer_start + cursor, (const void*) val, size) == nullptr) {
        return false;
    }

    cursor += size;
    return true;
}

bool File::read(size_t offset, void* dest, size_t len) {
    return memcpy(dest, (const void*)(start+offset), len) != nullptr;
}