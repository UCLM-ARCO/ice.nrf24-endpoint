// -*- mode: c++; coding: utf-8 -*-

#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>
#include <RF24.h>

#include <IceC.h>
#include <nRF24Endpoint.h>
#include <MowayduinoRobot.h>

#include "rover.h"

#define CE_PIN 0
#define CS_PIN 17
#define NORMAL_SPEED 100
#define IDM_ADDRESS "5F10"

Ice_Communicator ic;
Ice_ObjectAdapter adapter;
Example_Rover servant;

mowayduinorobot robot;

void setup() {
    robot.beginMowayduino();
    robot.Ledsoff();

    // initialize Ice and Object adapter
    Ice_initialize(&ic);
    nRF24Endpoint_init(&ic, CE_PIN, CS_PIN);
    Ice_Communicator_createObjectAdapterWithEndpoints(
        &ic, "Adapter", "nrf24 -c 90 -a car01", &adapter);
    Ice_ObjectAdapter_activate(&adapter);

    // register Rover servant
    Example_Rover_init(&servant);
    Ice_ObjectAdapter_add(&adapter, (Ice_ObjectPtr)&servant, IDM_ADDRESS);

    robot.Greenon();
    delay(50);
    robot.Greenoff();
}

void loop() {
    Ice_Communicator_loopIteration(&ic);
}

void Example_RoverI_forward(Example_RoverPtr self) {
    robot.Straight(NORMAL_SPEED);
}

void Example_RoverI_backward(Example_RoverPtr self) {
    robot.Back(NORMAL_SPEED);
}

void Example_RoverI_left(Example_RoverPtr self) {
    robot.TurnLeft(NORMAL_SPEED);
}

void Example_RoverI_right(Example_RoverPtr self) {
    robot.TurnRight(NORMAL_SPEED);
}

void Example_RoverI_stop(Example_RoverPtr self) {
    robot.Stop();
}
