#include "structFS/module.hpp"

int x = 100;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // auto foo = Foo(4);
  Serial.printf("x = %d\n", x);
  delay(100);
}