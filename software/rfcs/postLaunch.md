# PostLaunch

In a perfect world we'd be able to plug our avionics into a laptop and boom we've got all the data off the board, and all the graphs have been drawn.

Let's get closer to a perfect world

## Pre-reading

1. The [RocketCtl RFC](https://github.com/icl-rocketry/art-apex/pull/7)

2. The [structFS docs](../docs/structFS.md)

3. Knowing that the flight data is stored in a binary format for space efficiency reasons.

4. Roughly how the Apex State Machine looks like. You can find this by reading the Apex Python code, or by pestering me into writing a doc to explain it.

## Why

Waiting a while to get data/graphs after recovering a rocket is mega cringe.

## Main Proposal

Right now we find a rocket, then wait a bit until I (Akshat) can plug it into my laptop. Then I copy over the binary data stored on the board and convert it into a csv. Shrey/Rohit would then write/run some other code to turn the csv into pretty graphs, that we'd see eventually.

This takes a while and really shouldn't. We know what the format is gonna be before we launch, and it can only change so much anyways, so really we should just have 1 program that would extract, convert and graph the flight data.

### General details

Since we're using Arduino now, we can't just drag and drop the file to a laptop anymore, and are gonna need to write some code to transfer it. I think we should use RPCs to get the data off the board, since 

1. We're already using them for RocketCtl
2. They're an efficient way to request data off the board - you'd just send an RPC to the board for some data, and it'd give it to you.

Once we've got the data off the board, parsing shouldn't be too complicated, since it's just converting binary to text. Making graphs also isn't too bad.

### Where should this code live?

I think the code for this should be part of RocketCtl, because then we can share the RPC client code, and it also just makes sense because RocketCtl should be able to fully control the rocket.

We'd also need some file serving code on the avionics that would interface with our RPCs to transfer the file. 

Now there are really only 2 places that this code can go - the first state or the last state of the state machine.

#### The first place

Putting the code in the last state makes sense because "duh that's when all the data has been recorded and it's the last thing you do in a flight". But sadly no, this won't work for an extremely subtle reason.

Brace yourself, this next part is gonna get intricate (you can also skip it and blindly believe me).

So the way the RPC library works, is that it waits for exactly 1 RPC every time you call the function `interface()`. Just bear this in mind.

Currently the last state is the recovery state, where we make sounds every couple seconds to let people find the rocket. So if we put the file server code here, we wouldn't be able to make sounds until we've received an RPC. This is bad.

"Ok so we can't put the file server in the recovery state, so why don't we just make a new state after it?". Well there are 2 reasons why this wouldn't work.

1. How would we go from the recovery state to the file recovery state? We'd either need a timer or a button. The timer idea assumes we find the rocket before the timer runs out. If not then we won't ever find the rocket. Even if we do find it, say super early, then we'd need to wait on the timer to run out, which is cringe.

2. What happens if the battery runs out before we get to this state? We lose all the data because we'd overwrite it.

So really we can't put the file server in the last state.

#### The second place

We put the file server in the first state, where all the other RPCs are, so we can either just file read, or start the launch process. One thing to bear in mind is that it might be very easy to accidentally erase flight data if we send the wrong RPC. So we'd need a security mechanism to stop that.

I'd say we have a password that someone would have to type to erase the flight data, if it exists.

#### The third place???

Yeah so I lied, there's a super secret 3rd place, which is the 2nd core of the rp2040 chip we're using. If we have a parallel file server, then we don't run into the problems of the first place. I'd rather not do this because it's more complicated than it needs to be, because accessing stuff in parallel can be weird.


## Potential Issues

One potential issue we might have is when we want to disable some reports (like I said in the RocketCtl RFC). Say we don't want to record temperature for a flight, then it doesn't make sense for our code to generate a temperature graph. I'm going to cover this issue in the next RFC, but the jist of it is that we'd store what's actually been recorded in the flight data as well.

## Extensions

There are loads of ways this can be extended, mostly waht we do with the data once we have it.

1. Use Kalman filtering to actually reconstruct a flight trajectory.

2. The cool VR animation thing that the person offered to do.

3. Automatic Instagram post if we break the record?

4. Automatically emailing the data to everyone.

(Feel free to add more suggestions to this)