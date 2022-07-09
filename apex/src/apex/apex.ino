#include <Wire.h>
#include <Adafruit_DPS310.h>
#include <Adafruit_NeoPixel.h>


#define NUMPIXELS 1

Adafruit_DPS310 dps;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(115200);

    Wire1.setPins(SDA1, SCL1);
    Wire1.begin();

    if (!dps.begin_I2C(DPS310_I2CADDR_DEFAULT, &Wire1))
    { // Can pass in I2C address here
        while (1)
        {
            Serial.println("Failed to find DPS");
            delay(10);
        }
    }

    dps.setMode(DPS310_CONT_PRESTEMP);
    dps.configurePressure(DPS310_128HZ, DPS310_8SAMPLES);
    dps.configureTemperature(DPS310_128HZ, DPS310_8SAMPLES);

#if defined(NEOPIXEL_POWER)
    // If this board has a power control pin, we must set it to output and high
    // in order to enable the NeoPixels. We put this in an #if defined so it can
    // be reused for other boards without compilation errors
    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

    pixels.begin();
    pixels.setBrightness(20);

    Serial.println("Started");
}

void loop()
{
    sensors_event_t temp_event, pressure_event;

      pixels.fill(0xFF0000);
      pixels.show();
      delay(500);
      pixels.fill(0x00FF00);
      pixels.show();
      delay(500);
      pixels.fill(0x0000FF);
      pixels.show();
      delay(500);


    while (!dps.temperatureAvailable() || !dps.pressureAvailable())
    {
        return; // wait until there's something to read
    }

    dps.getEvents(&temp_event, &pressure_event);
    Serial.print(F("Temperature = "));
    Serial.print(temp_event.temperature);
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(pressure_event.pressure);
    Serial.println(" hPa");

    Serial.println();
}