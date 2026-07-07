#!/bin/bash

vpath %.a	../../../rb/lib

%.r: %.a
	rma $< -o=$@

TARGET	= ../../../rb/lib/rblib.l

MATH	= add.r subtract.r multiply.r divide.r
IO	= printboolean.r printbyte.r printstring.r printinteger.r printreal.r \
	printcr.r inputboolean.r inputbyte.r inputstring.r inputinteger.r \
	inputreal.r iocommon.r
STR	= addstring.r copystring.r 
MISC	= compare.r
SYS	= bfinish.r shell.r

OBJS	= $(MATH) $(IO) $(STR) $(MISC) $(SYS)

all:	$(OBJS) ../../../rb/lib/rbstart.r
	cat $(OBJS) > $(TARGET)

clean:
	-rm $(OBJS) $(TARGET)

install: all
	cp $(TARGET) ~/relic
	cp ../../../rb/lib/rbstart.r ~/relic
