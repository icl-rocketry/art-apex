#include "zapStream.hpp"
#include "message.hpp"

template <typename T>
bool ZapStream::send(uint8_t msg_id, T msg) {
    const Message<T> msg(msg_id, msg);
    return stream.send((const uint8_t*) &msg, msg.len);
}

template <typename T>
bool ZapStream::recv(uint8_t expected_msg_id, T* msg) {
    const size_t size = sizeof(Message<T>)
    uint8_t raw[size];
    if (!stream.recv((uint8_t*) raw, size)) {
        return false;
    }
    if (raw[0] != expected_msg_id) {
        return false;
    }
    Message<T> recvd = (Message<T>)raw;
    *msg = recvd.payload;
    return true;
}