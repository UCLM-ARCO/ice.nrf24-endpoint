# -*- mode: python3; coding: utf-8 -*-

import os
import serial
import uuid

import pyendpoint as pe

# pe.log.activated = True
ICE_NRF24_TYPE = 15


def show_trace(fn):
    def deco(*args, **kwargs):
        pe.trace()
        return fn(*args, **kwargs)
    return deco


class EndpointInfo:
    @show_trace
    def __init__(self, options):
        self.channel = None
        self.address = None

        try:
            self.channel = options[options.index('-c') + 1]
            self.address = options[options.index('-a') + 1]
            assert self.channel is not None and self.address is not None
        except (ValueError, KeyError, AssertionError):
            raise RuntimeError(
                "Invalid endpoint format: '{}'".format(" ".join(options)))

    @show_trace
    def __str__(self):
        return ""


class EndpointFactoryI(pe.EndpointFactoryI):
    @show_trace
    def __init__(self, communicator):
        pe.EndpointFactoryI.__init__(self, communicator)
        self._type = ICE_NRF24_TYPE
        self._protocol = "nrf24"

    @show_trace
    def create(self):
        return EndpointI(self._communicator, self._protocol)

#     @show_trace
#     def read(self, stream):
#         # remove encapsulation info
#         stream.startReadEncaps()
#         stream.endReadEncaps()
#
#         endp = self.create()
#         return endp


class EndpointI(pe.EndpointI):
    @show_trace
    def __init__(self, communicator, protocol):
        pe.EndpointI.__init__(self, communicator, protocol)
        self._info = None
        self._type = ICE_NRF24_TYPE

    @show_trace
    def initWithOptions(self, args, oaEndpoint):
        self._info = EndpointInfo(args)
        self._oaEndpoint = oaEndpoint

    @show_trace
    def options(self):
        return str(self._info)

    @show_trace
    def datagram(self):
        return True

#     # required by IceGrid
#     @show_trace
#     def streamWrite(self, stream):
#         # add encapsulation header
#         stream.startWriteEncaps()
#         stream.endWriteEncaps()
#
#     @show_trace
#     def transceiver(self):
#         return TransceiverI(self._communicator, self._protocol, self._info)
#
#     @show_trace
#     def endpoint(self, trans):
#         args = str(self._info).split()
#         endp = EndpointI(self._communicator, self._protocol)
#         endp.initWithOptions(args, self._oaEndpoint)
#         return endp
#
#     @show_trace
#     def equivalent(self, other):
#         return self._type == other._type

    @show_trace
    def connectors_async(self, callback):
        connectors = [ConnectorI(self._communicator, self._protocol, self._info)]
        callback.connectors(connectors)

    # required by IceGrid
    @show_trace
    def toString(self):
        return "{} {}".format(self._protocol, str(self._info)).strip()


class ConnectorI(pe.ConnectorI):
    @show_trace
    def __init__(self, communicator, protocol, info):
        pe.ConnectorI.__init__(self, communicator, protocol)
        self._info = info
        self._type = ICE_NRF24_TYPE

    @show_trace
    def connect(self):
        return TransceiverI(self._communicator, self._protocol, self._info, True)

    @show_trace
    def toString(self):
        return str(self._info)


class nRF24Bridge:
    MAX_PAYLOAD_SIZE = 256
    NBP_MAGIC = b"NBP"
    CMD_SET_CHANNEL = b'1'
    CMD_SET_ADDRESS = b'2'
    CMD_SET_DESTINATION = b'3'
    CMD_SEND = b'4'

    def __init__(self, path, speed, channel, address):
        self._device = serial.Serial(path, speed)
        self.set_channel(channel)

        if address is not None:
            self.set_address(address)

    def set_channel(self, channel):
        channel = int(channel)
        assert (channel >= 1 and channel <= 125), "Invalid channel, not in [1, 125]"

        cmd = self.NBP_MAGIC + self.CMD_SET_CHANNEL + bytes([channel])
        self._device.write(cmd)
        self._device.flush()

    def set_address(self, address):
        assert len(address) <= 5, "nRF24 address should be 5 bytes or less"
        address = bytes(address, "utf-8")
        address += b'\x00' * (5 - len(address))

        cmd = self.NBP_MAGIC + self.CMD_SET_ADDRESS + address
        self._device.write(cmd)
        self._device.flush()

    def set_destination(self, dst):
        assert len(dst) <= 5, "nRF24 destination should be 5 bytes or less"
        dst = bytes(dst, "utf-8")
        dst += b'\x00' * (5 - len(dst))

        cmd = self.NBP_MAGIC + self.CMD_SET_DESTINATION + dst
        self._device.write(cmd)
        self._device.flush()

    def send(self, data):
        assert len(data) <= self.MAX_PAYLOAD_SIZE, "Message is too big!"

        cmd = self.NBP_MAGIC  + self.CMD_SEND
        cmd += bytes([(len(data) >> 8) & 0xff, len(data) & 0xff])
        cmd += data

        print("send: {} bytes".format(len(data)))
        self._device.write(cmd)
        self._device.flush()
        return len(data)

    def get_serial_fd(self):
        return self._device.fileno()

    def close(self):
        self._device.flush()
        self._device.close()


class TransceiverI(pe.TransceiverI):
    @show_trace
    def __init__(self, communicator, protocol, info, connect=False):
        pe.TransceiverI.__init__(self, communicator, protocol, connect)

        props = communicator.getProperties()
        path = props.getProperty(
            "nRF24Endpoint.Device")
        speed = int(props.getPropertyWithDefault(
            "nRF24Endpoint.Speed", "115200"))

        if not path:
            raise ValueError("Invalid config: nRF24Endpoint.Device not defined")

        if not os.path.exists(path):
            raise ValueError("Invalid config: device '{}' does not exist"
                             .format(path))

        address = None if connect else info.address
        self._device = nRF24Bridge(path, speed, info.channel, address)

        self._fd = self._device.get_serial_fd()
        self._incoming = not connect
        self._info = info
        self._connectionId = uuid.uuid4().hex

    # NOTE: this is needed by Ice.Admin object adapter, used in IceGrid
    @show_trace
    def getInfo(self):
        adapterName = ""
        return pe.ConnectionInfo(self._incoming, adapterName, self._connectionId)

    @show_trace
    def toString(self):
        return "{}{}".format(self._protocol, str(self._info)).strip()

    @show_trace
    def toDetailedString(self):
        return self.toString()

    @show_trace
    def write(self, buff):
        self._device.set_destination(self._info.address)
        return self._device.send(buff)

    @show_trace
    def read(self, buff):
        print(" ## READ called")
        self._device.read()
        return True, 0

        # message = self.read_message()
        # buff[:len(message)] = message[:]
        # return True, len(message)

    @show_trace
    def close(self):
        self._device.close()

#     def read_message(self):
#         data = ""
#         while True:
#             c = self._device.read()
#             data += c
#             if len(data) < 14:
#                 continue
#
#             header = "IceP\1\0\1"
#             if data.startswith(header):
#                 break
#
#             data = data[1:]
#
#         size = ord(data[10])
#         size += ord(data[11]) << 8
#         size += ord(data[11]) << 16
#         size += ord(data[11]) << 24
#
#         data += self._device.read(size - 14)
#         return data
