// #include "bno/module.hpp"
// #include <Wire.h>
// #include <Arduino.h>

// Adafruit_BNO08x bno08x;
// sh2_SensorValue_t sensorValue;

// void setup(void) {
//   Wire.begin(SDA1, SCL1);
//   Serial.begin(9600);
//   while (!Serial)
//     delay(10); // will pause Zero, Leonardo, etc until serial console opens

//   Serial.println("Adafruit BNO08x test!");

//   // Try to initialize!
//   if (!bno08x.begin_I2C(BNO08x_I2CADDR_DEFAULT, &Wire, 0)) {
//     // if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte
//     // UART buffer! if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
//     Serial.println("Failed to find BNO08x chip");
//     while (1) {
//       delay(10);
//     }
//   }
//   Serial.println("BNO08x Found!");

//   for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
//     Serial.print("Part ");
//     Serial.print(bno08x.prodIds.entry[n].swPartNumber);
//     Serial.print(": Version :");
//     Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
//     Serial.print(".");
//     Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
//     Serial.print(".");
//     Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
//     Serial.print(" Build ");
//     Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
//   }

//   setReports();

//   Serial.println("Reading events");
//   delay(100);
// }

// // Here is where you define the sensor outputs you want to receive
// void setReports(void) {
//   Serial.println("Setting desired reports");
//   if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
//     Serial.println("Could not enable accelerometer");
//   }
//   if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
//     Serial.println("Could not enable gyroscope");
//   }
//   if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
//     Serial.println("Could not enable magnetic field calibrated");
//   }
//   if (!bno08x.enableReport(SH2_LINEAR_ACCELERATION)) {
//     Serial.println("Could not enable linear acceleration");
//   }
//   if (!bno08x.enableReport(SH2_GRAVITY)) {
//     Serial.println("Could not enable gravity vector");
//   }
//   if (!bno08x.enableReport(SH2_ROTATION_VECTOR)) {
//     Serial.println("Could not enable rotation vector");
//   }
//   if (!bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR)) {
//     Serial.println("Could not enable geomagnetic rotation vector");
//   }
//   if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
//     Serial.println("Could not enable game rotation vector");
//   }
//   if (!bno08x.enableReport(SH2_STEP_COUNTER)) {
//     Serial.println("Could not enable step counter");
//   }
//   if (!bno08x.enableReport(SH2_STABILITY_CLASSIFIER)) {
//     Serial.println("Could not enable stability classifier");
//   }
//   if (!bno08x.enableReport(SH2_RAW_ACCELEROMETER)) {
//     Serial.println("Could not enable raw accelerometer");
//   }
//   if (!bno08x.enableReport(SH2_RAW_GYROSCOPE)) {
//     Serial.println("Could not enable raw gyroscope");
//   }
//   if (!bno08x.enableReport(SH2_RAW_MAGNETOMETER)) {
//     Serial.println("Could not enable raw magnetometer");
//   }
//   if (!bno08x.enableReport(SH2_SHAKE_DETECTOR)) {
//     Serial.println("Could not enable shake detector");
//   }
//   if (!bno08x.enableReport(SH2_PERSONAL_ACTIVITY_CLASSIFIER)) {
//     Serial.println("Could not enable personal activity classifier");
//   }
// }
// void printActivity(uint8_t activity_id) {
//   switch (activity_id) {
//   case PAC_UNKNOWN:
//     Serial.print("Unknown");
//     break;
//   case PAC_IN_VEHICLE:
//     Serial.print("In Vehicle");
//     break;
//   case PAC_ON_BICYCLE:
//     Serial.print("On Bicycle");
//     break;
//   case PAC_ON_FOOT:
//     Serial.print("On Foot");
//     break;
//   case PAC_STILL:
//     Serial.print("Still");
//     break;
//   case PAC_TILTING:
//     Serial.print("Tilting");
//     break;
//   case PAC_WALKING:
//     Serial.print("Walking");
//     break;
//   case PAC_RUNNING:
//     Serial.print("Running");
//     break;
//   case PAC_ON_STAIRS:
//     Serial.print("On Stairs");
//     break;
//   default:
//     Serial.print("NOT LISTED");
//   }
//   Serial.print(" (");
//   Serial.print(activity_id);
//   Serial.print(")");
// }
// void loop() {
//   delay(10);

//   if (bno08x.wasReset()) {
//     Serial.print("sensor was reset ");
//     setReports();
//   }
  
//   Serial.println("=================");
//   if (!bno08x.getSensorEvent(&sensorValue)) {
//     return;
//   }
//   Serial.println("=================");
// }

#include <Adafruit_TestBed.h>
extern Adafruit_TestBed TB;

#define DEFAULT_I2C_PORT &Wire

// Some boards have TWO I2C ports, how nifty. We should scan both
#if defined(ARDUINO_ARCH_RP2040) \
    || defined(ARDUINO_ADAFRUIT_QTPY_ESP32S2) \
    || defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3_NOPSRAM) \
    || defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3) \
    || defined(ARDUINO_ADAFRUIT_QTPY_ESP32_PICO) \
    || defined(ARDUINO_SAM_DUE)
  #define SECONDARY_I2C_PORT &Wire1
#endif

void setup() {
  Serial.begin(115200);

  // Wait for Serial port to open
  while (!Serial) {
    delay(10);
  }
  delay(500);
  Serial.println("Adafruit I2C Scanner");

#if defined(ARDUINO_ADAFRUIT_QTPY_ESP32S2) || \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3_NOPSRAM) || \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3) || \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32_PICO)
  // ESP32 is kinda odd in that secondary ports must be manually
  // assigned their pins with setPins()!
  Wire1.setPins(SDA1, SCL1);
#endif

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  // turn on the I2C power by setting pin to opposite of 'rest state'
  pinMode(PIN_I2C_POWER, INPUT);
  delay(1);
  bool polarity = digitalRead(PIN_I2C_POWER);
  pinMode(PIN_I2C_POWER, OUTPUT);
  digitalWrite(PIN_I2C_POWER, !polarity);
#endif

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2_TFT)
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
#endif

#if defined(ADAFRUIT_FEATHER_ESP32_V2)
  // Turn on the I2C power by pulling pin HIGH.
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, HIGH);
#endif
}

void loop() {
  Serial.println("");
  Serial.println("");

  Serial.print("Default port (Wire) ");
  TB.theWire = DEFAULT_I2C_PORT;
  TB.printI2CBusScan();

#if defined(SECONDARY_I2C_PORT)
  Serial.print("Secondary port (Wire1) ");
  TB.theWire = SECONDARY_I2C_PORT;
  TB.printI2CBusScan();
#endif

  delay(3000); // wait 3 seconds
}