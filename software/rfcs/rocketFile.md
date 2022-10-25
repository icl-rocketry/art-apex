# RocketFile

*Aside: These definitely need cooler names, like what even is RocketFile?*

RocketFile is our custom file format for storing recorded data on Apex systems. It is just a binary format.

## Pre-reading

1. Know what [StructFS](../docs/structFS.md) is.
2. The RocketCtl RFC (RFC #3002)

## Why

With RocketCtl we can choose which sensor readings we want to record for a particular flight. RocketFile will keep track of them so that we don't have to.

If we tracked this information ourselves, it'd be a lot of hassle + the chance that we have some human error goes waaay up.

## Main Proposal

I'm going to leave this one vague, because I don't want to design everything for this myself, and it should be a good way for the freshers to get some input.

The general jist of what I'm thinking is that we split our file into 2 sections: header and data. The header section would keep track of any metadata like how big the file is, the column names, and the data types for the columns (int32, uint32, float64 etc). And the data section would be a bunch of rows containing, each one containing the data from the buffer. 

Note: the rows would all have the same fixed size, because we want to read from all sensors whenever possible.

When reading the file, RocketCtl would first read the header, and then use that information to read and parse the rest of the file.

## Design Criterion

*Somehow there's only 1*

The header format should be easy to parse, over being space efficient, since I don't think it'd be that big compared to the size of the data we actually record.

## Potential issues

A case *could* be made that we want variable sized rows, for example if some sensors don't update as frequently as others. I'm not sure if this is the case or not, but feel free to investigate.

The name "RocketFile" is kinda bad, cooler sounding name would be great. Bonus points if it's obscure and doesn't link to the fact that this is a file format at all.

## Extensions

Stream compression perhaps? Basically finding a way to compress our data so that we can either record data for longer, or take more readings per second. Although this is probably super extra.