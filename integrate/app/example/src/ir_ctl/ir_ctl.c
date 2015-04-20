/**********************************************************************
 *
 * Function:    SPI to GPIO for IR-Cut/LED control program    
 *
 **********************************************************************/
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include "snx_gpio.h"

/* -------------------------------------------------------------------

NOTE: 
    snx_ir_ctl is an example application to describe how to control a 

    IR CUT. We take MS1_1, MS1_2 as the IR CUT P/N pins,  MS1_3 as the 

    Day/Night mode detection pin. You can modified them to fit your 

    spec. The detail GPIO control methods, please refer to the 

    GPIO example code.

----------------------------------------------------------------------*/
#define IR_CUT_P_PIN		1
#define IR_CUT_N_PIN		2
#define IR_CUT_DN_PIN		3

#define IR_DAY_MODE			1
#define IR_NIGHT_MODE		0

gpio_pin_info ircut_p, ircut_n, ir_dn;

int spi_gpio_init(void)
{
 	snx_ms1_gpio_open();
 	ircut_p.pinumber = IR_CUT_P_PIN;
 	ircut_p.mode 	 = 1; //output mode
 	ircut_p.value 	 = 1; //initialize
 	if (snx_ms1_gpio_write(ircut_p) == GPIO_FAIL)
    	printf ("msi%d gpio fail\n",ircut_p.pinumber); 

 	ircut_n.pinumber = IR_CUT_N_PIN;
 	ircut_n.mode 	 = 1; //output mode
 	ircut_n.value 	 = 1; //initialize
 	if (snx_ms1_gpio_write(ircut_n) == GPIO_FAIL)
    	printf ("msi%d gpio fail\n",ircut_n.pinumber); 

    ir_dn.pinumber 		= IR_CUT_DN_PIN;
 	ir_dn.mode 	 		= 0; //input mode
 	ir_dn.value 	 	= 0; 
 	if (snx_ms1_gpio_write(ir_dn) == GPIO_FAIL)
    	printf ("msi%d gpio fail\n",ir_dn.pinumber); 
	return 0;
}

int ir_dn_check(void)
{

	int ret;
	snx_ms1_gpio_read(&ir_dn);
	if (ir_dn.value == 1)
		ret = IR_DAY_MODE;
	else
		ret = IR_NIGHT_MODE;
	return ret;

}

int ir_cut_set(int op)
{

	if (op == IR_DAY_MODE) {
		ircut_p.value 	 = 1;
		ircut_n.value 	 = 0;
        printf("[IR-CTL] Day mode\n");
	} else if ( op == IR_NIGHT_MODE ) {
		ircut_p.value 	 = 0;
		ircut_n.value 	 = 1;
        printf("[IR-CTL] Night mode\n");
	} else {
		printf("[IR-Cut] Wrong operation!\n");
	}

	if (snx_ms1_gpio_write(ircut_p) == GPIO_FAIL)
		printf ("msi%d gpio fail\n",ircut_p.pinumber);
    if (snx_ms1_gpio_write(ircut_n) == GPIO_FAIL)
		printf ("msi%d gpio fail\n",ircut_n.pinumber);

	return 0;

}

int spi_gpio_close(void)
{

	snx_ms1_gpio_close();
	return 0;
}
/*
* main()
*/
int main(int argc,char *argv[])
{
	int dn;
	int pre_dn;
	dn = pre_dn = IR_DAY_MODE;
	spi_gpio_init();

	if (argc == 1) {
		while (1) {
		
			dn=ir_dn_check();
			if(dn != pre_dn) {
				ir_cut_set(dn);
			}
			pre_dn = dn;
			usleep(1000000); //wait one second for the next check
		}
	} else if (argc == 2) {

		if(atoi(argv[1]) == 0) {
	        ir_cut_set(IR_DAY_MODE);
	    } else if (atoi(argv[1]) == 1)
	        ir_cut_set(IR_NIGHT_MODE);
	     else
	     	printf("%s 0/1\n",argv[0]);
	} else {

		printf("%s [0/1]\n",argv[0]);
	}

	spi_gpio_close();

}

