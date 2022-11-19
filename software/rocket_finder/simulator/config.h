#pragma once

#include <vector>

#include "../include/nlohmann/json.hpp"

using json = nlohmann::json;

struct DeviceConfig {
    float x, y, z;
    float tx_range;
};

struct Config {

    std::vector<DeviceConfig> devices;
    DeviceConfig rocket;

    int rocket_msg_ticks;
    float p_failure;
    float p_corruption;
};

void from_json(const json& j, DeviceConfig& cfg) {
    j.at("x").get_to(cfg.x);
    j.at("y").get_to(cfg.y);
    j.at("z").get_to(cfg.z);
    j.at("transmission_range").get_to(cfg.tx_range);
}

void from_json(const json& j, Config& cfg) {
    j.at("devices").get_to(cfg.devices);
    j.at("rocket").get_to(cfg.rocket);
    j.at("rocket_msg_ticks").get_to(cfg.rocket_msg_ticks);
    j.at("p_failure").get_to(cfg.p_failure);
    j.at("p_corruption").get_to(cfg.p_corruption);
}
