#pragma once
#include <queue>

// Use this to do anything broadcast/receive related
template <typename MSG>
class LoRadio {
public:
    // Broadcast a message of type MSG
    virtual bool broadcast(const MSG& msg) const = 0;

    // Receive into a message of type MSG
    virtual bool receive(MSG& msg) const = 0;
};

template <typename MSG>
class SimLoRa: public LoRadio<MSG> {
private:
    // Stuff used by the simulation
    std::queue<MSG>& receive_queue;
    std::queue<MSG>& broadcast_queue;

public:
    SimLoRa(std::queue<MSG>& r_q, std::queue<MSG>& b_q) : receive_queue(r_q), broadcast_queue(b_q) {}

    // This will send the message to the underlying simulation
    // It will always succeed
    bool broadcast(const MSG& msg) const override {
        broadcast_queue.push(msg);
        return true;
    }
    
    // This will receive a message from the simulation
    // But it will fail if the queue is empty
    // It may also succeed but give you a corrupted message
    bool receive(MSG& msg) const override {
        if (receive_queue.empty()) {
            return false;
        }
        msg = receive_queue.front();
        receive_queue.pop();
        return true;
    }
};