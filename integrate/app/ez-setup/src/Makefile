
PACKAGE		= snx-ez-lib
PACKAGE		+= ez-setup
PACKAGE		+= pcm_play

MULTI_VIDEO		= 0
SOUND_DEBUG		= 1
WIFI_CHECK		= 0
### WEP=0 / WPA=1 / WPA2=2
WIFI_ENCRYPT		= 2
CMD_ENC			= 1

########################################################################

INSTALL_DIR     ?= ../rootfs

APP_FLAGS	+= -DMULTI_VIDEO=$(MULTI_VIDEO)
APP_FLAGS	+= -DWIFI_ENCRYPT=$(WIFI_ENCRYPT)
APP_FLAGS	+= -DSOUND_FOR_DEBUG=$(SOUND_DEBUG)
APP_FLAGS 	+= -DWIFI_CHECK_DEBUG=$(WIFI_CHECK)
APP_FLAGS	+= -DCMD_ENC=$(CMD_ENC)
#APP_FLAGS 	+= -g

.PHONY:	clean all install

all:
#	@ if [ "$(findstring 1,$(CMD_ENC))" ]; then \
#		cp target/include/cloudtalk/server7_client.h target/include/cloudtalk/client.h; \
#		cp target/lib/server7_libcloudtalk.a target/lib/libcloudtalk.a; \
#		cp target/lib/server7_libcloudmedia.a target/lib/libcloudmedia.a; \
#	else \
#		cp target/include/cloudtalk/server6_client.h target/include/cloudtalk/client.h; \
#		cp target/lib/server6_libcloudtalk.a target/lib/libcloudtalk.a; \
#		cp target/lib/server6_libcloudmedia.a target/lib/libcloudmedia.a; \
#	fi;


	@ for dir in $(PACKAGE); do $(MAKE) -C  $$dir APP_FLAGS="$(APP_FLAGS)" || exit 1; done


install: 
	@ for dir in $(PACKAGE); \
	do $(MAKE) -C $$dir APP_360="$(APP_360)" WIFI_ENCRYPT="$(WIFI_ENCRYPT)"  install \
	|| exit 1; done

clean: 
	@ for dir in $(PACKAGE); do $(MAKE) -C $$dir clean || exit 1; done
	rm -f ./target/sbin/*
