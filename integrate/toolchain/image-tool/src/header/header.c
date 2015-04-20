#include "common.h"
#include "ms_nf.h"

u32 FLASH_INFO_START;	
u32 HW_SETTING_START;   
u32 U_BOOT_START;       
u32 U_ENV_START;        
u32 FLASH_LAYOUT_START; 
u32 FACTORY_START;      
u32 U_LOGO_START;       
u32 RESCUE_START;
u32 AHB2_CLOCK;

#define SNX_SYS_BASE			0x98000000

static get_clock ()
{
	u32 pll800=0, ahb2_rate=0;

	pll800 = ((* (volatile unsigned int *) SNX_SYS_BASE) & 0x3f8000) >> 15;
	ahb2_rate = ((* (volatile unsigned int *) (SNX_SYS_BASE + 4)) & 0xf8000) >> 15;
		
	AHB2_CLOCK = (pll800 * 12 * 1000000) / ahb2_rate;
	
	return (0);
}

int main(void)
{	 
	FLASH_INFO_START		=	_FLASH_INFO_START		+ _TEXT_BASE;
	HW_SETTING_START        =	_HW_SETTING_START       + _TEXT_BASE;
	U_BOOT_START            =	_U_BOOT_START           + _TEXT_BASE;
	U_ENV_START             =	_U_ENV_START            + _TEXT_BASE;
	FLASH_LAYOUT_START      =	_FLASH_LAYOUT_START     + _TEXT_BASE;
	FACTORY_START           =	_FACTORY_START          + _TEXT_BASE;
	U_LOGO_START            =	_U_LOGO_START           + _TEXT_BASE;
	RESCUE_START            =	_RESCUE_START           + _TEXT_BASE;

	
	
	serial_puts("----UPDATE FIRMWARE----\n");
	
	serial_printf("FLASH_INFO_START = 0x%x\n", FLASH_INFO_START);
	serial_printf("HW_SETTING_START = 0x%x\n", HW_SETTING_START);
	serial_printf("U_BOOT_START = 0x%x\n", U_BOOT_START);
	serial_printf("U_ENV_START = 0x%x\n", U_ENV_START);
	serial_printf("FLASH_LAYOUT_START = 0x%x\n", FLASH_LAYOUT_START);
	serial_printf("FACTORY_START = 0x%x\n", FACTORY_START);
	serial_printf("U_LOGO_START = 0x%x\n", U_LOGO_START);
	serial_printf("RESCUE_START = 0x%x\n", RESCUE_START);
	
	serial_printf("_TEXT_BASE = 0x%x\n", _TEXT_BASE);
	serial_printf("_bss_start = 0x%x\n", _bss_start);
	serial_printf("_bss_end = 0x%x\n", _bss_end);

	get_clock();
	
	serial_init();
	ms1_set_mode(GPIO_MODE);
	ms1_gpio_mode(0x0, 0xD);

	if(!ms1_gpio_read(0xD)) {
		if(ms_serial_flash_init())
			return -1;
		if(ms_serial_flash_update()) {
			serial_puts("serial flash update failed\n");
			return -1;
		}
		serial_puts("serial flash update success\n");
	}
	else {
		if(ms_nand_flash_init())
			return -1;
		if(ms_nand_flash_update()) {
			serial_puts("nand flash update failed\n");
			return -1;
		}
		serial_puts("nand flash update success\n");
	}

	return 0;
}
