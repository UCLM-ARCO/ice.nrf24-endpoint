#!/usr/bin/python3
# -*- mode: python3; coding: utf-8 -*-

import sys
import Ice
from time import sleep

Ice.loadSlice("rover.ice")
import Example


class Client(Ice.Application):
    def run(self, args):
        rover = "Rover -d:nrf24 -c 100 -a rover"
        if len(args) > 1:
            rover = args[1]

        ic = self.communicator()
        rover = ic.stringToProxy(rover)
        rover = Example.RoverPrx.uncheckedCast(rover)

        self.move_rover(rover)

    def move_rover(self, rover):
        while True:
            rover.forward()
            return
            sleep(1)
            rover.left()
            sleep(0.5)


if __name__ == "__main__":
    Client().main(sys.argv, "client.config")
