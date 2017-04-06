# -*- mode: makefile-gmake; coding: utf-8 -*-

SLICE = rover.ice

all: ${SLICE:%.ice=%.js}

%.js: %.ice
	slice2js $^

.PHONY: clean
clean:
	$(RM) ${SLICE:%.ice=%.js}
