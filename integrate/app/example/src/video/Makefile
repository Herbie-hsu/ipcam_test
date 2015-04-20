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
CATEGORY	:= video

INC     	= -I $(KERNEL_DIR)/src/include
INC     	+= -I$(MIDDLEWARE_INS_DIR)
INC     	+= -I$(MIDDLEWARE_INS_DIR)/include/snx_vc
INC			+= -I$(MIDDLEWARE_INS_DIR)/include/snx_rc
INC			+= -I $(PWD)/include
CFLAGS      := -Wall -g -O2 $(EXTRA_CFLAGS) $(INC)
LDFLAGS     := $(EXTRA_LDFLAGS) -Wl,--as-needed
LDFLAGS 	+= -pthread

SNX_VC_LIB = $(MIDDLEWARE_INS_DIR)/lib/libsnx_vc.so
SNX_RC_LIB = $(MIDDLEWARE_INS_DIR)/lib/libsnx_rc.so $(MIDDLEWARE_INS_DIR)/lib/libsnx_isp.so $(MIDDLEWARE_INS_DIR)/lib/libsnx_common.so

SNX_VIDEO_CODEC = snx_video_codec.o

targets = snx_m2m_one_stream snx_m2m_two_stream snx_m2m_capture_2stream snx_m2m_capture_4stream
targets += snx_m2m_one_stream_with_rc snx_m2m_capture_2stream_with_rc
targets += snx_vc_ds
#targets += snx_one_stream_rc_live555

.PHONY : clean distclean all
%.o : %.c
	$(CC) $(CFLAGS) -c $<

all: $(targets)
#	$(MAKE) -C live $@  INSTALL_DIR=$(abspath $(BIN_INS_DIR))

#$(SNX_VC_LIB): $(MIDDLEWARE_INS_DIR)/../video/src/snx-vc-lib/snx_vc_lib.c
#	$(CC) $(CFLAGS) -c -o snx_vc_lib.o $<
#	$(CC) $(CFLAGS) -shared -o $@ snx_vc_lib.o

#$(SNX_RC_LIB): $(MIDDLEWARE_INS_DIR)/../rate_ctl/src/snx_rc_lib.c
#	$(CC) $(CFLAGS) -c -o snx_rc_lib.o $<
#	$(CC) $(CFLAGS) -shared -o $@ snx_rc_lib.o

$(SNX_VIDEO_CODEC): vc/snx_video_codec.c
	$(CC) $(CFLAGS) -c -o $@ $<

snx_m2m_one_stream: snx_m2m_one_stream.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_m2m_two_stream: snx_m2m_two_stream.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_m2m_capture_2stream: snx_m2m_capture_2stream.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_m2m_capture_4stream: snx_m2m_capture_4stream.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_m2m_one_stream_with_rc: snx_m2m_one_stream_with_rc.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_m2m_capture_2stream_with_rc: snx_m2m_capture_2stream_with_rc.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_vc_ds: snx_vc_ds.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

snx_one_stream_rc_live555: snx_one_stream_rc_live555.o $(SNX_VIDEO_CODEC) $(SNX_VC_LIB) $(SNX_RC_LIB)
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

.PHONY: install
install:
	@ if [ ! -d $(BIN_INS_DIR)/$(CATEGORY) ]; \
	then \
	install -d $(BIN_INS_DIR)/$(CATEGORY); \
	fi
	install -c $(targets) $(BIN_INS_DIR)/$(CATEGORY)
	#$(MAKE) -C live $@  INSTALL_DIR=$(abspath $(BIN_INS_DIR))

clean:
	rm -f *.o *.yuv *.bak *.a *.out *.so $(targets)
	#$(MAKE) -C live $@  INSTALL_DIR=$(abspath $(BIN_INS_DIR))

distclean : clean
	rm -f $(targets)
