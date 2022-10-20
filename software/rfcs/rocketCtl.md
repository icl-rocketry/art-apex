# RocketCtl

RocketCtl or RocketControl is our system for configuring Apex systems before launch. It can control much of the setup of the avionics without needing us to recompile the code. 

## Pre-reading

1. Knowing that UART = Serial comminication = sending data on a wire to the avionics.

2. What an RPC is.

3. [Checksums](https://www.geeksforgeeks.org/error-detection-code-checksum/)

## Why

We want to get the Apex flight recorder certified as an official altimeter by UKRA, so we don't need to use 3rd party altimeters. This means that the firmware (including any on board config files) mustn't change between flights. But what if we want to change some minor details?

## Main Proposal

Ok so we can't change anything on the board between flights now, so what if the config wasn't stored on the board? What I'm proposing is that we store the config in a file somewhere on Github, and before a flight we can tell Apex that it needs to be configured in a specific way via serial communication.

Now you might be wondering "What on this flat Earth do we need to configure on Apex??", and that's a fair question, so here's a list of stuff that we could want to change:

1. The pre-flight delay time - we seem to want to change this quite a bit

2. The recording frequency - we might realise after a flight that it needs to be higher or lower

3. The type of sensor reports taken - i.e. do we really need to know the quaternion of rotation or not?

4. The total recording time

So now that we've established that we could want to change stuff between flights, how do we do this with technology??

Well we'd need code both on Apex to act as a receiver and a transmitter program that someone can run on their laptop or something.

So before a launch what you'd do is:

1. Change whatever config you need to.
2. Plug Apex into the controller (your laptop).
3. Run a program to configure Apex.
4. Unplug Apex.

Let's use tech words now - Apex is now the server, your laptop is now the client. 

The client would first read and parse the config file, to figure out what it needs to send to the server. It would then send the values to the server, which would reply with an acknowledgement of some sorts that it's received the configuration exactly how you sent it. Then the server would use the configuration to configure everything, and it should be smooth sailing from there.

### The config file

Probably yaml, that looks nice

### The client code

I'm thinking we use [SimpleRPC](https://pypi.org/project/arduino-simple-rpc/) with [Python](https://www.youtube.com/watch?v=dQw4w9WgXcQ) to send RPCs to the server. (You should remember RPCs from that pre-reading you did)

### The server code

Ok so this will be the more complicated bit. We already have a [SimpleRPC server](https://www.arduino.cc/reference/en/libraries/simplerpc/) for Arduino, but where do we put the server code? I say we put in it's own state at the start of the state machine, just so we configure everything before running anything. Since it's a state machine state, we should know that the rocket is ready once the led changes colour, i.e. the state has finished because everything went peachy.

But wait, what if some of the config gets corrupted during transmission, what do we do then?

What we can do is get the client to calculate a checksum of the config it needs to send, and also include it in the config message. The server would then also calculate the checksum of the config, and compare it to the received checksum, if they don't match, it'll ask the client to resend the config. This'll happen until the checksums match on the server's end.

## Dodgy design decisions

I've probably been waaay too gung-ho about the checksum bit, most likely we won't ever actually need it, because we barely send any data and serial communications are reliable.

## Extensions

This can't be extended because we can't change the firmware once it's been written. So we better get it right first time.