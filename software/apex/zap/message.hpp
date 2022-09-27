#pragma once

#include "../util/uint.hpp"

class MessageHeader {
public:
    MessageHeader(uint32_t length, uint32_t msg_id): length(length), msg_id(msg_id) {}

    uint32_t length;
    uint32_t msg_id;
    uint32_t seq_num;
};

template <typename T>
class Message {
public:
    Message(MessageHeader header, T payload): header(header),
                                              payload(payload) {}
    Message(uint32_t msg_id, T payload): header(sizeof(T), msg_id),
                                         payload(payload) {}

    MessageHeader header;
    T        payload;
};

#define ROLE_MESSAGE_ID 0
#define CONT_MESSAGE_ID 1

enum Role {
    WRITER,
    READER,
    UNDEFINED
};

// Role message is sent by the writer to inform the reader of its (the writer's) new role
class RoleMessage: public Message<Role> {
public:
    RoleMessage(): RoleMessage(Role::UNDEFINED) {};
    RoleMessage(Role role): Message<Role>(ROLE_MESSAGE_ID, role) {}
};

// Continue message is sent by the reader to let the writer know that it's ready to receive max_bytes worth of data
class ContinueMessage: public Message<size_t> {
public:
    ContinueMessage(): ContinueMessage(0) {}
    ContinueMessage(size_t max_bytes): Message<size_t>(CONT_MESSAGE_ID, max_bytes) {}
};

class GenericMessage: public Message<void*> {
public:
    GenericMessage(MessageHeader header, const void* src): Message<void*>(header.msg_id, nullptr) {
        payload = new uint8_t[header.length];
        memcpy(payload, src, header.length);
    }

    template <typename T>
    void to(Message<T>* other) {
        other->header = header;
        memcpy((void*)other->payload, payload, sizeof(T));
    }

    ~GenericMessage() {
        delete payload;
    }
};