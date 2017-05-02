# -*- mode: makefile; coding: utf-8 -*-

all:

install: clean
	python3 setup.py install \
	--prefix=$(DESTDIR)/usr/ \
	--no-compile \
        --install-layout=deb

	$(RM) -fr ./debian/ice-nrf24/usr/lib/python3/dist-packages/nRF24Endpoint/__pycache__

.PHONY: clean
clean:
	find -name "*.pyc" -print -delete
	find -name "*~" -print -delete

	$(RM) -rf build
	$(MAKE) -C ArduinoFirmware clean
	$(MAKE) -C examples/rover-client/server clean
	$(MAKE) -C examples/rover-server/client clean
