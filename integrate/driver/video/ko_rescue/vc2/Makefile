PWD			:= $(shell pwd)
ARCH			?= arm
CROSS_COMPILE		?= $(ARCH)-linux-
vKERNEL			?= 2.6.35.12
INSTALL_DIR		?= ../../../lib/modules/$(vKERNEL)/kernel/drivers
KO_INSTALL_DIR	?= ../../../ko/vc2
EXTRA_CFLAGS		+= -O2 -DUSE_DEF_FONT=0 -I$(AUTOCONF_DIR)

export ARCH CROSS_COMPILE

OBJS_ISP 		:= dummy.o
OBJS_ISP2 		:= dummy2.o

OBJS_CODEC 		:= snx_vid_intr.o
OBJS_CODEC 		+= snx_ic_datastamp.o
OBJS_CODEC 		+= snx_ic.o snx_h264.o
OBJS_CODEC 		+= snx_mjpeg.o
OBJS_CODEC 		+= snx_vid_codec.o

OBJS_SNX_VB2		:= snx_videobuf2.o

MODULE_ISP		= snx_dummy
MODULE_ISP2		= snx_dummy2
MODULE_CODEC		= snx_vc
MODULE_SNX_VB2		= snx_vb2

MODULES			= $(MODULE_CODEC).ko $(MODULE_SNX_VB2).ko

$(MODULE_ISP)-objs	:= $(OBJS_ISP)
$(MODULE_ISP2)-objs	:= $(OBJS_ISP2)
$(MODULE_CODEC)-objs	:= $(OBJS_CODEC)
$(MODULE_SNX_VB2)-objs	:= $(OBJS_SNX_VB2)

obj-m			:= $(MODULE_CODEC).o $(MODULE_SNX_VB2).o

ifeq ($(DUMMY_ISP),1)

obj-m			+= $(MODULE_ISP).o $(MODULE_ISP2).o
MODULES			+= $(MODULE_ISP).ko $(MODULE_ISP2).ko
endif

modules:	
	@ if [ -z "$(KERNEL_DIR)" ]; then \
		echo "Error! Please set kernel directory by 'KERNEL_DIR' variable"; \
		exit 1; \
	fi;
	@ if [ ! -d $(KO_INSTALL_DIR) ]; \
	then \
		install -d $(KO_INSTALL_DIR); \
	fi	
	$(MAKE) -C $(KERNEL_DIR)/src M=$(PWD) modules
	install -c $(MODULE_CODEC).ko $(KO_INSTALL_DIR)
	install -c $(MODULE_SNX_VB2).ko $(KO_INSTALL_DIR)
	install -c Makefile $(KO_INSTALL_DIR)

.PHONY: clean
clean:
	-rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order

install:
	@ if [ ! -d $(INSTALL_DIR) ]; \
	then \
		install -d $(INSTALL_DIR); \
	fi
	install -c $(abspath $(KO_INSTALL_DIR))/$(MODULE_CODEC).ko $(INSTALL_DIR)
	install -c $(abspath $(KO_INSTALL_DIR))/$(MODULE_SNX_VB2).ko $(INSTALL_DIR)

uninstall:
	@ if [ ! -d $(INSTALL_DIR) ]; \
	then \
		install -d $(INSTALL_DIR); \
	fi
	rm -rf  $(INSTALL_DIR)/$(MODULE_CODEC).ko
	rm -rf  $(INSTALL_DIR)/$(MODULE_SNX_VB2).ko
