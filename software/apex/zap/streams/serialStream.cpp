#include "serialStream.hpp"

bool SerialStream::send(const uint8_t* src, size_t len)  {
    return len == serial.write(src, len); // We only care if everything was written
}

bool SerialStream::recv(uint8_t* dest, size_t len)  {
    return len == serial.readBytes(dest, len); // We only care if everything was read
}

size_t SerialStream::get_max_bytes() {
    return SERIAL_STREAM_MAX_BYTES;
}