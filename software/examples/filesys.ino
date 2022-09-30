#include "structFS/module.hpp"

#define KB 1024
#define MB (KB * KB)

File file(2*MB);
void setup() {
  Serial.begin(9600);

  //Just to make sure it only starts when you want it to
  wait_for_char('h');
  wait_for_char('e');
  wait_for_char('l');
  wait_for_char('l');
  wait_for_char('o');

  FS fs = FS(4 * MB);

  // Add the file to the filesystem (only need to do this because my semantics are janky)
  if (!fs.AddFile(file)) {
    Serial.println("Couldn't add file");
  }
  file.makeWriteable(); // Warning, this will erase all data in the file, so only do this when you need to
  Serial.println("Filesys initialised");

  // Write your data here
  const int n = 4096;
  for (uint32_t i = 0; i < n; i++) {
    if (!file.append(i)) {
      Serial.printf("Couldn't add %d to file\n", i);
    }
  }

  // Make sure to write any data that's left in the buffer
  if (!file.flush()) {
    Serial.println("Couldn't flush file");
  }

  Serial.println("Done writing");
  delay(5000);

  // Reading obviously
  uint32_t x[n];
  if (!file.read(0, &x, sizeof x)) {
    Serial.printf("Couldn't read\n");
  }

  for (int i = 0; i < n; i++) {
    Serial.printf("%d: Read %zu\n", i, x[i]);
  }
}

void loop() {
  Serial.println(end - start);
}

void wait_for_char(char c) {
  char c1;
  do {
    Serial.printf("Press %c to start\n", c);
    Serial.readBytes(&c1, sizeof(char));
    delay(100);
  } while(c1 != c);
}