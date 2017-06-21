##################################################################################################################
# Author:       Muersel Yildiz
# Date:         21-Mar-2017
# Description:  This makefile is used to generate the
#     		vensey_test_c  application binaries 
##################################################################################################################

SRC_DIR:=$(shell pwd)
build_date := $(shell date +%Y%m%d-%H:%M:%S)
ifeq (,$(wildcard .buildinfo))
build_info := BUILD_INFO=$(build_date)
else
build_info := $(shell echo "BUILD_INFO="`cat .buildinfo`",$(build_date)")
endif

packages := gthread-2.0  
INCLUDES += -I./ -I$(SRC_DIR)/../include -I$(PKG_CONFIG_PATH) `pkg-config --cflags $(packages)`
LDFLAGS += -lpthread $(INCLUDES) `pkg-config --libs $(packages)`
CFLAGS +=  -g -Wall \
          -DBUILD_INFO=\"$(build_info)\" \
          $(ALL_INC:%=-I%)


vensey_test_c: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

OBJECTS := \
	$(patsubst %.c,%.o,$(wildcard *.c)) \
	$(patsubst %.c,%.o,$(wildcard ../include/*.c)) 

BINARY := vensey_test_c

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJECTS):
	$(CC) $(CFLAGS) $(INCLUDES) -c ${@:.o=.c} -o $@

target := vensey_test_c 

all: $(BINARY) $(target)


$(target): $(OBJECTS)
	$(CC) -o $@  $(CFLAGS) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
 
