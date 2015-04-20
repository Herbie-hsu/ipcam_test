deps_config := \
	driver_Kconfig \
	app_Kconfig \
	Kconfig

include/config/snx_sdk.conf: \
	$(deps_config)


$(deps_config): ;
