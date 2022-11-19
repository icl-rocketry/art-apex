# Use this to easily and efficiently compile the code
# It only updates things you've changed, so you don't need to spend years compiling

# Set up some variables
CC = g++
CC_FLAGS = -g # This lets you use debuggers like gdb

all: sim

# Compile other things you need
# If you're adding a file, copy paste edit this and add the "bin/filename.o" to the list in "sim:"
bin/lora.o: modules/lora.h modules/lora.cpp
	${CC} ${CC_FLAGS} -c modules/lora.cpp -o bin/lora.o

# Compile the main file
sim: simulator/main.cpp bin/lora.o bin/lora.o
	${CC} ${CC_FLAGS} simulator/main.cpp bin/lora.o -o sim

# Just in case someone makes the "clean" files
.PHONY: clean

clean:
	rm sim
	rm bin/*.o