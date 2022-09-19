#include "structFS/module.hpp"

#define KB 1024
#define MB (KB * KB)

File file(2*MB);
unsigned long start;
unsigned long end;

void setup() {
  Serial.begin(9600);

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
  Serial.println("Filesys initialised");


  const int n = 1024; //Fill up the buffer, but don't cause a flush
  for (uint32_t i = 0; i < n; i++) {
    if (!file.append(53)) {
      Serial.printf("Couldn't add %d to file\n", i);
    }
  }

  start = micros();
  if (!file.flush()) {
    Serial.println("Couldn't flush file");
  }
  end = micros();


  Serial.println("Done writing");
  delay(5000);
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