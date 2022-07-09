#include <Wire.h>
#include <Adafruit_DPS310.h>

Adafruit_DPS310 dps;

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

    dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
    Serial.println("Started");
}

void loop()
{
    sensors_event_t temp_event, pressure_event;

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