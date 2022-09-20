# StructFS

StructFS is our simple filesystem for recording data on Apex systems. It stores all our data on the flash memory of the rp2040 board that we're using.

## Pre-reading

Flash on rp2040 boards is waaay more complicated than it needs to be, so I'd recommend skimming the [docs](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf) first, just to see what kinda stuff you can do.

Since the docs don't really give you too much useful/practical information look at this [2nd link](https://medium.com/@akshat.tripathi6568/beginners-guide-to-flash-programming-on-the-rp2040-1a2460923d9) (Bonus: it's also a shameless self plug). If this doesn't make too much sense either, check out the links at the bottom or look at [this repo](https://gitlab.com/mcturra2000/rpi/-/tree/master/pico/flash). Note the `flash_range_erase` code here uses `FLASH_PAGE_SIZE` which is wrong and should be `FLASH_SECTOR_SIZE`.


## Why

We need a fast way to store flight data on the board.

## How to use

1. Create a `FS` object with a size.
2. Create however many files you want, with their own sizes and buffer sizers.
3. Add the files to the filesystem object via `fs.AddFile(file)`.
4. To write to it, use `file.append(value)`
5. To read from it, use `file.read(offset, dest, len)` which reads `len` bytes from the file at position `offset` into `dest`.

## General advice
- Make sure that the filesystem isn't larger than the amount of flash you have available.
- When adding files to the filesystem, if the function returns `false` then you've run out of space in the filesystem.
- The filesystem rounds the filesize up to the nearest 4kB, so you could run out of space even though the filesizes that you enter add up properly.
- Reading or writing to a file will return a boolean, which will be `false` if the operation failed, make sure to check for this.
- Remember to flush the file once you're done writing
- Sometimes compilers can add [padding](https://www.javatpoint.com/structure-padding-in-c#:~:text=Structure%20padding%20is%20a%20concept,align%20the%20data%20in%20memory.) to struct fields which will make them bigger than what they seem, make sure to consider this so you don't waste any space.


## How it works

StructFS has 2 classes: `FS` and `File`. 

All `FS` does is let you add files to it and erase them, and once you do so, it'll prep the file, by telling it where its first address and last address are. It can erase files to make them actually writable using `MakeWritable` (you can't write to flash unless it's been erased).

`File` is an class that you can append to, or read from. I've chosen to only let you append to the file, since we don't ever need to edit flight data. However you can read from any point in the file. Since we have the `template <typename T>` above `append`, you can write any type to the file.

### File append
Writing to flash memory is much slower than writing to RAM, so we want to make sure we write as much data to RAM as we can before storing it in flash. This is why the `File` class has the `buffer_start`, `cursor` and `buffer_size` fields.

When we create a file, we use `buffer_size` bytes somewhere in RAM to store a bunch of data and the address of the buffer is stored at `buffer_start`. The `cursor` is what we use to track the "free" data in the buffer, that we can overwrite. 

Suppose we want to add 32 bytes to the buffer, what we'd do is copy those bytes into the buffer at the position of the `cursor`, and then we add 32 to the `cursor`, so it represents our free space. If the buffer is completely full, we `flush` it to flash which is where the data is actually stored, and then reset the cursor to the start of the buffer. 

If we called `flush` when the buffer wasn't full, then we assume that the rest of the data in the buffer should be 0, to avoid keeping any junk data, which is why we have the `memset` in `flush`. At the end of a `flush`, we move the `curr` field forward, so that we don't overwrite the same data in flash.

#### File read
Reading a file is much easier than writing to it. All we need to do is pick where in the file we're reading it from (`offset`), how much data to read (`len`) and where to read it to (`dest`). What'll then happen, is that we'll copy the data from flash, into the destination and call it a day.


## Benchmarks

Check out [this file](../benchmarks/filesys.ino).

Writing 4kb of data to flash takes roughly 25ms.

## Questionable design decisions

While I was writing this doc up, I realised that there were a couple of highly questionable decisions made here. These should be quite simple to fix, so I'm leaving them in here as mini exercises for people to get some experience working with the filesystem.

These are only questionable because I *think* they are, so it's entirely possible that I'm completely wrong about them. If so, I'll be happy so make sure to let me know if you disagree.

1. We erase the space for individual files in `AddFile`, but since we're probably gonna use the entire filesystem, I think it makes more sense to move that code to the filesytem initialisation code. This would also mean that filesizes aren't rounded up to the nearest 4kB but the nearest 256B page. *Edit This was actually a bug, because always erasing the flash meant that you'd not be able to recover data once the device was restarted, since we'd always overwrite the data - fixed in commit 5a3632e3302aae68f067c0af39c78b6a331d7c5d*

2. I think calling `flush` when the buffer is empty still performs a wasteful flash write, so we should get rid of it, since it could cause failures when everything is actually fine, i.e. you've filled up the entire file, and when you flush onces more, it'll zero out part of the next file's storage.

3. File read doesn't stop you from reading other files, since there's no check to make sure that the offset and length mean that you'll only read data from the current file.

4. It's possible to overwrite the data of another file by calling flush on its own several times. I don't think anyone would do that on purpose, but bugs are never on purpose. We need to check that the file isn't full when we flush data.

5. Generally there are places where we don't have checks on the sizes of values and are assuming that people know what they're doing and won't ever make mistakes, so if someone accidentally passes in a dodgy value, the code will behave in unexpected ways.

## Extensions

To make structFS faster, you can use the 2nd core of the rp2040 to perform a flash write. The architecture for this is more complicated than what we have right now. To do this we'll need 2 buffers instead of 1. We'll also have something running on the 2nd core that's constantly waiting for the main buffer to be filled. Once the buffer is filled, we'll immediately swap it with a spare buffer, and have core 2 write the contents of the spare buffer to flash. This way core 1 will always see a buffer that it can write to, and so won't have to ever wait for a flash write. I doubt that we'll have to take this route ever since our speed should be fast enough, but if we do, this is what will do it. If you do this, make sure to use a mutex/lock/semaphore to prevent a [data race](https://en.wikipedia.org/wiki/Race_condition) on the buffers.