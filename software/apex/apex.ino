#include "bno/module.hpp"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>

#define DEBUG_SERIAL 1

Adafruit_BNO08x bno08x;
sh2_SensorValue_t sensorValue;

// Performance counters
uint32_t n_msgs = 0;

// Macros for sensor definitions
#define ACCELEROMETER 0x01
#define GYRO          0x02
#define MAG           0x04
#define LIN_ACC       0x08
#define ROTATION      0x10
#define GEOMAG_ROT    0x20

void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

  // Try to initialize!
  if (!bno08x.begin_I2C(BNO08x_I2CADDR_DEFAULT, &Wire1, 0)) {
    Serial.println("Failed to find BNO08x chip");
    delay(50);
    while (1) {
      delay(10);
    }
  }

  
  Serial.println("BNO08x Found!");
  
  for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
    Serial.print("Part ");
    Serial.print(bno08x.prodIds.entry[n].swPartNumber);
    Serial.print(": Version :");
    Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
  }

  enable_reports(0);

  Serial.println("Reading events");
  delay(100);
}

void enable_reports(uint8_t flag) {
  if ((flag & ACCELEROMETER) && !bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  }
  if ((flag & GYRO) && !bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
    Serial.println("Could not enable gyroscope");
  }
  if ((flag & MAG) && !bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
    Serial.println("Could not enable magnetic field calibrated");
  }
  if ((flag & LIN_ACC) && !bno08x.enableReport(SH2_LINEAR_ACCELERATION)) {
    Serial.println("Could not enable linear acceleration");
  }
  if ((flag & ROTATION) && !bno08x.enableReport(SH2_ROTATION_VECTOR)) {
    Serial.println("Could not enable rotation vector");
  }
  if ((flag & GEOMAG_ROT) && !bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR)) {
    Serial.println("Could not enable geomagnetic rotation vector");
  }
}

// Measure the time it takes to fill an array
void fill(uint8_t flags) {
  uint8_t temp_flags = 0;

  while (temp_flags != flags) {
    if (!bno08x.getSensorEvent(&sensorValue)) {
      continue;
    }

    n_msgs++;
    switch (sensorValue.sensorId) {
      case SH2_ACCELEROMETER:
        temp_flags |= ACCELEROMETER;
        break;
      case SH2_GYROSCOPE_CALIBRATED:
        temp_flags |= GYRO;
        break;
      case SH2_MAGNETIC_FIELD_CALIBRATED:
        temp_flags |= MAG;
        break;
      case SH2_LINEAR_ACCELERATION:
        temp_flags |= LIN_ACC;
        break;
      case SH2_ROTATION_VECTOR:
        temp_flags |= ROTATION;
        break;
      case SH2_GEOMAGNETIC_ROTATION_VECTOR:
        temp_flags |= GEOMAG_ROT;
        break;
    }
  }
}

#define TIME_IT(expr) start = micros(); \
                      for (int i = 0; i < 50; i++) { expr; } \
                      end = micros(); \
                      Serial.printf(" took %d microseconds and used %d packets\n", end - start, n_msgs); \
                      n_msgs = 0

void loop() {
  uint8_t flags = 0;
  unsigned long start = 0;
  unsigned long end = 0;

  flags |= ACCELEROMETER;
  Serial.print("Accelerometer only");
  enable_reports(flags);
  delay(100);
  TIME_IT(fill(flags));
  bno08x.hardwareReset();
  delay(1000);

  flags |= GYRO;
  Serial.print("+ Gyro");
  enable_reports(flags);
  delay(100);
  TIME_IT(fill(flags));
  bno08x.hardwareReset();
  delay(1000);

  flags |= MAG;
  Serial.print("+ Mag");
  enable_reports(flags);
  delay(100);
  TIME_IT(fill(flags));
  bno08x.hardwareReset();
  delay(1000);

  flags |= LIN_ACC;
  Serial.print("+ Linear accelerometer");
  enable_reports(flags);
  delay(100);
  TIME_IT(fill(flags));
  bno08x.hardwareReset();
  delay(1000);

  flags |= ROTATION;
  Serial.print("+ Rotation");
  enable_reports(flags);
  delay(100);
  TIME_IT(fill(flags));
  bno08x.hardwareReset();
  delay(1000);

  flags |= GEOMAG_ROT;
  Serial.print("+ Geomagnetic rotation");
  enable_reports(flags);
  delay(100);
  TIME_IT(fill(flags));
  bno08x.hardwareReset();
  delay(1000);
}
