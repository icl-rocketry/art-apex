#pragma once

#include "../util/uint.hpp"

template <typename T>
class Message {
public:
    Message(uint32_t msg_id, T payload): len(sizeof T),
                                         msg_id(msg_id),
                                         payload(payload) {};

    uint32_t length;
    uint32_t msg_id;
    uint32_t seq_num;
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