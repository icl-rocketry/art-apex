# RocketFind

RocketFind is our system for finding rockets once they've landed.

## Pre-reading

We've tried several GPS based approaches to finding rockets in the past. 

In 2021, we had a custom avionics solution which would send us a text message with the GPS coordinates. Unfortunately we weren't able to launch APEX I then, and due to the temperamental nature of the avionics (mostly the SIM800L board), we decided to replace it with an off the shelf solution.

In 2022, we used a Vodafone Curve tracker to find the rocket. The Curve would allow us to access the rocket's GPS coordinates via an app. The Curve service has a small monthly fee. We encountered some issues with this system when we tried to find APEX I. 

1. We didn't receive any coordinates for 40ish minutes, most likely due to the patch network conditions at SARA. 

2. We received a 2nd set of coordinates that would place the rocket 12km away in a nearby town. We aren't exactly sure why this happened, but the leading theory is that the Curve attempted to predict it's location, by using past readings.

## Why

We need to find the rocket in a Scottish moor.

## Proposal

This is a faily open ended proposal, since I don't know what the best solution is, nor do I know if I've covered every possible solution to the problem. What I am going to do is discuss several different ideas that we could use to locate the rocket faster.

## General approach

All of the below proposals have the same underlying approach - have something that broadcasts a signal, and use the signal strength to allow people to locate the device. A key advantage of this approach is that it allows us to find the rocket regardless of the network conditions at the landing site, since we're only sending a signal and not sending data over a network. The tradeoff to not connecting to a network is that we'll have to be within a certain distance of the rocket to locate it, which would depend on the type of radio broadcast device we use.

Note: None of these should be integrated with the main avionics, so we can iterate on this, without changing the firmware of the main avionics.

### Approach 1 - Bluetooth

Here our transmitter would be a bluetooth device, these are fairly easy to find off the shelf, and also mean that we won't have to make any special hardware to find the device, since all *modern* mobile phones have bluetooth capabilities, and should be able to give us reports on the signal strength of the device. Normal bluetooth is said to (read: I Googled it) have a range of upto 100m outdoors, and we're probably not gonna land 100m away, what this means is that when searching, we'd need to split up with our phones, and search until someone gets a ping, then we'd converge on the location of whoever got the ping, and *should* be able to triangulate the position of the rocket using them.

I suspect that the actual range might be much higher than 100m since we're launching in the middle of nowhere and shouldn't have too much interference. We can probably get a good estimate of the range by testing in Richmond Park.

We'd also need to ensure that the battery of any bluetooth beacon lasts several hours, just in case we can't find it on the day and need to look for it the next day.

We might also have an issue when locating the device, since the connection is fairly spotty, so we might not be able to actually tell each other that we've got a ping. To rememdy this I suggest we have flares (no not really).

### Approach 2 - Bluetooth Low Energy

This is pretty much Approach 1, but with a longer battery life, and smaller transmission range of ~50m. Because of the shorter transmission range, I suggest we don't exclusively use BLE, but instead use it if we don't find the rocket within say 3 hours. If we end up doing that, we may not be able to use an off the shelf beacon, but instead have to make one ourselves.


### Approach 3 - Wifi

This is basically the ones above, but with yet another transmission range/power consumption trade off. Here the rocket would be a wifi hotspot that we'd need to find.


### Approach 4 - LoRa

LoRa has a much longer transmission range, than any of the above, and would we would probably be able to receive signal right at the launch site. Because of this, we ditch the spread out phone based finding method, and instead create a "compass" that would always point towards the rocket.

I've not heard good things about LoRa from Shrey and the main team, but since they have experience with it, we might be able to get help setting it up. 

### Approach 5 - 433MHz radio transmitters

Lastly I've seen these on the internet, and I think they'd be like the bluetooth/wifi beacons, but easier to program, more flexible and with a longer range. These also seem to be a standard in locating drones, so we could probably find exisitng designs quite easily online. Note there are some legal requirements that come with using these, they can't use more than a certain amount of power etc.

I think this approach is definitely worth exploring, since the transmitters/receivers are super cheap, and there's a ton of information online about them.

