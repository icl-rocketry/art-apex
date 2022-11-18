#include <iostream>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <stdint.h>

#include "modules/lora.h"
#include "modules/device.h"

using namespace std; // So we don't need to keep typing std

template <typename MSG>
struct SimDevice {
    queue<MSG> broadcast_q;
    queue<MSG> receive_q;
    Device<MSG> device;
    
    float x, y, z;
    float tx_range; //Assuming we transmit in a perfect sphere

    SimDevice(float tx_range) : tx_range(tx_range), 
                                device(Device<MSG>(SimLoRa<MSG>(receive_q, broadcast_q))) {
        // TODO: Randomise
        x = 0.5;
        y = 0.1;
        z = 0.42;
    }
    
    SimDevice(float tx_range, RocketData rocket_data) : tx_range(tx_range),
                                                        device(Device<MSG>(rocket_data, SimLoRa<MSG>(receive_q, broadcast_q))) {
        // TODO: Randomise
        x = 0.5;
        y = 0.1;
        z = 0.42;
    }

    float distance_to(SimDevice<MSG>& other) {
        return sqrt((x - other.x) * (x - other.x) + 
                    (y - other.y) * (y - other.y) + 
                    (z - other.z) * (z - other.z));
    }
};

// Wrap the message with a time, so we can work out a good delivery schedule
// We have a recepient for each message, because a message can be delivered to different people at the same time
template <typename MSG>
struct TimedMessage {
    uint64_t time;
    MSG msg;
    uint8_t recepient;
};


// This is just needed to compare 2 different timed messages
template <typename MSG>
struct compare {
    bool operator()(const TimedMessage<MSG>& a, const TimedMessage<MSG>& b) {
        return a.time > b.time;
    }
};


template <typename MSG>
class Simulation {
public:
    Simulation(int n_devices) : time(0) {
        // Add the rocket first
        RocketData rocket_data{4, 65, 23}; // TODO: randomise this
        SimDevice<MSG> rocket(3, rocket_data);

        devices.insert({0, rocket});

        // Add the other devices
        for (int i = 1; i <= n_devices; i++) {
            SimDevice<MSG> device(i);
            devices.insert({i, device});
        }
    }

    void tick() {
        // Deliver every message, then tick every device and collect all messages
        deliver_messages();

        for (auto const& device_pair : devices) {
            auto device = device_pair.second;
            auto id = device_pair.first;
            device.device.tick(time);
        
            schedule_broadcast(id, device);
        }

        time++;
    }

private:
    uint64_t time;
    unordered_map<uint8_t, SimDevice<MSG>> devices; // Maps ids to devices
    priority_queue<TimedMessage<MSG>, vector<TimedMessage<MSG>>, compare<MSG>> msgs; // Queue of all messages, soonest first

    void deliver_messages() {
        vector<TimedMessage<MSG>> this_tick_msgs;
        unordered_map<uint8_t, int> msgs_per_device;

        TimedMessage<MSG> msg = msgs.top();
        // Find all messages that would be delivered at this time
        while (msg.time == time) {
            this_tick_msgs.push_back(msg);
            msgs_per_device[msg.recepient]++;
            msgs.pop();
        }

        // Only send messages to devices which don't have conflicts
        // Conflicts occur when a device attempts to read 2 messages at once
        for (auto const& msg : this_tick_msgs) {
            if (msgs_per_device[msg.recepient] == 1) {
                devices.at(msg.recepient).receive_q.push(msg.msg);
            }
        }
    }

    void schedule_broadcast(uint64_t id, SimDevice<MSG> device) {
        // Collect each broadcast message
        while (!device.broadcast_q.empty()) {
            auto msg = device.broadcast_q.front();

            // Schedule a message for each other device, within tx_range
            for (auto const& device_pair2 : devices) {
                auto device2 = device_pair2.second;
                auto id2 = device_pair2.first;

                if (id == id2) {
                    continue;
                }

                float distance = ceil(device.distance_to(device2));
                if (distance < device.tx_range) {

                uint64_t propagation_delay = static_cast<uint64_t>(distance);
                msgs.push(TimedMessage<MSG>{time + propagation_delay, msg, id2});
                }
            }

            device.broadcast_q.pop();
        }
    }
};

int main() {
    const int n_devices = 5;
    const float width = 50;
    const float height = 50;
    const float depth = 50;
    const float tx_range = 10; // TODO config  

    Simulation<int> sim(n_devices);

    for (int i = 0; i < 100; i++) {
        sim.tick();
    }

    return 0;
}