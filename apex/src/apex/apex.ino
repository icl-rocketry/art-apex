// SPDX-FileCopyrightText: 2020 Brent Rubell for Adafruit Industries
//
// SPDX-License-Identifier: MIT

/*
  Web client

 This sketch connects to a website (wifitest.adafruit.com/testwifi/index.html)
 using the WiFi module.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */

#include <WiFi.h>

// Enter your WiFi SSID and password
char ssid[] = "ASTRA3";   // your network SSID (name)
char pass[] = "password"; // your network password (use for WPA, or use as key for WEP)
char start_message[] = "START?";

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192, 168, 238, 238); // numeric IP for Google (no DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(115200);

    // attempt to connect to Wifi network:
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Connected to WiFi");
}

void loop()
{
    while (1)
    {
        Serial.println("\nStarting connection to server...");
        // if you get a connection, report back via serial:
        if (client.connect(server, 8080))
        {
            Serial.println("connected to server");
            int n = client.write(start_message);
            Serial.println(n);

            delay(100);

            char c = client.read();
            Serial.println(c, HEX);
            if (c == 'y')
            {
                Serial.println("YAY");
                client.write("DONE");
                client.stop();
                goto end;
            }
        }
        delay(1000);
    }
end:

    // if the server's disconnected, stop the client:
    if (!client.connected())
    {
        Serial.println();
        Serial.println("disconnecting from server.");
        client.stop();

        // do nothing forevermore:
        while (true)
        {
            say_hi();
            delay(100);
        }
    }
}