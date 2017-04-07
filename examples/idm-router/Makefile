# -*- mode: makefile; coding: utf-8 -*-

SHELL = /bin/bash

all:

run-router:
	source set_env.sh; idm-router --Ice.Config=router.config

http-server:
	cd client; python3 -m http.server
