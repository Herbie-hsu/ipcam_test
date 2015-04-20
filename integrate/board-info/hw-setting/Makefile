# Read in config

AUTOCONF_DIR	?= ../../buildscript/include
-include $(AUTOCONF_DIR)/config/snx_sdk.conf

VER_HW ?= $(if $(CONFIG_SYSTEM_PREVIOUS_PLATFORM),ver)
ifeq ($(VER_HW),ver)
	VER_WHCH = $(CONFIG_SYSTEM_PREVIOUS_VERSION)
else
	VER_WHCH = none
endif

WHICH_VALUE =

.PHONY: sdk keil jlink sdk-f quake-h
sdk:
	perl hw_setting_convert.pl $(VER_WHCH) ./sn986xx sdk

keil:
	perl hw_setting_convert.pl $(VER_WHCH) ./sn986xx keil
	
jlink:
	perl hw_setting_convert.pl $(VER_WHCH) ./sn986xx jlink
	
sdk-f:
	perl hw_setting_convert.pl $(VER_WHCH) ./sn986xx sdk -f $(WHICH_VALUE)

quake-h:
	perl hw_setting_convert.pl $(VER_WHCH) ./sn986xx quake -h $(WHICH_VALUE)
	
PHONY += hwsetting-clean
hwsetting-clean:
	rm -rf ./sdk ./quake ./keil ./jlink ./sdk_$(VER_WHCH) ./quake_$(VER_WHCH) ./keil_$(VER_WHCH) ./jlink_$(VER_WHCH)
