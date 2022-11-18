#include "device.h"

//Every time the device "ticks", it'll:
//1. Receive some messages
//2. Do stuff
//3. Broadcast some messages
//4. Update its estimate of the rocket's position
template <typename MSG>
void Device<MSG>::tick(uint64_t time) {
    // TODO: Write this
}