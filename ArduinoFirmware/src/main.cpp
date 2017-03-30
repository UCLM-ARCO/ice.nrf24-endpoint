// -*- mode: c++; coding: utf-8 -*-

#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <RF24.h>

#include <LFP.h>

// These pins are for Arduino Nano
#define RF_CE 7
#define RF_CS 8

RF24 radio(RF_CE, RF_CS);

void setup() {
    Serial.begin(115200);

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening();
}

void loop() {
    if (!Serial.available())
        return;
}
