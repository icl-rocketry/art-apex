#include <iostream>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <stdint.h>
#include <fstream>
#include <memory>
#include <string>
#include <cstdlib>

#include "../include/nlohmann/json.hpp"

#include "../modules/lora.h"
#include "../modules/device.h"
#include "util.h"
#include "config.h"

using namespace std; // So we don't need to keep typing std
using json = nlohmann::json;


template <typename MSG>
struct SimDevice {
    queue<MSG> broadcast_q;
    queue<MSG> receive_q;
    Device<MSG> device;
    
    float x, y, z;
    float tx_range; //Assuming we transmit in a perfect sphere

    SimDevice(DeviceConfig cfg) : x(cfg.x), y(cfg.y), z(cfg.z),
                                  tx_range(cfg.tx_range),
                                  device(Device<MSG>(std::make_unique<SimLoRa<MSG>>(receive_q, broadcast_q))) {}

    float distance_to(SimDevice<MSG>* other) {
        return sqrt((x - other->x) * (x - other->x) + 
                    (y - other->y) * (y - other->y) + 
                    (z - other->z) * (z - other->z));
    }
};

template <typename MSG>
class Simulation {
public:
    Simulation(Config cfg, MSG rocket_msg) : time(0), rocket_msg(rocket_msg) {
        rocket_msg_ticks = cfg.rocket_msg_ticks;
        p_failure = cfg.p_failure;
        p_corruption = cfg.p_corruption;
        
        // Add the rocket first (technically a mem leak, but it'll live as long as the program so it's fine)
        SimDevice<MSG>* rocket = new SimDevice<MSG>(cfg.rocket);

        int id = 0;

        devices.insert({id, rocket});

        id++;
        // Add the other devices
        for (auto const& device_cfg : cfg.devices) {
            SimDevice<MSG>* device = new SimDevice<MSG>(device_cfg);
            devices.insert({id, device});
            id++;
        }
    }

    void tick() {
        // Every 100 ticks, the rocket will broadcast its position
        if (time % rocket_msg_ticks == 0) {
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

    uint64_t rocket_msg_ticks;
    float p_failure, p_corruption;

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
        for (auto msg : this_tick_msgs) {
            std::string delivery_type = "DELIVERED";

            MSG msg_copy = msg.msg;
            
            if (msgs_per_device[msg.recepient] != 1) {
                collide(&msg_copy);
                delivery_type = "CONFLICT";
            }

            if (corrupt(&msg_copy, p_corruption) > 0) {
                delivery_type = "CORRUPTED";
            }

            if (rand_uniform() >= p_failure) {
                devices.at(msg.recepient)->receive_q.push(msg_copy);
            } else {
                delivery_type = "FAILED";
            }
            std::cout << delivery_type << "\t";
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

            std::cout << "BROADCAST\t";;
            log(time, id, msg);
            device->broadcast_q.pop();
        }
    }
};

/* List of stupid things
    1. Ticks aren't tied to any real life timing model
    2. Conflicts aren't corrupted realistically I think
*/
int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Expected 2 arguments - config file location and simulation duration" << endl;;
        return 1;
    }

    srand(0); // Seed the randomness so results are reproducible

    std::string config_path = argv[1];
    uint64_t ticks = stoi(argv[2]);

    ifstream f(argv[1]);
    json data = json::parse(f);
    Config cfg;
    from_json(data, cfg);

    Simulation<int> sim(cfg, 5);

    for (uint64_t i = 0; i < ticks; i++) {
        sim.tick();
    }
    return 0;
}