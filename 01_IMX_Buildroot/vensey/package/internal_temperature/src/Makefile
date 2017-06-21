##################################################################################################################
# Author:       Muersel Yildiz
# Date:         05-Jul-2015
# Description:  This makefile is used to generate the
#               internal temperature application binaries 
##################################################################################################################

SRC_DIR:=$(shell pwd)
build_date := $(shell date +%Y%m%d-%H:%M:%S)
ifeq (,$(wildcard .buildinfo))
build_info := BUILD_INFO=$(build_date)
else
build_info := $(shell echo "BUILD_INFO="`cat .buildinfo`",$(build_date)")
endif

INCLUDES += -I./ -I$(SRC_DIR)/../include
LDFLAGS = -lpthread $(INCLUDES) `pkg-config --libs $(packages)`

internal_temperature: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

OBJECTS := \
	$(patsubst %.c,%.o,$(wildcard *.c)) \
	$(patsubst %.c,%.o,$(wildcard ../include/*.c)) 

BINARY := internal_temperature

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS):
	$(CC) $(CFLAGS) $(INCLUDES) -c ${@:.o=.c} -o $@

target := internal_temperature 

all: $(BINARY) $(target)


$(target): $(OBJECTS)
	$(CC) -o $@  $(CFLAGS) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
 
