// -*- mode: c++; coding; utf-8 -*-

// #include <Arduino.h>
// #include <SPI.h>
// #include <EEPROM.h>
// #include <RF24.h>
// #include <MowayduinoRobot.h>
//
// #include "LFP.h"
//
// // These pins are for Arduino Nano
// // #define RF_CE 7
// // #define RF_CS 8
//
// // Mowayduino pins
// #define CE_PIN 0
// #define CS_PIN 17
//
// RF24 radio(CE_PIN, CS_PIN);
// mowayduinorobot robot;
//
// void setup() {
//     // Serial.begin(115200);
//     robot.beginMowayduino();
//     robot.Ledsoff();
//
//     radio.begin();
//     radio.setDataRate(RF24_2MBPS);
//     radio.setCRCLength(RF24_CRC_16);
//     radio.setPayloadSize(32);
//     radio.setAutoAck(true);
//     radio.setRetries(10, 15);
//     radio.setPALevel(RF24_PA_HIGH);
//
//     byte address[] = "rover";
//     radio.setChannel(100);
//     // radio.openReadingPipe(1, address);
//     radio.openWritingPipe(address);
//     // radio.startListening();
//
//     // Serial.println("BOOT done!");
//     robot.Blueon();
//     delay(50);
//     robot.Blueoff();
// }
//
// void loop() {
//
//     byte payload[] = {
//          0x49, 0x63, 0x65, 0x50, 0x01, 0x00, 0x01, 0x00,
//          0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00,
//          0x00, 0x00, 0x05, 0x52, 0x6F, 0x76, 0x65, 0x72,
//          0x00, 0x00, 0x07, 0x66, 0x6F, 0x72, 0x77, 0x61,
//          0x72, 0x64, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
//          0x01, 0x00
//     };
//
//     byte msg_size = 42;
//     byte sent_size = LFP_write_packet(&radio, payload, msg_size);
//
//     if (sent_size == msg_size)
//         robot.Greenon();
//     else
//         robot.Redon();
//
//     delay(50);
//     robot.Ledsoff();
//     delay(950);
//
//     // if (radio.available()) {
//     //     Serial.println("Data ready!");
//     //     // radio.read(void *buf, uint8_t len)
//     // }
//
//
// }







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
    // delay(500);
    // Example_Rover_stop(&rover);

    robot.Blueon();
    delay(50);
    robot.Blueoff();
    delay(950);
}
