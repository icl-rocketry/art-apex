# Use this to easily and efficiently compile the code
# It only updates things you've changed, so you don't need to spend years compiling

# Set up some variables
CC = g++
CC_FLAGS = -g # This lets you use debuggers like gdb

# Compile other things you need
# If you're adding a file, copy paste edit this
bin/test.o: modules/test.h modules/test.cpp
	${CC} ${CC_FLAGS} -c modules/test.cpp -o bin/test.o

# Compile the main file
sim: main.cpp bin/test.o
	${CC} ${CC_FLAGS} main.cpp bin/test.o -o sim

# Just in case someone makes the "clean" files
.PHONY: clean

all: sim

clean:
	rm sim
	rm bin/*.o