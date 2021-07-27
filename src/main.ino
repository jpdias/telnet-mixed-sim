#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>

WiFiClient client;

boolean checkParity(char c)
{
    byte parity = 0;
    for (byte i = 1; i < 8; i++)
    {
        parity ^= (c >> i) & 0x01;
    }
    return parity == (c & 0x01);
}

void setup()
{
    Serial.begin(115200);
    ESP8266WiFiMulti WiFiMulti;
    WiFiMulti.addAP("wifi", "wifipass");
    while (WiFiMulti.run() != WL_CONNECTED)
    {
        delay(100);
    }

    if (client.connect("192.168.0.149", 23))
    { //port to connect to; telnet/TCP
        Serial.println("connected");
    }
    else
    {
        Serial.println("connection failed");
    }
}

void loop()
{
    if (client.available() && client.connected())
    {
        auto c = char(client.read());
        if (checkParity(c))
        {
            Serial.println(c, BIN);
        }
        else
        {
            Serial.print("Wrong parity:");
            Serial.println(c, BIN);
        }
    }
}
