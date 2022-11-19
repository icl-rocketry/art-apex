#include <iostream>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <stdint.h>

#include "../modules/lora.h"
#include "../modules/device.h"
#include "util.h"

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

    float distance_to(SimDevice<MSG>* other) {
        return sqrt((x - other->x) * (x - other->x) + 
                    (y - other->y) * (y - other->y) + 
                    (z - other->z) * (z - other->z));
    }
};

template <typename MSG>
class Simulation {
public:
    Simulation(int n_devices, MSG rocket_msg) : time(0), rocket_msg(rocket_msg) {
        // Add the rocket first
        SimDevice<MSG>* rocket = new SimDevice<MSG>(10); //TODO: magic number
        rocket->x = 0;

        devices.insert({0, rocket});

        // Add the other devices
        for (int i = 1; i <= n_devices; i++) {
            SimDevice<MSG>* device = new SimDevice<MSG>(i);
            devices.insert({i, device});
        }
    }

    void tick() {
        // Every 100 ticks, the rocket will broadcast its position
        // TODO: make this configurable
        if (time % 100 == 0) {
            auto rocket = devices.at(0);
            rocket->broadcast_q.push(rocket_msg);
        }


        // Deliver every message, then tick every device and collect all messages
        deliver_messages();

        for (auto const& device_pair : devices) {
            auto device = device_pair.second;
            auto id = device_pair.first;
            device->device.tick(time);
        
            schedule_broadcast(id, device);
        }

        time++;
    }

private:
    uint64_t time;
    MSG rocket_msg; // It's annoying that this needs to be here, but c'est la vie
    unordered_map<uint8_t, SimDevice<MSG>*> devices; // Maps ids to devices
    priority_queue<TimedMessage<MSG>, vector<TimedMessage<MSG>>, compare<MSG>> msgs; // Queue of all messages, soonest first

    void deliver_messages() {
        if (msgs.empty()) {
            return;
        }

        vector<TimedMessage<MSG>> this_tick_msgs;
        unordered_map<uint8_t, int> msgs_per_device;

        TimedMessage<MSG> msg = msgs.top();
        // Find all messages that would be delivered at this time
        while (msg.time == time && !msgs.empty()) {
            this_tick_msgs.push_back(msg);
            msgs_per_device[msg.recepient]++;
            msgs.pop();
            msg = msgs.top();
        }

        // Only send messages to devices which don't have conflicts
        // Conflicts occur when a device attempts to read 2 messages at once
        for (auto const& msg : this_tick_msgs) {
            if (msgs_per_device[msg.recepient] == 1) {
                devices.at(msg.recepient)->receive_q.push(msg.msg);
                std::cout << "DELIVERED ";
            } else {
                std::cout << "CONFLICT ";
            }
            log(msg);
        }
    }

    void schedule_broadcast(uint64_t id, SimDevice<MSG>* device) {
        // Collect each broadcast message
        while (!device->broadcast_q.empty()) {
            auto msg = device->broadcast_q.front();

            // Schedule a message for each other device, within tx_range
            for (auto const& device_pair2 : devices) {
                auto device2 = device_pair2.second;
                auto id2 = device_pair2.first;

                if (id == id2) {
                    continue;
                }

                float distance = ceil(device->distance_to(device2));
                if (distance < device->tx_range) {
                    uint64_t propagation_delay = static_cast<uint64_t>(distance);
                    msgs.push(TimedMessage<MSG>{time + propagation_delay, msg, id2});
                }
            }

            std::cout << "BROADCAST " << "{\"msg\": " << to_string(msg) << "}" << std::endl;
            device->broadcast_q.pop();
        }
    }
};

int main() {
    const int n_devices = 5;
    const float width = 50;
    const float height = 50;
    const float depth = 50;
    const float tx_range = 10; // TODO config  

    Simulation<int> sim(n_devices, 5);

    for (int i = 0; i < 200; i++) {
        sim.tick();
    }

    return 0;
}