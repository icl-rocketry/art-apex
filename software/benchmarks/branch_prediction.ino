/*
This benchmark exists to see if the board has a branch predictor.
We find the maximum of a sorted and unsorted array (they should
be the same). If there's a branch predictor then it'll be faster
to find the max of the sorted array than the unsorted array since
branch prediction will skip a ton of branches.
*/

#include "util.h"

#define KB 1024
unsigned long seq_start;
unsigned long seq_end;

unsigned long rand_start;
unsigned long rand_end;
bool error = false;

uint32_t max(uint32_t* buffer, size_t len);

void setup() {
  Serial.begin(9600);

  const size_t size = 100 * KB;
  uint32_t* buffer = (uint32_t*)new uint8_t[size]; 

  // Make each item in the buffer point to the next one
  // The last item will point to the first one
  for (int i = 0; i < size / 4; i++) {
    buffer[i] = i;
  }
  buffer[size / 4 - 1] = 0;


  seq_start = micros();
  auto m1 = max(buffer, size / 4);
  seq_end = micros();
  // Make sure m1 is used so the compiler doesn't optimise the max call away
  // (If the result of the function isn't used, the compiler might just not make the call)
  Serial.println(m1);
  

  // Shuffle the elements around
  for (size_t last = size / 4 - 1; last > 0; last--) {
    // Pick a random index before last
    size_t idx = random(0, last);
    auto temp = buffer[last];
    buffer[last] = buffer[idx];
    buffer[idx] = temp;
  }
  
  rand_start = micros();
  auto m2 = max(buffer, size / 4);
  rand_end = micros();
  Serial.println(m2);

  // If our assumption isn't right, signal it by setting everything to 0
  error = m1 != m2;
}

uint32_t max(uint32_t* buffer, size_t len) {
  uint32_t max = 0;
  for (auto i = 0; i < len; i++) {
    if (buffer[i] > max) {
      max = buffer[i];
    }
  }
  return max;
}

void loop() {
  if (error) {
    Serial.println("Error");
  } else {
    Serial.printf("seq: %d, rand: %d\n", seq_end - seq_start, rand_end - rand_start);
  }
  delay(1000);
}
