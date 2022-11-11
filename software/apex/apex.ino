#include "bno/module.hpp"
#include <Wire.h>

Adafruit_BNO08x bno08x;
sh2_SensorValue_t sensorValue;

void setup(void) {
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

  delay(100);

  bool found = false;

  // Try to initialize!
  for (uint8_t x = 0; x < 0x100; x++) {
    if (!bno08x.begin_I2C(x, &Wire1, 0)) {
      Serial.println("Failed to find BNO08x chip");
      delay(50);
    } else {
      Serial.println(x);
      found = true;
      break;
    }
  }

  if (found) {
    Serial.println("BNO08x Found!");
  }
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

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  }
  if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
    Serial.println("Could not enable gyroscope");
  }
  if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
    Serial.println("Could not enable magnetic field calibrated");
  }
  if (!bno08x.enableReport(SH2_LINEAR_ACCELERATION)) {
    Serial.println("Could not enable linear acceleration");
  }
  if (!bno08x.enableReport(SH2_GRAVITY)) {
    Serial.println("Could not enable gravity vector");
  }
  if (!bno08x.enableReport(SH2_ROTATION_VECTOR)) {
    Serial.println("Could not enable rotation vector");
  }
  if (!bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR)) {
    Serial.println("Could not enable geomagnetic rotation vector");
  }
  if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
    Serial.println("Could not enable game rotation vector");
  }
  if (!bno08x.enableReport(SH2_STEP_COUNTER)) {
    Serial.println("Could not enable step counter");
  }
  if (!bno08x.enableReport(SH2_STABILITY_CLASSIFIER)) {
    Serial.println("Could not enable stability classifier");
  }
  if (!bno08x.enableReport(SH2_RAW_ACCELEROMETER)) {
    Serial.println("Could not enable raw accelerometer");
  }
  if (!bno08x.enableReport(SH2_RAW_GYROSCOPE)) {
    Serial.println("Could not enable raw gyroscope");
  }
  if (!bno08x.enableReport(SH2_RAW_MAGNETOMETER)) {
    Serial.println("Could not enable raw magnetometer");
  }
  if (!bno08x.enableReport(SH2_SHAKE_DETECTOR)) {
    Serial.println("Could not enable shake detector");
  }
  if (!bno08x.enableReport(SH2_PERSONAL_ACTIVITY_CLASSIFIER)) {
    Serial.println("Could not enable personal activity classifier");
  }
}
void printActivity(uint8_t activity_id) {
  switch (activity_id) {
  case PAC_UNKNOWN:
    Serial.print("Unknown");
    break;
  case PAC_IN_VEHICLE:
    Serial.print("In Vehicle");
    break;
  case PAC_ON_BICYCLE:
    Serial.print("On Bicycle");
    break;
  case PAC_ON_FOOT:
    Serial.print("On Foot");
    break;
  case PAC_STILL:
    Serial.print("Still");
    break;
  case PAC_TILTING:
    Serial.print("Tilting");
    break;
  case PAC_WALKING:
    Serial.print("Walking");
    break;
  case PAC_RUNNING:
    Serial.print("Running");
    break;
  case PAC_ON_STAIRS:
    Serial.print("On Stairs");
    break;
  default:
    Serial.print("NOT LISTED");
  }
  Serial.print(" (");
  Serial.print(activity_id);
  Serial.print(")");
}
void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }
  
  Serial.println("=================");
  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
  Serial.println("=================");
}