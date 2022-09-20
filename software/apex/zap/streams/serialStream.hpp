#pragma once

#include "module.hpp"
#include <Arduino.h>

class SerialStream : public Stream_ {
    SerialStream(HardwareSerial& serial) : serial(serial) {};

    // Override functions from Stream_
    virtual bool recv(uint8_t* dest, size_t len);
    virtual bool send(const uint8_t* src, size_t len);
private:
    HardwareSerial& serial;
};