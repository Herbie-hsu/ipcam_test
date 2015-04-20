PWD     	:= $(shell pwd)
TARGET      ?= arm-linux
CROSS_COMPILE   ?= $(TARGET)-
CPP     	:= $(CROSS_COMPILE)g++
CC      	:= $(CROSS_COMPILE)gcc
AR      	:= $(CROSS_COMPILE)ar
RANLIB      := $(CROSS_COMPILE)ranlib

LIBINSTALLDIR = $(INSTALL_DIR)/lib

INSTALL = install

CFLAGS		= -Os $(APP_FLAGS)

SNX_EZ_LIB	= libsnx_ez.so

CFLAGS		+= -I$(MIDDLEWARE_INS_DIR)/include

#LDFLAGS	+= $(MIDDLEWARE_INS_DIR)/lib/libnvram.so
LDFLAGS 	+= $(SNX_EZ_LIB)


targets = $(SNX_EZ_LIB)

all: $(targets)

$(SNX_EZ_LIB): snx_ez_lib.o
	$(CC) -shared -o $@ $^
	
clean:
	rm -f *.o $(SNX_EZ_LIB)

install:
	$(INSTALL) -d $(LIBINSTALLDIR)
	$(INSTALL) $(SNX_EZ_LIB) $(LIBINSTALLDIR)


