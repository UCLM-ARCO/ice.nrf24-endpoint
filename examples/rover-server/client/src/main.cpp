// -*- mode: c++; coding; utf-8 -*-

#include <Arduino.h>
#include <EEPROM.h>

#include <IceC.h>
#include <RF24.h>
#include <nRF24Endpoint.h>
#include <MowayduinoRobot.h>

#include "rover.h"

// Arduino Duemilanove pins
// #define CE_PIN 7
// #define CS_PIN 8

// Mowayduino pins
#define CE_PIN 0
#define CS_PIN 17

#define NORMAL_SPEED 100

mowayduinorobot robot;

Ice_Communicator ic;
Ice_ObjectPrx rover;

void setup() {
    // Serial.begin(115200);
    robot.beginMowayduino();
    robot.Ledsoff();

    // init and setup communicator
    Ice_initialize(&ic);
    nRF24Endpoint_init(&ic, CE_PIN, CS_PIN);

    // create proxy to remote object
    Ice_Communicator_stringToProxy(
        &ic, "Rover -d:nrf24 -c 100 -a rover", &rover);
    // Serial.println("Ready!");
}

void loop() {
    // Serial.println("Iter");
    Example_Rover_forward(&rover);
    delay(500);
    Example_Rover_stop(&rover);

    robot.Blueon();
    delay(50);
    robot.Blueoff();
    delay(950);
}
