#pragma once
#include <queue>
#include <vector>

// Use this to do anything broadcast/receive related
template <typename MSG>
class LoRadio {
public:
    // Broadcast a message of type MSG
    virtual bool broadcast(const int channel, const MSG& msg) const = 0;

    // Receive into a message of type MSG
    virtual bool receive(const int channel, MSG& msg) const = 0;
};

template <typename MSG>
class SimLoRa: public LoRadio<MSG> {
private:
    // Stuff used by the simulation
    std::vector<std::queue<MSG>>& receive_queues;
    std::vector<std::queue<MSG>>& broadcast_queues;

public:
    SimLoRa(std::vector<std::queue<MSG>>& r_qs, std::vector<std::queue<MSG>>& b_qs) : receive_queues(r_qs), broadcast_queues(b_qs) {}

    // This will send the message to the underlying simulation
    // It will always succeed
    bool broadcast(const int channel, const MSG& msg) const override {
        broadcast_queues[channel].push(msg);
        return true;
    }
    
    // This will receive a message from the simulation
    // But it will fail if the queue is empty
    // It may also succeed but give you a corrupted message
    bool receive(const int channel, MSG& msg) const override {
        auto& receive_queue = receive_queues[channel];
	if (receive_queue.empty()) {
            return false;
        }
        msg = receive_queue.front();
        receive_queue.pop();
        return true;
    }
};
