#pragma once

#include "../util/uint.hpp"
#include "../util/buffer.hpp"
#include "../util/linked_list.hpp"
#include "streams/module.hpp"
#include "message.hpp"

class ZapStream {
public:
    ZapStream(Stream_& stream);

    bool assume_role(Role role);

    template <typename T>
    bool send_msg(Message<T> msg);

    template <typename T>
    bool read_msg(uint32_t expected_msg_id, Message<T> msg);

    // TODO: Maybe add something to read the first message in the buffer regardless of its contents?

private:
    Stream_& stream;
    uint32_t seq_num;
    Role role;

    // Reader specific members
    Buffer* buffer; // Contains all the unprocessed data received from the stream
    LinkedList<Message<void*>> messages; // All the messages
};