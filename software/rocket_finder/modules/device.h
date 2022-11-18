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
    const LoRadio<MSG>& lora;

public:
    Device(RocketData data, const LoRadio<MSG>& lora) : rocket(data), lora(lora) {}
    Device(const LoRadio<MSG>& lora) : Device<MSG>(RocketData{0, 0, 0}, lora) {}

    RocketData get_rocket_data() {
        return rocket;
    }

    void tick(uint64_t time);
};