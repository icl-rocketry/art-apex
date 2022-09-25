#define PROFILING 0

#include "structFS/module.hpp"
#include "util/Profiling.h"

#define KB 1024
#define MB (KB * KB)

File file(2*MB);
unsigned long start;
unsigned long end;

void setup() {
  Serial.begin(9600);

  #if PROFILING
    PROFILE_BEGIN();
  #endif


  //Just to make sure it only starts when you want it to
  wait_for_char('h');
  wait_for_char('e');
  wait_for_char('l');
  wait_for_char('l');
  wait_for_char('o');

  FS fs = FS(4 * MB);

  if (!fs.AddFile(file)) {
    Serial.println("Couldn't add file");
  }
  file.makeWriteable();
  Serial.println("Filesys initialised");


  start = micros();
  const int n = 4096;
  for (uint32_t i = 0; i < n; i++) {
    if (!file.append(i+300)) {
      Serial.printf("Couldn't add %d to file\n", i);
    }
  }

  if (!file.flush()) {
    Serial.println("Couldn't flush file");
  }
  end = micros();

  Serial.println("Done writing");
  // delay(5000);
  uint32_t x[n];
  if (!file.read(0, &x, sizeof x)) {
    Serial.printf("Couldn't read\n");
  }

  for (int i = 0; i < n; i++) {
    Serial.printf("%d: Read %zu\n", i, x[i]);
  }

  #if PROFILING
    PROFILE_END();
  #endif
}

void loop() {
  // Serial.println(end - start);
}

void wait_for_char(char c) {
  char c1;
  do {
    Serial.printf("Press %c to start\n", c);
    Serial.readBytes(&c1, sizeof(char));
    delay(100);
  } while(c1 != c);
}