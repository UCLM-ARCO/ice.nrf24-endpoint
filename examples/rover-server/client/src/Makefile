# -*- mode: makefile-gmake; coding: utf-8 -*-

SLICE = rover.ice

all: ${SLICE:%.ice=%.h}

%.h: %.ice
	slice2c $^

.PHONY: clean
clean:
	$(RM) ${SLICE:%.ice=%.h}
