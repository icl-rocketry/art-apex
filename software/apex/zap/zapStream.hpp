#pragma once

#include "../util/uint.hpp"
#include "../util/buffer.hpp"
#include "../util/linked_list.hpp"
#include "streams/module.hpp"
#include "message.hpp"

class ZapStream {
public:
    ZapStream(Stream_& stream);
    ~ZapStream();

    bool assume_role(Role new_role);

    template <typename T>
    bool send_msg(Message<T> msg, unsigned long timeout);

    template <typename T>
    bool read_msg(uint32_t expected_msg_id, Message<T>* msg);

    // TODO: Maybe add something to read the first message in the buffer regardless of its contents?

private:
    Stream_& stream;
    uint32_t seq_num; // Number to keep track of how many messages were sent, doesn't do anything
    Role role;

    // Reader specific members
    LinkedList<GenericMessage> messages; // All the messages
    
    // If I'm a reader and need to become a writer, only do that once all messages have been read
    bool role_switch_pending;

    // Writer specific members
    size_t reader_buffer_size;
    size_t write_limit; // The number of bytes we can send so that the sender can receive the data


    template <typename T>
    bool send_msg_unchecked(Message<T> msg);

    // Wait for the reader to send a continue message, or for the timeout to end
    // The timeout is in milliseconds.
    // Returns true if the message was received
    bool wait_for_reader(unsigned long timeout);

    template <typename T>
    bool get_msg_from_messages(uint32_t expected_msg_id, Message<T>* msg);
};