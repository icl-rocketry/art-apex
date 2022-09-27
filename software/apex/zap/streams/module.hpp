#pragma once

#include "serialStream.hpp"
#include "serialStream.cpp"

// Virtual functions mean that noone can directly create a stream
// they have to use a _type_ of stream e.g. SerialStream
class Stream_ {
public:
    virtual size_t get_max_bytes();
    virtual bool recv(uint8_t* dest, size_t len);
    virtual bool send(const uint8_t* src, size_t len);
};