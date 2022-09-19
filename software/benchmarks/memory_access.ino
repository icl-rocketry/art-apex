/*
This benchmark compares the performance of the board when reading data in a 
sequential order vs a random order. The significance of this benchmark is that
if the performance is similar, then there's no cache at *any* stage of the 
board's pipeline and so we won't need to think about data locality at all.

(rp2040)
I believe that there's no cache already since it's not on the diagram of the
ARM Cortex M0+, but I'm not an electrical engineer so there's no harm in checking.
*/

#include "util.h"

#define KB 1024
unsigned long seq_start;
unsigned long seq_end;

unsigned long rand_start;
unsigned long rand_end;
bool error = false;

uint32_t sum(uint32_t* buffer, size_t len);

void setup() {
  Serial.begin(9600);

  const size_t size = 100 * KB;
  uint32_t* buffer = (uint32_t*)new uint8_t[size]; 

  // Make each item in the buffer point to the next one
  // The last item will point to the first one
  for (int i = 0; i < size / 4; i++) {
    buffer[i] = i + 1;
  }
  buffer[size / 4 - 1] = 0;


  seq_start = micros();
  auto s1 = sum(buffer, size / 4);
  seq_end = micros();
  // Make sure s is used so the compiler doesn't optimise the sum call away
  // (If the result of the function isn't used, the compiler might just not make the call)
  Serial.println(s1);
  

  // Shuffle the elements around to make the memory accesses "random"
  // Uses the minesweeper random algorithm to make sure that we'll still 
  // visit every element.
  // But first the array needs to have item i at index i
  for (int i = 0; i < size / 4; i++) {
    buffer[i] = i;
  }

  for (size_t last = size / 4 - 1; last > 0; last--) {
    // Pick a random index before last
    size_t idx = random(0, last);
    auto temp = buffer[last];
    buffer[last] = buffer[idx];
    buffer[idx] = temp;
  }
  
  rand_start = micros();
  auto s2 = sum(buffer, size / 4);
  rand_end = micros();
  Serial.println(s2);

  // If our assumption isn't right, signal it by setting everything to 0
  error = s1 != s2;
}

uint32_t sum(uint32_t* buffer, size_t len) {
  uint32_t sum = 0;
  size_t n_visited = 0;
  size_t i = 0;
  do {
    sum += i;
    i = buffer[i];
    n_visited++;
  } while(n_visited < len);
  return sum;
}

void loop() {
  if (error) {
    Serial.println("Error");
  } else {
    Serial.printf("seq: %d, rand: %d\n", seq_end - seq_start, rand_end - rand_start);
  }
  delay(1000);
}
