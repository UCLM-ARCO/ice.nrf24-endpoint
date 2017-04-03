// -*- mode: c++; coding; utf-8 -*-

#include <Arduino.h>
#include <EEPROM.h>

#include <IceC.h>
#include <RF24.h>
#include <nRF24Endpoint.h>
// #include <MowayduinoRobot.h>

#include "rover.h"

// Arduino Duemilanove pins
#define CE_PIN 7
#define CS_PIN 8

// Mowayduino pins
// #define CE_PIN 0
// #define CS_PIN 17

#define NORMAL_SPEED 50

// mowayduinorobot robot;

Ice_Communicator ic;
Ice_ObjectAdapter adapter;
Example_Rover servant;

void setup() {
    Serial.begin(115200);
    // robot.beginMowayduino();
    // robot.Ledsoff();
    // robot.beginMotor();

    // init and setup communicator
    Ice_initialize(&ic);
    nRF24Endpoint_init(&ic, CE_PIN, CS_PIN);

    // create object adapter
    Ice_Communicator_createObjectAdapterWithEndpoints
        (&ic, "Adapter", "nrf24 -c 100 -a rover", &adapter);
    Ice_ObjectAdapter_activate(&adapter);

    // register servant
    Example_Rover_init(&servant);
    Ice_ObjectAdapter_add(&adapter, (Ice_ObjectPtr)&servant, "Rover");
    Serial.println("Ready!");
}

void loop() {
    Ice_Communicator_loopIteration(&ic);
}

void
Example_RoverI_forward(Example_RoverPtr self) {
    Serial.println("Forward");
//    robot.Straight(NORMAL_SPEED);
}

void
Example_RoverI_backward(Example_RoverPtr self) {
    Serial.println("Backward");
//    robot.Back(NORMAL_SPEED);
}

void
Example_RoverI_left(Example_RoverPtr self) {
    Serial.println("Left");
//    robot.TurnLeft(NORMAL_SPEED);
}

void
Example_RoverI_right(Example_RoverPtr self) {
    Serial.println("Right");
//    robot.TurnRight(NORMAL_SPEED);
}

void
Example_RoverI_stop(Example_RoverPtr self) {
    Serial.println("Stop");
//    robot.Stop();
}
