#pragma once


// This file defines a buffer class for anything that needs one.
// You can append to the buffer, read it, or reset it using the methods.

class Buffer {
public:
    Buffer(size_t buffer_size): buffer_size(buffer_size), cursor(0) {
        buffer_start = new uint8_t[buffer_size];
    }

    ~Buffer() {
        delete buffer_start;
    }

    // Returns false if the append failed
    template <typename T>
    bool append(T val) {
        size_t size = sizeof val;
        if (cursor + size > buffer_size) {
            return false;
        }
        
        memcpy(buffer_start + cursor, (const void*) &val, size);
        cursor += size;

        return true;
    }

    // Set all bytes after cursor to 0
    void clear() {
        memset(buffer_start + cursor, 0, buffer_size - cursor);
    }

    void reset() {
        cursor = 0;
    }

    size_t getBufferSize() {
        return buffer_size;
    }

    const uint8_t* getBufferStart() {
        return buffer_start;
    }

    // Read will return false if you try to read too many bytes
    bool read(void* dest, size_t len) {
        if (len > cursor) {
            return false;
        }

        memcpy(dest, buffer_start, len);
        return true;
    }

private:
    size_t cursor;
    const size_t buffer_size;
    uint8_t* buffer_start;
};