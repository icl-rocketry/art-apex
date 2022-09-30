#include "structFS/module.hpp"

#define KB 1024
#define MB (KB * KB)

File file(2*MB);
const uint32_t FILE_SIZE = 2*MB;
const int N_WORDS = 4096; // 1 word is 4 bytes

unsigned long write_start;
unsigned long write_end;
unsigned long erase_start;
unsigned long erase_end;

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
  erase_start = micros();
  file.makeWriteable();
  Serial.println("Filesys initialised");
  erase_end = micros();


  write_start = micros();
  for (uint32_t i = 0; i < N_WORDS; i++) {
    if (!file.append(i+300)) {
      Serial.printf("Couldn't add %d to file\n", i);
    }
  }

  if (!file.flush()) {
    Serial.println("Couldn't flush file");
  }
  write_end = micros();

  Serial.println("Done writing");
  delay(5000);
  uint32_t x[N_WORDS];
  if (!file.read(0, &x, sizeof x)) {
    Serial.printf("Couldn't read\n");
  }

  for (int i = 0; i < N_WORDS; i++) {
    Serial.printf("%d: Read %zu\n", i, x[i]);
  }
}

void loop() {
  Serial.printf("Time taken to write %d bytes: %d microseconds\n", N_WORDS * 4, write_end - write_start);
  Serial.printf("Time taken to erase %d kilobytes: %d microseconds\n", FILE_SIZE / KB, erase_end - erase_start);
}

void wait_for_char(char c) {
  char c1;
  do {
    Serial.printf("Press %c to start\n", c);
    Serial.readBytes(&c1, sizeof(char));
    delay(100);
  } while(c1 != c);
}