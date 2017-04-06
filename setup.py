#!/usr/bin/python3
# -*- mode: python3; coding: utf-8 -*-

from distutils.core import setup


def get_version():
    with open('debian/changelog', errors="ignore") as chlog:
        return chlog.readline().split()[1][1:-1]


setup(
    name          = 'ice-nrf24',
    version       =  get_version(),
    description   = "ZeroC Ice endpoint using a nRF24 and Arduino",
    author        = "Oscar Acena",
    author_email  = "oscaracena@gmail.com",
    url           = "https://bitbucket.org/arco_group/ice.nrf24-endpoint",

    packages      = ['nRF24Endpoint'],
)
