PWD		:= $(shell pwd)
MAKE            ?= make
INSTALL_DIR	?= $(PWD)/../../
FIRMWARE_DIR	?= $(PWD)/../../
vKERNEL		?= 2.6.35.12
DRIVER_INS_DIR	:= $(INSTALL_DIR)/lib/modules/$(vKERNEL)/kernel/drivers
FIRMWARE_INS_DIR	:= $(INSTALL_DIR)/root/etc_default/firmware
KO_INSTALL_DIR	:= ../../ko

SENSORS = SNX111 OV9715 OV2715 OV2659 OV9772 AR0130 IMX132 IMX188 IMX238

ISP2	= isp2
VC2		= vc2

.PHONY: all clean install
all:
	@ for sensor in $(SENSORS); do $(MAKE) -C $(ISP2) KERNEL_DIR=$(abspath $(KERNEL_DIR)) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/isp2 SNESOR_MODULE_NAME=$$sensor || exit 1; done
	$(MAKE) -C $(VC2) KERNEL_DIR=$(abspath $(KERNEL_DIR)) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/vc2 || exit 1
	install -c Makefile $(KO_INSTALL_DIR)

clean: 
	@ for sensor in $(SENSORS); do $(MAKE) -C $(ISP2) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/isp2 SNESOR_MODULE_NAME=$$sensor $@ || exit 1; done
	$(MAKE) -C $(VC2) $@ || exit 1

install:
	@ for sensor in $(SENSORS); do ss="$$(echo $$sensor | tr A-Z a-z)".ko; rm -rf $(DRIVER_INS_DIR)/$$ss || exit 1; done
	$(MAKE) -C $(ISP2) $@ INSTALL_DIR=$(abspath $(DRIVER_INS_DIR)) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/isp2 FIRMWARE_DIR=$(abspath $(FIRMWARE_INS_DIR)) || exit 1
	$(MAKE) -C $(VC2) $@ INSTALL_DIR=$(abspath $(DRIVER_INS_DIR)) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/vc2 || exit 1
	
uninstall:
	$(MAKE) -C $(ISP2) $@ INSTALL_DIR=$(abspath $(DRIVER_INS_DIR)) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/isp2 FIRMWARE_DIR=$(abspath $(FIRMWARE_INS_DIR)) || exit 1
	$(MAKE) -C $(VC2) $@ INSTALL_DIR=$(abspath $(DRIVER_INS_DIR)) KO_INSTALL_DIR=$(abspath $(KO_INSTALL_DIR))/vc2 || exit 1
