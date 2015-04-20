INSTALL_DIR	?= ../rootfs/usr
BIN_INS_DIR	:= $(INSTALL_DIR)/bin/example
LIB_INS_DIR	:= $(INSTALL_DIR)/lib/example

targets = snx_pwm_period_test snx_pwm_ap_test
PWM	:= pwm
all:
	arm-linux-gcc -o snx_pwm_period_test snx_pwm_period_test.c
	arm-linux-gcc -o snx_pwm_ap_test snx_pwm_ap_test.c	

.PHONY: clean
clean:
	-rm -rf *.o  snx_pwm_period_test snx_pwm_ap_test

.PHONY: install
install:
	@ if [ ! -d $(BIN_INS_DIR)/$(PWM) ]; \
	then \
		install -d $(BIN_INS_DIR)/$(PWM); \
	fi
	install -c $(targets) $(BIN_INS_DIR)/$(PWM)

