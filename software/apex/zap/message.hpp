#pragma once

template <typename T>
class Message {
public:
    Message() {};
    Message(uint8_t id, T payload): id(id), payload(payload), len(sizeof T) {};

    uint8_t id; // We only support 256 message IDs
    size_t len;
    T payload;
};