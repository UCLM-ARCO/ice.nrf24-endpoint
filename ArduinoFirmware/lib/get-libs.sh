#!/bin/bash
# -*- mode: sh; coding: utf-8 -*-

BB=https://bitbucket.org/arco_group

echo -n "IceC... "
if [ ! -d IceC ]; then
    wget -q $BB/icec/downloads/arduino-icec-latest.zip
    unzip -qq arduino-icec-latest.zip
    rm arduino-icec-latest.zip
    echo "[ok] "
else
    echo "[already present]"
fi

echo -n "RF24... "
if [ ! -d RF24 ]; then
    wget -q https://github.com/TMRh20/RF24/archive/master.zip -O RF24.zip
    unzip -qq RF24.zip
    mv RF24-master RF24
    rm RF24.zip
    echo "[ok] "
else
    echo "[already present]"
fi

echo -n "nRF24Endpoint... "
if [ ! -d nRF24Endpoint ]; then
    wget -q $BB/icec.nrf24-endpoint/downloads/arduino-nrf24-endpoint-latest.zip
    unzip -qq arduino-nrf24-endpoint-latest.zip
    rm arduino-nrf24-endpoint-latest.zip
    echo "[ok] "
else
    echo "[already present]"
fi
