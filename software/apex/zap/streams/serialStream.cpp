#include "serialStream.hpp"

bool SerialStream::send(const uint8_t* src, size_t len)  {
    return len == serial.write(src, len); // We only care if everything was written
}

size_t SerialStream::recv(uint8_t* dest, size_t len)  {
    return serial.readBytes(dest, len);
}

size_t SerialStream::get_max_bytes() {
    return SERIAL_STREAM_MAX_BYTES;
}