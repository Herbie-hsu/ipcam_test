PWD     	:= $(shell pwd)
TARGET		?= arm-linux
CROSS_COMPILE   ?= $(TARGET)-
CPP     	:= $(CROSS_COMPILE)g++
CC      	:= $(CROSS_COMPILE)gcc
AR      	:= $(CROSS_COMPILE)ar
RANLIB		:= $(CROSS_COMPILE)ranlib

BIN_DIR		= $(INSTALL_DIR)/bin
#SNIP39_DIR	= $(INSTALL_DIR)/root/etc_default/hostapd
SNIP39_DIR	= $(INSTALL_DIR)/root/etc_default/SNIP39
DEF_DIR		= files

INSTALL		= install

######################################################
SSID_NEW	= 36099990220
SSID_KEY_NEW	= zzzz8998
# 10.42.0.100 ~ 10.42.0.251
IP_RANGE_NEW	= 10.42.0.

SSID_OLD	= SNIP39_AAINAMMK
SSID_KEY_OLD	= 2mpAzUUmsU77q
# 5.5.1.100 ~ 5.5.1.251
IP_RANGE_OLD	= 5.5.1.
######################################################

DEF_CONF	= default.conf
UID_CONF	= SNIP39_UID.conf

DHCPD 		= udhcpd.conf
WPA_SUPPLICANT	= wpa_supplicant.conf

ifeq ($(WIFI_ENCRYPT), 0)
HOSTAP		= hostapd_wep.conf
endif
ifeq ($(WIFI_ENCRYPT), 1)
HOSTAP		= hostapd_wpa.conf
endif
ifeq ($(WIFI_ENCRYPT), 2)
HOSTAP		= hostapd_wpa2.conf
endif

CFLAGS		= -Os $(APP_FLAGS) -I../snx-ez-lib

SNX_EZ_LIB	= libsnx_ez.so

APSTA_OBJS	= snx_ez.o
SNX_RST_OBJS	= snx_rst.o

LDFLAGS		+= ../snx-ez-lib/$(SNX_EZ_LIB)

CFLAGS 		+= -I$(MIDDLEWARE_INS_DIR)/include
LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libnvram.so
LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libzbar.so

LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libsnx_vc.so
LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libsnx_rc.so
LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libsnx_isp.so
LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libsnx_common.so
LDFLAGS		+= $(MIDDLEWARE_INS_DIR)/lib/libzbar.so.0.2.0

LDFLAGS 	+= ../../../../toolchain/crosstool-4.5.2/arm-linux/sysroot/usr/lib/libssl.so.1.0.0
LDFLAGS 	+= ../../../../toolchain/crosstool-4.5.2/arm-linux/sysroot/usr/lib/libcrypto.so.1.0.0

targets = snx_ez snx_rst snx_scan

all: $(targets)

snx_scan: 
	arm-linux-gcc -o snx_scan snx_scan.c get_frame_isp.c -I$(MIDDLEWARE_INS_DIR)/include/ -L$(MIDDLEWARE_INS_DIR)/lib/ -lzbar
#	arm-linux-gcc -o snx_scan snx_scan.c get_frame_isp.c $(CFLAGS) $(LDFLAGS)

snx_ez: $(APSTA_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(APSTA_OBJS) -o $@ 

snx_rst: $(SNX_RST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SNX_RST_OBJS) -o $@ 
	
clean:
	rm -f *.o $(targets) $(SNX_EZ_LIB)

install:
	$(INSTALL) -d $(BIN_DIR)
	$(INSTALL) snx_ez $(BIN_DIR)
	$(INSTALL) snx_rst $(BIN_DIR)
	$(INSTALL) snx_scan $(BIN_DIR)

	$(INSTALL) -d $(SNIP39_DIR)
	$(INSTALL) $(DEF_DIR)/$(HOSTAP) $(SNIP39_DIR)/$(HOSTAP)
	$(INSTALL) $(DEF_DIR)/$(DHCPD) $(SNIP39_DIR)/$(DHCPD)
	$(INSTALL) $(DEF_DIR)/$(WPA_SUPPLICANT) $(SNIP39_DIR)/$(WPA_SUPPLICANT)
	$(INSTALL) $(DEF_DIR)/$(DEF_CONF) $(SNIP39_DIR)/$(DEF_CONF)
	$(INSTALL) $(DEF_DIR)/$(UID_CONF) $(SNIP39_DIR)/$(UID_CONF)
#	cp $(DEF_SNIP39_DIR)/$(HOSTAP) $(SNIP39_DIR)/$(HOSTAP)

ifeq ($(APP_360), 1)
	sed -i 's/$(SSID_OLD)/$(SSID_NEW)/g' $(SNIP39_DIR)/$(HOSTAP)
	sed -i 's/$(SSID_KEY_OLD)/$(SSID_KEY_NEW)/g' $(SNIP39_DIR)/$(HOSTAP)
	sed -i 's/$(SSID_KEY_OLD)/$(SSID_KEY_NEW)/g' $(SNIP39_DIR)/$(HOSTAP)
	sed -i 's/$(IP_RANGE_OLD)/$(IP_RANGE_NEW)/g' $(SNIP39_DIR)/$(DHCPD)
endif



