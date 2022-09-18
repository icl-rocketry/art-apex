#include "file.hpp"

File::File(uint32_t size) {
    this->start = nullptr;
    this->end = (uint8_t*)size;
    this->curr = nullptr;

    this->buffer_start = new uint8_t[BUFFER_SIZE];
    this->cursor = 0;
}

uint32_t File::getSize() {
    return (uint32_t)this->end - (uint32_t)this->start;
}

bool File::created() {
    return this->start != nullptr;
}

template <typename T>
bool File::append(T val) {
    if (!this->created()) {
        return false;
    }

    size_t size = sizeof T;
    // If the buffer is full, write to flash
    if (this->cursor + size > BUFFER_SIZE) {
        //set any remaining bytes to 0
        while (this->cursor < BUFFER_SIZE) {
            this->buffer_start[this->cursor] = 0;
        }

        flash_range_program(this->curr, this->buffer_start, BUFFER_SIZE);
        this->cursor = 0;
    }

    if (memcpy(this->buffer_start[this->cursor], (const void*) val, size) == nullptr) {
        return false;
    }

    this->cursor += size;
    return true;
}