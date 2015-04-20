PWD     	:= $(shell pwd)
TARGET      ?= arm-linux
CROSS_COMPILE   ?= $(TARGET)-
CPP     	:= $(CROSS_COMPILE)g++
CC      	:= $(CROSS_COMPILE)gcc
AR      	:= $(CROSS_COMPILE)ar
RANLIB      := $(CROSS_COMPILE)ranlib
INSTALL_DIR	?= ../rootfs/usr
BIN_INS_DIR	:= $(INSTALL_DIR)/bin/example
LIB_INS_DIR	:= $(INSTALL_DIR)/lib/example
CATEGORY	:= spi
INC     	= -I $(KERNEL_DIR)/src/include
INC			+= -I $(PWD)/include
CFLAGS      := -Wall -g -O2 $(EXTRA_CFLAGS) $(INC)
LDFLAGS     := $(EXTRA_LDFLAGS) -Wl,--as-needed



#targets = snx_spi_gpio snx_spi_ctl
targets = snx_spi_ctl

.PHONY : clean distclean all
%.o : %.c
	$(CC) $(CFLAGS) -c $<

all: $(targets)


snx_spi_gpio: snx_spi_gpio.o $(SNX_CRYPTO)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_spi_ctl: snx_spi_ctl.o $(SNX_CRYPTO)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

	
.PHONY: install
install:
	@ if [ ! -d $(BIN_INS_DIR)/$(CATEGORY) ]; \
	then \
	install -d $(BIN_INS_DIR)/$(CATEGORY); \
	fi
	install -c $(targets) $(BIN_INS_DIR)/$(CATEGORY)

clean:
	rm -f *.o *.yuv *.bak *.a *.out *.so $(targets)

distclean : clean
	rm -f $(targets)
