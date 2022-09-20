#pragma once

#include "streams/module.hpp"

class ZapStream {
public:
    ZapStream(Stream_& stream): stream(stream) {};

    template <typename T>
    bool send(uint8_t msg_id, T msg);
    
    template <typename T>
    bool recv(uint8_t expected_msg_id, T* msg);

private:
    Stream_& stream;
};