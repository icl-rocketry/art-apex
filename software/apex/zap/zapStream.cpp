#include "zapStream.hpp"

ZapStream::ZapStream(Stream_& stream): stream(stream),
                                       seq_num(0),
                                       role(Role::UNDEFINED) {
}

ZapStream::~ZapStream() {
    while (!messages.is_empty()) {
        Message<void*>* msg = messages.pop();
        delete msg;
    }
}

// Try to become the role <role>
// This only works if you go from undefined -> writer
//                                 writer   -> reader
// TODO: Add error codes?
bool ZapStream::assume_role(Role new_role) {
    bool can_assume_role = (role == Role::UNDEFINED && new_role == Role::WRITER) ||
                           (role == Role::WRITER    && new_role == Role::READER);
    if (!can_assume_role) {
        return false;
    }

    RoleMessage msg;

    // If we're in the undefined role, then we can't guarantee that the 
    // other person has heard us, so we keep sending it until we get an 
    // acknowledgement
    while (true) {
        if (!send_msg_unchecked(msg)) {
            return false;
        }
        if (wait_for_reader(100)) {
            return true;
        }
    }
}

template <typename T>
bool ZapStream::send_msg(Message<T> msg, unsigned long timeout) {
    // We can't send anything until we know that the reader is ready to receive it
    if (write_limit < sizeof(Message<T>)) {
        if (!wait_for_reader(timeout)) {
            return false
        }
    }

    if (!send_msg_unchecked(msg)) {
        return false;
    }

    write_limit -= sizeof(Message<T>);
    return true;
}

template <typename T>
bool ZapStream::read_msg(uint32_t expected_msg_id, Message<T>* msg) {
    if (!messages.is_empty()) {
        // Try to get a message from the message queue
        if (get_msg_from_messages(expected_msg_id, msg)) {
            return true;
        }
    }

    // Get all the data from the underlying stream
    size_t buffer_size = stream.get_max_bytes();
    uint8_t* buf = new uint8_t[buffer_size]; // Temporary buffer
    size_t bytes_read = stream.recv(buffer, buffer_size);
    if (bytes_read == 0) { // TODO: maybe check if we can parse a single header
        return false;
    }

    // Parse the data into messages and add them to the queue
    uint8_t* ptr = buf;
    while (ptr < buf + bytes_read) {
        MessageHeader header;
        deserialise_header(&header, ptr);
        
        //If the message wasn't fully sent over, drop it
        if (ptr + header.length <= bytes_read) {            
            GenericMessage* msg = new GenericMessage(header, ptr);
            messages.append(msg);
        }

        ptr += header.length;
    }
    delete buf;

    // Try getting the message from the queue again
    return get_msg_from_messages(expected_msg_id, msg);
}

template <typename T>
bool ZapStream::get_msg_from_messages(uint32_t expected_msg_id, Message<T>* msg) {
    GenericMessage* found_msg = messages.remove([](GenericMessage msg) -> bool {
        return msg.msg_id == expected_msg_id;
    });

    // Copy the fields over
    if (found_msg != nullptr) {
        found_msg->to(msg);

        delete found_msg;
        return true;
    }
    return false;
}

bool ZapStream::wait_for_reader(unsigned long timeout) {
    ContinueMessage cont_msg;
    unsigned long deadline = millis() + timeout;

    while (millis() < deadline) {
        if (!stream.recv((uint8_t*) &cont_msg, sizeof(cont_msg))) {
            continue;
        }
        if (cont_msg.header.msg_id != CONT_MESSAGE_ID) {
            continue;
        }
        reader_buffer_size = cont_msg.payload;
        write_limit = reader_buffer_size;
        return true;
    }
    return false;
}

template <typename T>
bool ZapStream::send_msg_unchecked(Message<T> msg) {
    msg.seq_num = seq_num;
    seq_num++;
    return stream.send((const uint8_t*) &msg, sizeof(Message<T>));
}

void deserialise_header(MessageHeader* header, uint8_t* ptr) {
    memcpy((void*) &header, (uint8_t*) ptr, sizeof(MessageHeader));
    ptr += sizeof(MessageHeader);
}