# RocketFinder

## Simulator
Compile the simulator using `make` in this folder.
The configuration for the simulator can be found in config.json.

The simulation has 1 rocket, and n devices. Each rocket/device has a position and a transmission range, used to decide who can receive broadcasts from who.

Every `rocket_msg_ticks` the rocket will broadcast its message. As of this commit, the rocket's message is an integer, but that can be changed to a custom packet type by:
1. Replacing `Simulation<int> sim(cfg, 5);` in `main.cpp` with `Simulation<YOUR_TYPE> sim(cfg, A_PACKET_OF_YOUR_TYPE)`
2. Creating a new `to_string` function in `util.h` like so:
`string to_string(YOUR_TYPE msg) {...}`

As of now, every device will just rebroadcast every message it has received, but you want to change that. You can do this by changing the `tick` member function in `device.h`.

Note: there is a `p_failure` chance a device won't receive a message, and a `p_corruption` chance that the device will receive a corrupted message.

Messages take some time, proportional to the distance between the sender and receiver, to be received, so it's perfectly reasonable that 2 devices receive the same message at different times. Note if 2 or more messages arrive at a device at the same time, then a single corrupted message will be received.

### Example trace
`BROADCAST  {"time": 0, "sender": 0, "msg": 5}`
`BROADCAST  {"time": 0, "sender": 1, "msg": 6}`
`DELIVERED  {"time": 1, "recepient": 3, "msg": 5}`
`CONFLICT   {"time": 1, "recepient": 2, "msg": 5}`
`CONFLICT   {"time": 1, "recepient": 2, "msg": 6}`
`CORRUPTED  {"time": 1, "recepient": 4, "msg": 6}`
`FAILED     {"time": 1, "recepient": 5, "msg": 5}`

In this (nonsensical) example devices 0 and 1 (the rocket and another) broadcast messages at time 0. Both messages are delivered to a device equidistant from them (2) causing a conflict. Whereas the other devices either receive the message or it fails.