#pragma once

#define uint32_t unsigned int // Just getting the vscode extension to shut up

// https://stackoverflow.com/a/1766566
uint32_t round_up_power_of_2(uint32_t val, uint32_t to) {
    const uint32_t power_minus_1 = to - 1;
    return val + power_minus_1 & ~power_minus_1;
}