// -*- mode: c++; coding: utf-8 -*-

// #include <Arduino.h>
// #include <SPI.h>
// #include <EEPROM.h>
// #include <RF24.h>
//
// #include <LFP.h>
//
// // These pins are for Arduino Nano
// #define RF_CE 7
// #define RF_CS 8
//
// RF24 radio(RF_CE, RF_CS);
//
// void setup() {
//     Serial.begin(115200);
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
//     radio.openReadingPipe(1, address);
//     radio.startListening();
//
//     Serial.print("OK");
// }
//
// void loop() {
//     if (radio.available()) {
//         Serial.println("Radio available!");
//     }
// }




#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <RF24.h>

#include <LFP.h>

// These pins are for Arduino Nano
#define RF_CE 7
#define RF_CS 8

#define SERIAL_SPEED 115200
#define MAX_PAYLOAD_SIZE 256

// nRF24 Bridge Protocol
#define NBP_MAGIC            "NBP"
#define CMD_SET_CHANNEL      '1'
#define CMD_SET_ADDRESS      '2'
#define CMD_SET_DESTINATION  '3'
#define CMD_SEND             '4'

RF24 radio(RF_CE, RF_CS);

bool serial_has_data() {
    byte ts = 50;
    while (!Serial.available()) {
        delay(1);
        if (!ts--)
            return false;
    }
    return true;
}

bool serial_read(byte* into, int size) {
    for (int i=0; i<size; i++) {
        if (!serial_has_data())
            return false;
        into[i] = Serial.read();
    }
    return true;
}

bool starts_with_magic() {
    String magic = NBP_MAGIC;
    byte buff[] = {0, 0, 0, 0};

    byte* c = buff;
    while (true) {
        if (!serial_has_data())
            return false;

        *c++ = Serial.read();
        if (c - buff < 3)
            continue;
        if (magic == (const char*)buff)
            return true;

        buff[0] = buff[1];
        buff[1] = buff[2];
        c--;
    }
    return false;
}

void cmd_set_channel() {
    if (!serial_has_data())
        return;
    byte channel = Serial.read();

    radio.setChannel(channel);

    // Serial.print("- set channel: ");
    // Serial.println(channel);
}

void cmd_set_address() {
    byte address[] = {0, 0, 0, 0, 0, 0};
    if (!serial_read(address, 5))
        return;

    // Serial.print("- set address: '");
    // Serial.print((const char*)address);
    // Serial.println("'");

    radio.openReadingPipe(1, address);
    radio.startListening();
}

void cmd_set_destination() {
    byte address[] = {0, 0, 0, 0, 0, 0};
    if (!serial_read(address, 5))
        return;

    // Serial.print("- set destination: '");
    // Serial.print((const char*)address);
    // Serial.println("'");

    radio.openWritingPipe(address);
}

void cmd_send() {
    byte buff[] = {0, 0};
    if (!serial_read(buff, 2))
         return;

    uint16_t size = ((uint16_t)buff[0] << 8) + buff[1];
    if (size > MAX_PAYLOAD_SIZE)
        return;

    byte payload[size];
    if (!serial_read(payload, size))
        return;

    LFP_write_packet(&radio, payload, size);
}

void process_input() {
    if (!starts_with_magic() or !serial_has_data())
        return;

    byte cmd = Serial.read();
    switch (cmd) {
        case CMD_SET_CHANNEL: return cmd_set_channel();
        case CMD_SET_ADDRESS: return cmd_set_address();
        case CMD_SET_DESTINATION: return cmd_set_destination();
        case CMD_SEND: return cmd_send();
    }
}

void forward_data() {
    byte payload[MAX_PAYLOAD_SIZE];
    uint16_t size = LFP_read_packet(&radio, payload, MAX_PAYLOAD_SIZE);
    if (size) {
        Serial.write(payload, size);
        Serial.flush();
    }
}

void setup() {
    Serial.begin(SERIAL_SPEED);

    radio.begin();
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_16);
    radio.setPayloadSize(32);
    radio.setAutoAck(true);
    radio.setRetries(10, 15);
    radio.setPALevel(RF24_PA_HIGH);

    Serial.print("OK");
}

void loop() {
    if (Serial.available())
        process_input();
    if (radio.available())
        forward_data();
}
