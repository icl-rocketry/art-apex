#pragma once
#include "lora.h"

// All known data about the rocket
// Feel free to add more things
struct RocketData {
    float longitude, latitude, altitude;
};

template <typename MSG>
class Device {
private:
    RocketData rocket;
    LoRadio lora;    

public:
    Device(LoRadio lora) : rocket(RocketData{0, 0, 0}), lora(lora) {}

    RocketData get_rocket_data() {
        return rocket;
    }

    void tick();
};