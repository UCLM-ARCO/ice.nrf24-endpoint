Description
===========

This is an endpoint for ZeroC Ice using an Arduino, which also uses a
nRF24 for communications. The Arduino is connected as a serial device
(usually ``/dev/ttyUSB0``). The endpoint type is `nrf24`, and it has
two parameters, `-c`, which is the channel for the nRF24, and `-a` for
the node address. A stringfied proxy should look like the following:

    Light -e 1.0 -d:nrf24 -c 100 -a node1

See the `examples` dir for more information.

Hardware
========

The Arduino device should have the firmware located on the
`ArduinoFirmware` folder.
