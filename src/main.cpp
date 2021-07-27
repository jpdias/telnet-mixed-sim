#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SoftwareSerial.h>
#include "secrets.hpp"

#define SERVER_ADDR "192.168.1.254"
#define SERVER_PORT 23

byte pinList[8] = { D0, D1, D2, D3, D4, D5, D6, D7 };

WiFiClient client;

void togglePins(char c) {
    for (auto i = 0; i < 8; i++) {
        digitalWrite(pinList[i], (c >> i) & 1);
    }
}

boolean checkParity(char c) {
    byte parity = 0;
    for (byte i = 0; i < 8; i++)
        parity ^= (c >> i) & 1;

    return parity == 0;
}

void setup() {
    Serial.begin(115200);
    for (byte i = 0; i < 8; i++)
        pinMode(pinList[i], OUTPUT);

    ESP8266WiFiMulti WiFiMulti;
    WiFiMulti.addAP(WIFI_SSID, WIFI_PWD);
    while (WiFiMulti.run() != WL_CONNECTED)
    {
        delay(100);
    }
}

void loop() {
    if (client.connected() && client.available()) {
        if (Serial.available() > 0) {
            auto incomingByte = Serial.read();
            client.write(incomingByte);
        }

        auto c = char(client.read()) & 0b01111111;

        if (checkParity(c)) {
            Serial.print("Received: ");
        } else {
            Serial.print("Wrong parity: ");
        }

        togglePins(c);
        Serial.println(c, BIN);
    }
    else if (!client.connected())
    {
        Serial.println("Not connected");
        while (!client.connect(SERVER_ADDR, SERVER_PORT)) {
            yield();
            delay(100);
        }
        Serial.println("Connected");
    }
}
