#pragma once
#include "lora.h"
#include <stdint.h>

// All known data about the rocket
// Feel free to add more things
struct RocketData {
    float longitude, latitude, altitude;
};

template <typename MSG>
class Device {
private:
    RocketData rocket;
    const LoRadio<MSG>& lora;

public:
    Device(RocketData data, const LoRadio<MSG>& lora) : rocket(data), lora(lora) {}
    Device(const LoRadio<MSG>& lora) : Device<MSG>(RocketData{0, 0, 0}, lora) {}

    RocketData get_rocket_data() {
        return rocket;
    }


    //Every time the device "ticks", it'll:
    //1. Receive some messages
    //2. Do stuff
    //3. Broadcast some messages
    //4. Update its estimate of the rocket's position
    void tick(uint64_t time) {
        // TODO: Write this
    }
};