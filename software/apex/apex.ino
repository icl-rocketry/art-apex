#include "structFS/module.hpp"

#define KB 1024
#define MB (KB * KB)

void setup() {
  Serial.begin(9600);
  FS fs(4 * MB);
  File file(2 * MB);

  if (!fs.AddFile(file)) {
    Serial.println("Couldn't initialise filesystem");
  }

  delay(100);

  for (uint32_t i = 0; i < 512; i++) {
    if (!file.append(i)) {
      Serial.printf("Couldn't add %d to file \n", i);
    }
  }

  uint32_t x;
  for (int i = 0; i < 512; i++) {
    if (!file.read((size_t) (i * 4), &x, 4)) {
      Serial.printf("Couldn't read %dth int\n", i);
    } else {
      Serial.printf("Read %d\n", x);
    }
  }
}

void loop() {}