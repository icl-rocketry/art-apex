#pragma once

#include "module.hpp"
#include <Arduino.h>

#define SERIAL_STREAM_MAX_BYTES 64
class SerialStream : public Stream_ {
    SerialStream(HardwareSerial& serial) : serial(serial) {};

    // Override functions from Stream_
    virtual size_t get_max_bytes();
    virtual size_t recv(uint8_t* dest, size_t len);
    virtual bool send(const uint8_t* src, size_t len);
private:
    HardwareSerial& serial;
};