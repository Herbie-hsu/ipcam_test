PWD		:= $(shell pwd)
ARCH		?= arm
CROSS_COMPILE	?= $(ARCH)-linux-
vKERNEL		?= 2.6.35.12
SNESOR_MODULE_NAME ?= SNX111
INSTALL_DIR	?= ../../../lib/modules/$(vKERNEL)/kernel/drivers
FIRMWARE_DIR	?= ../../../root/etc_default/firmware
KO_INSTALL_DIR	?= ../../../ko/isp2

EXTRA_CFLAGS 	+= -DCONFIG_VIDEO_SENSOR_$(SNESOR_MODULE_NAME)

SENSOR		:= $(shell echo $(SNESOR_MODULE_NAME) | tr A-Z a-z)

export ARCH CROSS_COMPILE

-include $(AUTOCONF_DIR)/config/snx_sdk.conf

INS_SENSOR		:= snx111

ifdef CONFIG_VIDEO_SENSOR_OV9715
INS_SENSOR		:= ov9715
endif

ifdef CONFIG_VIDEO_SENSOR_OV2715
INS_SENSOR		:= ov2715
endif

ifdef CONFIG_VIDEO_SENSOR_IMX132
INS_SENSOR		:= imx132
endif

ifdef CONFIG_VIDEO_SENSOR_AR0130
INS_SENSOR		:= ar0130
endif

ifdef CONFIG_VIDEO_SENSOR_IMX188
INS_SENSOR		:= imx188
endif

ifdef CONFIG_VIDEO_SENSOR_OV2659
INS_SENSOR		:= ov2659
endif

ifdef CONFIG_VIDEO_SENSOR_SNX111
INS_SENSOR		:= snx111
endif

ifdef CONFIG_VIDEO_SENSOR_OV9772
INS_SENSOR		:= ov9772
endif

ifdef CONFIG_VIDEO_SENSOR_IMX238
INS_SENSOR		:= imx238
endif

INS_FIRMWARE	:= IQ.bin
INS_MODULE 		:= snx_isp

OBJS = snx_isp_sensor.o snx_isp_ae.o snx_isp_af.o \
			snx_isp_awb.o snx_isp_ui.o snx_isp_md.o snx_isp_pm.o  \
				snx_isp_osd.o snx_isp_iq.o snx_isp_v4l2.o 

$(INS_MODULE)-objs := $(OBJS)

obj-m	:= $(INS_MODULE).o

modules:
	@ if [ -z "$(KERNEL_DIR)" ]; then \
		echo "Error! Please set kernel directory by 'KERNEL_DIR' variable"; \
		exit 1; \
	fi;
	@ if [ ! -d $(KO_INSTALL_DIR)/$(SENSOR) ]; \
	then \
		install -d $(KO_INSTALL_DIR)/$(SENSOR); \
	fi	
	$(MAKE) -C ./firmware
	$(MAKE) -C ./firmware install  KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR)) SNESOR_MODULE_NAME=$(SNESOR_MODULE_NAME)
	$(MAKE) -C ./sensor KERNEL_DIR=$(abspath $(KERNEL_DIR)) SNESOR_MODULE_NAME=$(SNESOR_MODULE_NAME)
	$(MAKE) -C ./sensor install KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR)) SNESOR_MODULE_NAME=$(SNESOR_MODULE_NAME)
	$(MAKE) -C $(KERNEL_DIR)/src M=$(PWD) modules
	install -c $(INS_MODULE).ko $(KO_INSTALL_DIR)/$(SENSOR)
	install -c Makefile $(KO_INSTALL_DIR)

.PHONY: clean
clean:
	$(MAKE) -C ./firmware clean
	$(MAKE) -C ./sensor clean
	-rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order

install:
	@ if [ ! -d $(FIRMWARE_DIR) ]; \
	then \
		install -d $(FIRMWARE_DIR); \
	fi
	@ if [ ! -d $(INSTALL_DIR) ]; \
	then \
		install -d $(INSTALL_DIR); \
	fi
	install -c $(abspath $(KO_INSTALL_DIR))/$(INS_SENSOR)/$(INS_FIRMWARE) $(FIRMWARE_DIR)
	install -c $(abspath $(KO_INSTALL_DIR))/$(INS_SENSOR)/$(INS_SENSOR).ko $(INSTALL_DIR)
	install -c $(abspath $(KO_INSTALL_DIR))/$(INS_SENSOR)/$(INS_MODULE).ko $(INSTALL_DIR)

uninstall:
	@ if [ ! -d $(FIRMWARE_DIR) ]; \
	then \
		install -d $(FIRMWARE_DIR); \
	fi
	@ if [ ! -d $(INSTALL_DIR) ]; \
	then \
		install -d $(INSTALL_DIR); \
	fi
	rm -rf	$(FIRMWARE_DIR)/$(INS_FIRMWARE)
	rm -rf  $(INSTALL_DIR)/$(INS_SENSOR).ko
	rm -rf  $(INSTALL_DIR)/$(INS_MODULE).ko

