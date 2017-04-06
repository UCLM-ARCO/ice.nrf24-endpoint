#!/usr/bin/python3
# -*- mode: python3; coding: utf-8 -*-

import sys
import Ice

Ice.loadSlice("rover.ice")
import Example


class RoverI(Example.Rover):
    def forward(self, current):
        print("forward")

    def backward(self, current):
        print("backward")

    def left(self, current):
        print("left")

    def right(self, current):
        print("right")

    def stop(self, current):
        print("stop")


class Server(Ice.Application):
    def run(self, args):
        ic = self.communicator()

        adapter = ic.createObjectAdapter("Adapter")
        adapter.activate()

        oid = ic.stringToIdentity("Rover")
        proxy = adapter.add(RoverI(), oid).ice_datagram()
        print("Proxy ready: '{}'".format(proxy))

        self.shutdownOnInterrupt()
        ic.waitForShutdown()


if __name__ == "__main__":
    Server().main(sys.argv, "server.config")
