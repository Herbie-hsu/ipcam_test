#include "common.h"

#include "ms_sf.h"	
#include "sonix_sf.h"

//--------------------------------
#define SF_CTL_MODE_SF		0x3
#define SF_READ_MODE		0x1
#define SF_WRITE_MODE		0x0
#define SF_NORMAL			0x0
#define SF_AAI				0x1
#define SF_AAW				0x2			
#define SF_SE				0x0
#define SF_BE				0x1
#define SF_RETRY_CNT        0x05
#define SF_TSS_DETECT_CNT   0x08
#define SF_ALL_DETECT_CNT   0x10

#define PATTERN_TYPE 1  // 1: 0x0 -> 0xff; 0: 0xff -> 0x0

#define MSSF_ADDR_1(offset) ((offset >> 16) & 0xff)
#define MSSF_ADDR_2(offset) ((offset >> 8)  & 0xff)
#define MSSF_ADDR_3(offset) (offset & 0xff)

int CRC16_R[16];
u32 CRC_CORRECT;

static struct serial_flash *serial_flash_current = NULL;

static void sf_crc_init(void)
{
	int i;
	
	for (i = 0; i < 16; i++)
	{
		CRC16_R[i] = 0;
	}
}

static void sf_crc_cal(u32 si_data, int *CRC16_R)
{
	int i, j;
	u32 crc = 0x0;
	int  CRC16[16], si[8];

	for (i = 0; i < 16; i++)
	{
		CRC16[i] = *(CRC16_R + i);
	}

	for (i = 0; i < 8; i++){
		si[i] = (si_data & (0x1 << i)) >> i;	
	}

	for (i = 7; i >= 0; i--){

		for (j = 0; j < 16; j++){
			*(CRC16_R + j) = CRC16[j];
			CRC16[j] = 0;
		}	
			
		CRC16[0] = *(CRC16_R + 15) ^ si[i];

		CRC16[4] = *(CRC16_R + 3);
		CRC16[3] = *(CRC16_R + 2);
		CRC16[2] = *(CRC16_R + 1);
		CRC16[1] = *(CRC16_R + 0);		

		CRC16[5] = *(CRC16_R + 4) ^ *(CRC16_R + 15) ^ si[i];

		CRC16[11] = *(CRC16_R + 10);
		CRC16[10] = *(CRC16_R + 9);
		CRC16[9] = *(CRC16_R + 8);		
		CRC16[8] = *(CRC16_R + 7);		
		CRC16[7] = *(CRC16_R + 6);		
		CRC16[6] = *(CRC16_R + 5);		

		CRC16[12] = *(CRC16_R + 11) ^ *(CRC16_R + 15) ^ si[i];

		CRC16[15] = *(CRC16_R + 14);		
		CRC16[14] = *(CRC16_R + 13);		
		CRC16[13] = *(CRC16_R + 12);		
	}

	for (j = 0; j < 16; j++){		
		crc |= (CRC16[j] << j);
	}

	for (j = 0; j < 16; j++){
		*(CRC16_R + j) = CRC16[j];
		CRC16[j] = 0;
	}	

	CRC_CORRECT = crc;
}

static void sf_WREN(struct sf_instruction *sf_inst)
{
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	
	sfWriteData(sf_inst->WREN);
	
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	}
	
	sfChipDisable();
}

static u32 sf_RDSR(struct sf_instruction *sf_inst)
{
	u32 status = 0x0;
	
	sfChipEnable();
	
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->RDSR);
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	}
	
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	}
	status = sfReadData();
	
	sfChipDisable();
	
	return status;
}

#if 0
static void sf_WRSR(struct sf_instruction *sf_inst)
{
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->WRSR);
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	}
	
	/**
	 * write data
	 */
	sfWriteData(0x0);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable();
}

static void sf_WRSR_SST(struct sf_instruction *sf_inst)
{
	sfChipEnable ();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->EWSR);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable();

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->WRSR);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	
	/**
	 * write addr
	 */
	sfWriteData(0x0);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable();
}
#endif

static void sf_initial(void)
{	
	sfSetMsMode(SF_CTL_MODE_SF);
	sfChipDisable();
	sfExtraENSwitch(DISABLE);
	sfEccENSwitch(DISABLE);
	sfSetMsSpeed(0x1);
}

/******************************************************************
 * read ID type
 ******************************************************************/
	
static int sf_readID1(struct sf_instruction *sf_inst, u32 id[])
{
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->RDID); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	} 
	
	/**
	 * read Manufacturer ID
	 */
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	
	id[0] = sfReadData();

	/**
	 * read Memory Type ID
	 */
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	
	id[1] = sfReadData();

	/**
	 * read device ID
	 */
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	
	id[2] = sfReadData();

	sfChipDisable();

	return 0;
}

static int sf_readID2(struct sf_instruction *sf_inst, u32 id[])
{
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->RDID); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	} 
	
	/**
	 * dummy bytes 3 times
	 */
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	} 
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	} 
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	} 
	
	/**
	 * read ID
	 */
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	
	id[0] = sfReadData();
	
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	
	id[1] = sfReadData();
	
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	
	id[2] = sfReadData();

	sfChipDisable();

	return 0;
}

static int sf_readID3(struct sf_instruction *sf_inst, u32 id[])
{
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->RDID); 
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	} 
	
	/**
	 * write addr
	 */
	sfWriteData(0x0);
	while (1){
		if(sfCheckMsRdy()){
			break;
		}
	}
	sfWriteData(0x0);
	while (1){
		if(sfCheckMsRdy()){
			break;
		}
	}
	sfWriteData(0x0);
	while (1){
		if(sfCheckMsRdy()){
			break;
		}
	}	
	
	/**
	 * read Manufacture ID
	 */
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	}	
	id[0] = sfReadData();

	/**
	 * read Device ID
	 */
	sfMsRegRWSwitch(SF_READ_MODE);
	sfWriteData(0x0); 
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	}	
	id[1] = sfReadData();
	
	sfChipDisable();

	return 0;
}

///////add on 2013-8-30
/******************************************************************
 * serial flash clear write protected
 ******************************************************************/
static int sf_clear_WRSR(struct sf_instruction *sf_inst)
{
#if 0
	u32 status;
	//Set serial flash CE = 0
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sf_WREN(sf_inst);
	
	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x3) != 0x2);
	
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->WRSR); 
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	} 
	
	/**
	 * write addr
	 */
	sfWriteData(0x0);
	while (1){
		if(sfCheckMsRdy()){
			break;
		}
	}

	sfChipDisable ();
#endif
	return 0;
}
static int sf_ProtectSR(struct sf_instruction *sf_inst)
{
#if 0
	u32 status;
	//Set serial flash CE = 0
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	
	sf_WREN(sf_inst);
	
	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x3) != 0x2);
	
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	
	sfWriteData(sf_inst->WRSR); 
	while (1) {
		if(sfCheckMsRdy()) {
			break;
		}
	} 
	
	/**
	 * write addr
	 */
	if (strncmp(serial_flash_current->name, "SPANSION", 8) == 0) {
		sfWriteData(0x1c);
	} else if (strncmp(serial_flash_current->name, "MXIC&AMIC", 9) == 0) {
		sfWriteData(0x3c);
	} else if (strncmp(serial_flash_current->name, "GD", 2) == 0) {
		sfWriteData(0x7c);
	}
	while (1){
		if(sfCheckMsRdy()){
			break;
		}
	}

	sfChipDisable ();
#endif
	return 0;
}
///////end 

/******************************************************************
 * serial flash erase
 ******************************************************************/
static int sf_sector_erase(struct sf_instruction *sf_inst, u32 offset)
{
	u32 status = 0x0;

	sf_WREN(sf_inst);

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x3)!= 0x2);

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->SE);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}

	/**
	 * write addr
	 */
	sfWriteData(MSSF_ADDR_1(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(MSSF_ADDR_2(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(MSSF_ADDR_3(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable();

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x1) == 0x1);

	return 0;
}

/******************************************************************
 * serial flash erase
 ******************************************************************/
static int sf_block_erase(struct sf_instruction *sf_inst, u32 offset)
{
	u32 status = 0x0;

	sf_WREN(sf_inst);

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x3)!= 0x2);

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->BE);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}

	/**
	 * write addr
	 */
	sfWriteData(MSSF_ADDR_1(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(MSSF_ADDR_2(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(MSSF_ADDR_3(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable();

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x1) == 0x1);

	return 0;
}


/******************************************************************
 * CPU write
 ******************************************************************/
static int sf_cpu_write(struct sf_instruction *sf_inst, 
	u32 offset, uchar *addr, u32 len)
{
	int i;
	int retval;
	u32 status, data, write_data;

	retval = 0;

	sf_WREN(sf_inst);

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x3) != 0x2);

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->PP);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}

	/**
	 * write addr
	 */
	sfWriteData(MSSF_ADDR_1(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(MSSF_ADDR_2(offset));
	while (1){
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(MSSF_ADDR_3(offset));
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}

	/**
	 * CRC calulate enable
	 */
	sfEccENSwitch(DISABLE);
	sfEccENSwitch(ENABLE);
	sfCrcENSwitch(DISABLE);
	sfCrcENSwitch(ENABLE);

	sf_crc_init();

	/** 
	 * write data
	 */
	for (i = 0; i < len; i++) {
#if PATTERN_TYPE
		write_data = addr[i];
#else
		write_data = 0xff - addr[i];
#endif
		sfWriteData(write_data);
		sf_crc_cal(write_data, CRC16_R);
		
		while (1) {
			if (sfCheckMsRdy()) {
				break;
			}
		}
	}

	data = sfReadCrc16();
	if (data == CRC_CORRECT) {
		;
	}
	else {	
		serial_puts("sf_cpu_write_NORMAL: CRC ERROR\n");
		retval = -1;
	}
	sfEccENSwitch(DISABLE);
	sfCrcENSwitch(DISABLE);
	sfChipDisable();	

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x3) != 0x0);

	return retval;
}

static int sf_cpu_write_AAI(struct sf_instruction *sf_inst, 
	u32 offset, uchar *addr, u32 len)
{
	int i;
	int retval;
	u32 status, write_data, data;
	
	retval = 0;
	
	sfSetWMode(SF_NORMAL);

	sf_WREN (sf_inst);

	do {
		status = sf_RDSR(sf_inst);
	}while (status != 0x2);
	
	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->AAI);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	
	/**
	 * write addr
	 */
	sfWriteData((offset & 0xFF0000) >> 16);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData((offset & 0xFF00) >> 8);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(offset & 0xFF);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	

	/**
	 * CRC calulate enable
	 */
	sfEccENSwitch(ENABLE);
	sfEccENSwitch(ENABLE);
	sfCrcENSwitch(DISABLE);
	sfCrcENSwitch(ENABLE);

	sf_crc_init();
	
	/**
	 * write data
	 */
	i = 0;
#if PATTERN_TYPE
	write_data = addr[i++];
#else
	write_data = 0xff - addr[i++];	
#endif
	sfWriteData(write_data);
	sf_crc_cal(write_data, CRC16_R);

	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}

	data = sfReadCrc16();
	if (data == CRC_CORRECT) {
		;
	}
	else {
		serial_puts("sf_cpu_write_AAI: CRC ERROR\n");
		retval = -1;
	}		
	sfEccENSwitch(DISABLE);
	sfChipDisable();

	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x1) == 0x1);

	/**
	 * after second byte
	 */
	for (; i < len; i++){ 
		sfChipEnable();
		sfMsRegRWSwitch(SF_WRITE_MODE);
		sfWriteData(sf_inst->AAI);
		while (1) {
			if (sfCheckMsRdy()) {
				break;
			}
		}

		/**
		 * CRC calulate enable
		 */
		sfEccENSwitch(ENABLE);
		sfCrcENSwitch(DISABLE);

		sf_crc_init();
		 
		/**
		 * write data
		 */
#if PATTERN_TYPE
		write_data = addr[i];
#else
		write_data = 0xff - addr[i];	
#endif
		sfWriteData(write_data);
		sf_crc_cal(write_data, CRC16_R);
	
		while (1) {
			if (sfCheckMsRdy()) {
				break;
			}
		}	

		data = sfReadCrc16();
		if (data == CRC_CORRECT) {
			;
		}
		else {
			serial_puts("sf_cpu_write_AAI: CRC ERROR\n");
			retval = -1;
		}
		sfEccENSwitch(DISABLE);
		sfChipDisable();

		do {
			status = sf_RDSR(sf_inst);
		}while ((status & 0x1) == 0x1);	
	}

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->WRDI);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable ();
	
	return retval;
}

static int sf_cpu_write_AAW (struct sf_instruction *sf_inst,
	u32 offset, uchar *addr, u32 len)
{
	int i,j;	
	int retval;
	u32 status, data, write_data;

	retval = 0;
	
	sfSetWMode(SF_NORMAL);
	
	sf_WREN(sf_inst);

	do {
		status = sf_RDSR(sf_inst);
	}while (status != 0x2);

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->AAW);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	
	/**
	 * write addr
	 */
	sfWriteData((offset & 0xFF0000) >> 16);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData((offset &0xFF00) >> 8);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfWriteData(offset & 0xFF);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}	

	/**
	 * CRC calulate enable
	 */
	sfEccENSwitch(DISABLE);
	sfEccENSwitch(ENABLE);
	sfCrcENSwitch(DISABLE);
	sfCrcENSwitch(ENABLE);

	sf_crc_init();
	
	/**
	 * write data
	 *
	 * 1st & 2nd byte
	 */
	i = 0;
	
#if PATTERN_TYPE
	write_data = addr[i++];
#else
	write_data = 0xff - addr[i++];	
#endif
	sfWriteData(write_data);
	sf_crc_cal(write_data, CRC16_R);
		
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	
#if PATTERN_TYPE
	write_data = addr[i++];
#else
	write_data = 0xff - addr[i++];	
#endif
	sfWriteData(write_data);
	sf_crc_cal(write_data, CRC16_R);
	
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}

	data = sfReadCrc16();
	if (data == CRC_CORRECT) {
		;
	}
	else {
		serial_puts("sf_cpu_write_AAW: CRC ERROR\n");
		retval = -1;		
	}
	sfEccENSwitch(DISABLE);
	sfCrcENSwitch(DISABLE);
	sfChipDisable ();


	
	do {
		status = sf_RDSR(sf_inst);
	}while ((status & 0x1) == 0x1);

	/**
	 * after sencond byte
	 */
	for (; i < len; i = i + 2){ 
		sfChipEnable();
		sfMsRegRWSwitch(SF_WRITE_MODE);
		sfWriteData(sf_inst->AAW);
		while (1) {
			if (sfCheckMsRdy()) {
				break;
			}
		}

		/**
		 * CRC calulate enable
		 */
		sfEccENSwitch(DISABLE);
		sfEccENSwitch(ENABLE);
		sfCrcENSwitch(DISABLE);
		sfCrcENSwitch(ENABLE);

		sf_crc_init();
		
		for (j = i; ((j < i + 2) && (j < len)); j++) {
#if PATTERN_TYPE
			write_data = addr[j];
#else
			write_data = 0xff - addr[j];	
#endif
			sfWriteData(write_data);
			sf_crc_cal(write_data, CRC16_R);
	
			while (1) {
				if (sfCheckMsRdy()) {
					break;
				}
			}	
		}
		
		data = sfReadCrc16();
		if (data == CRC_CORRECT) {
			;
		}
		else {
			serial_puts("sf_cpu_write_AAW: CRC ERROR\n");
			retval = -1;		
		}
		sfEccENSwitch(DISABLE);
		sfCrcENSwitch(DISABLE);
		sfChipDisable();
		do{
			status = sf_RDSR(sf_inst);
		}while ((status & 0x1) == 0x1);	
	}

	sfChipEnable();
	sfMsRegRWSwitch(SF_WRITE_MODE);
	sfWriteData(sf_inst->WRDI);
	while (1) {
		if (sfCheckMsRdy()) {
			break;
		}
	}
	sfChipDisable();

	return retval;
}

/********************************************************************
 ********************************************************************
 ****** Serial flash common init, erase, read, write function *******
 ********************************************************************
 ********************************************************************/
 
static int serial_flash_init(struct serial_flash *sf)
{
	int retval;
	u32 id[3] = {0, 0, 0};

	sf_initial();
	
	if(NULL == sf->readID) {
		serial_printf("serial flash %s: serial_flash_readID is NULL\n", 
			sf->name);
		return -EFAULT;
	}
	
	retval = sf->readID(sf->sf_inst, id);
	if(retval) {
		serial_printf("serial flash %s: serial_flash_readID failed\n",
			sf->name);
		return -EFAULT;
	}

  //////add on 2013-8-30
  serial_printf("ID = %x,%x,%x\n",id[0],id[1],id[2]);
  //////end
  
	if((sf->id0 != id[0]) || (sf->id1 != id[1]) || (sf->id2 != id[2]))
		return -EFAULT;
  //////add on 2013-8-30
  sf->clearSR(sf->sf_inst);
  //////end
 
	return 0;
}

static int serial_flash_sector_erase(struct serial_flash *sf, u32 offset, size_t len)
{
	int retval;
	u32 top_addr;

	if(0 == sf->sector_size) {
		serial_printf("serial flash %s: sector size error\n", 
			sf->name);
		return -EFAULT;
	}

	if(NULL == sf->sector_erase) {
		serial_printf("serial flash %s: serial_flash_sector_erase is NULL\n", 
			sf->name);
		return -EFAULT;
	}

	if((offset % sf->sector_size) || (len % sf->sector_size)) {
		serial_printf("serial flash %s: offset/len must multiple of sector size\n", 
			sf->name);
		return -EINVAL;
	}

	top_addr = offset + len;
	
	if(top_addr > sf->chip_size) {
		serial_printf("serial flash %s: end address out of chip size\n", 
			sf->name);
		return -EINVAL;
	}

	serial_printf("Sector Erase:");

	for(; offset < top_addr; offset += sf->sector_size) {
		retval = sf->sector_erase(sf->sf_inst, offset);
		if(retval) {
			serial_printf("serial flash %s: serial_flash_sector_erase failed\n",
				sf->name);
			return -EFAULT;
		}
		serial_printf(".");
	}

	serial_printf("\n");

	return 0;
}

static int serial_flash_block_erase(struct serial_flash *sf, u32 offset, size_t len)
{
	int retval;
	u32 top_addr;

	if(0 == sf->block_size) {
		serial_printf("serial flash %s: block_size size error\n", 
			sf->name);
		return -EFAULT;
	}

	if(NULL == sf->block_size) {
		serial_printf("serial flash %s: serial_flash_block_erase is NULL\n", 
			sf->name);
		return -EFAULT;
	}

	if((offset % sf->block_size) || (len % sf->block_size)) {
		serial_printf("serial flash %s: offset/len must multiple of block size\n", 
			sf->name);
		return -EINVAL;
	}

	top_addr = offset + len;
	
	if(top_addr > sf->chip_size) {
		serial_printf("serial flash %s: end address out of chip size\n", 
			sf->name);
		return -EINVAL;
	}

	serial_printf("block Erase:");

	for(; offset < top_addr; offset += sf->block_size) {
		retval = sf->block_erase(sf->sf_inst, offset);
		if(retval) {
			serial_printf("serial flash %s: serial_flash_block_erase failed\n",
				sf->name);
			return -EFAULT;
		}
		serial_printf(".");
	}

	serial_printf("\n");

	return 0;
}



static int serial_flash_write(struct serial_flash *sf, u32 offset,
	uchar *addr, size_t len)
{	
	int retval;
	u32 bytes;
	u32 top_addr;
	u32 actual_len;

	if(0 == sf->page_size) {
		serial_printf("serial flash %s: page size error\n", 
			sf->name);
		return -EFAULT;
	}

	if(NULL == sf->write) {
		serial_printf("serial flash %s: serial_flash_write is NULL\n", 
			sf->name);
		return -EFAULT;
	}

	top_addr = offset + len;

	if(top_addr > sf->chip_size) {
		serial_printf("serial flash %s: end address out of chip size\n", 
			sf->name);
		return -EINVAL;
	}

	bytes = offset % sf->page_size;

	serial_printf("Write:");
	for(; offset < top_addr; offset += actual_len) {
		actual_len = min(top_addr - offset,
			sf->page_size - bytes);
		
	 	retval = sf->write(sf->sf_inst, offset, addr, actual_len);
		if(retval) {
			serial_printf("serial flash %s: serial_flash_write failed\n", 
				sf->name);
			return -EFAULT;
		}
		
		serial_printf(".");
		
		bytes = 0;
		addr += actual_len;
	}
	serial_printf("\n");

	return 0;
}

//////add on 2013-09-2
static int serial_flash_protect(struct serial_flash *sf)
{	
	int retval;

	retval = sf->ProtectSR(sf->sf_inst);
	return 0;
}
//////end

/********************************************************************
 ********************************************************************
 ******************* Serial flash sub-system API ********************
 ********************************************************************
 ********************************************************************/

static struct sf_instruction insts_array[] = {

	[0] = { //SST_AAI
			.WREN	=	0x06,
	  		.WRDI	=	0x04,
	  		.RDID	=	0x90,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	NONE,
			.SE	=	0x20,
			.BE	=	0x52,
			.DP	=	NONE,
			.RES	=	NONE,
			.AAI	=	0xaf,
			.AAW	=	NONE,
			.BP	=	0x02,
			.EWSR	=	0x50,
			.TBP	=	0xfa,
		},

	[1] = { //SST_AAW
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0x90,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	NONE,
			.SE	=	0x20,
			.BE	=	0x52,
			.DP	=	NONE,
			.RES	=	NONE,
			.AAI	=	NONE,
			.AAW	=	0xad,
			.BP	=	0x02,
			.EWSR	=	0x50,
			.TBP	=	0x7d,
		},

	[2] = { //SST_PP
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0x90,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	0x02,
			.SE	=	0x20,
			.BE	=	0x52, //32KB:0x52; 64KB:0xd8
			.DP	=	NONE,
			.RES	=	NONE,
			.AAI	=	NONE,
			.AAW	=	NONE,
			.BP	=	NONE,
			.EWSR	=	0x50,
			.TBP	=	NONE,
		},

	[3] = {	//MXIC&AMIC
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0x9f,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	0x02,
			.SE	=	0x20,
			.BE	=	0xd8,
			.CE     =	0xc7,
			.DP	=	0xb9,
			.RES	=	0xab,	
			.AAI	=	NONE,  
			.AAW	=	NONE,  
			.BP	=	NONE,
			.EWSR	=	NONE,
			.TBP	=	NONE,
		},

	[4] = { //GD25Q128C & GD25Q16B
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0x9f,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	0x02,
			.SE	=	0x20,
			.BE	=	0xd8,
			.CE     =	0xc7,
			.DP	=	0xb9,
			.RES	=	0xab,
			.AAI	=	NONE,
			.AAW	=	NONE,
			.BP	=	NONE,
			.EWSR	=	NONE,
			.TBP	=	NONE,
		},
	[5] = { //PMC
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0xab,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	0x02,
			.SE	=	0xd7,
			.BE	=	0x20, //sector:0x20 , block:0xd8
			.DP	=	NONE,
			.RES	=	NONE,
			.AAI	=	NONE,
			.AAW	=	NONE,
			.BP	=	NONE,
			.EWSR	=	NONE,
			.TBP	=	NONE,
		},

	[6] = { //EN
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0x9f,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	0x02,
			.SE	=	0x20, //sector:0x20 , block:0xd8
			.BE	=	0xd8, //use sector erase ,actual 0xc7
			.DP	=	0xb9,
			.RES	=	NONE,
			.AAI	=	NONE,
			.AAW	=	NONE,
			.BP	=	NONE,
			.EWSR	=	NONE,
			.TBP	=	NONE,
		},

	[7] = { //SPANSION
			.WREN	=	0x06,
			.WRDI	=	0x04,
			.RDID	=	0x9f,
			.RDSR	=	0x05,
			.WRSR	=	0x01,
			.READ	=	0x03,
			.PP	=	0x02,
			.SE	=	0x20,  
			.BE	=	0xd8,
			.DP	=	0xb9,
			.RES	=	0xab,
			.AAI	=	NONE,
			.AAW	=	NONE,
			.BP	=	NONE,
			.EWSR	=	NONE,
			.TBP	=	NONE,
		},
};

static struct serial_flash serial_flash_array[] = {
	{
		.name	= "SST_AAI",	//SST25VF512
		.id0	= 0xBF,
		.id1	= 0x48,
		.id2	= 0,
		.page_size	= 256,		// 256 byte
		.sector_size	= 256 * 16,	// 4K byte
		.block_size	= 256 * 16 * 8,	// 32K byte
		.chip_size	= 256 * 16 * 8 * 2,// 64K byte
		.sf_inst	= &insts_array[0],
		.readID		= sf_readID1,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write_AAI,
		//////add on 2013-8-30
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
		//////end
	},
	{
		.name	= "SST_AAW",
		.sf_inst	= &insts_array[1],
		.readID		= sf_readID3,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write_AAW,
		//////add on 2013-8-30
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
		//////end
	},
	{
		.name	= "SST_PP",
		.sf_inst	= &insts_array[2], 
		.readID		= sf_readID3,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		//////add on 2013-8-30
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
		//////end
	},
	{
		.name	= "MXIC&AMIC", //MX25L1605D
		.id0	= 0xc2,
		.id1	= 0x20,
		.id2	= 0x18,
		.page_size	= 256,		//256 byte
		.sector_size	= 256 * 16,	//4K byte
		.block_size	= 256 * 16 * 16,//64K byte
		.chip_size	= 256 * 16 * 16 * 256,//16M byte
		.max_hz		= 33 * 1024 * 1024,
		.sf_inst	= &insts_array[3], 
		.readID		= sf_readID1,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		//////add on 2013-8-30
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
		//////end
	},
	//////add on 2013-8-30
	{
		.name = "MXIC&AMIC", //MX25L12835E
		.id0	= 0xc2,
		.id1	= 0x20,
		.id2	= 0x19,
		.page_size	= 256,		//256 byte
		.sector_size	= 256 * 16,	//4K byte
		.block_size	= 256 * 16 * 16,//64K byte
		.chip_size	= 256 * 16 * 16 * 256,//16M byte
		.max_hz		= 33 * 1024 * 1024,
		.sf_inst	= &insts_array[3],
		.readID		= sf_readID1,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
	},
	//////end
	{
		.name = "GD", //GD25Q128C
		.id0	= 0xc8,
		.id1	= 0x40,
		.id2	= 0x18,
		.page_size	= 256,		//256 byte
		.sector_size	= 256 * 16,	//4K byte
		.block_size	= 256 * 16 * 16,//64K byte
		.chip_size	= 256 * 16 * 16 * 256,//16 Mbyte
		.max_hz		= 33 * 1024 * 1024,
		.sf_inst	= &insts_array[4],
		.readID		= sf_readID1,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
	},
	{
		.name = "GD", //GD25Q16B
		.id0	= 0xc8,
		.id1	= 0x40,
		.id2	= 0x15,
		.page_size	= 256,		//256 byte
		.sector_size	= 256 * 16,	//4K byte
		.block_size	= 256 * 16 * 16,//64K byte
		.chip_size	= 256 * 16 * 16 * 256,//16 Mbyte
		.max_hz		= 33 * 1024 * 1024,
		.sf_inst	= &insts_array[4],
		.readID		= sf_readID1,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
	},
	{
		.name	= "PMC", //Pm25LV512
		.id0	= 0x9d,
		.id1	= 0x7b,
		.id2	= 0x7f,
		.page_size	= 256,		// 256 byte
		.sector_size	= 256 * 16,	// 4K byte
		.block_size	= 256 * 16 * 8,	// 32K byte
		.chip_size	= 256 * 16 * 8 * 2,// 64K byte
		.sf_inst	= &insts_array[5], 
		.readID		= sf_readID2,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		//////add on 2013-8-30
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
		//////end
	},
	{
		.name = "EN", //EN25P64
		.id0 = 0xef,
		.id1 = 0x30,
		.id2 = 0x17,
		.page_size   = 256,		// 256 byte
		.sector_size = 256 * 16,	// 64K byte
		.block_size  = 256 * 16 * 16,	// 64K byte
		.chip_size   = 256 * 16 * 16 * 128,// 8M byte
		.sf_inst     = &insts_array[6],
		.readID      = sf_readID1,
		.sector_erase = sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write       = sf_cpu_write,
		//////add on 2013-8-30
		.clearSR       = sf_clear_WRSR,
		.ProtectSR     = sf_ProtectSR,
		//////end
	},
	{
		.name = "SPANSION", //S25FL127S
		.id0         = 0x01,
		.id1         = 0x20,
		.id2         = 0x18,
		.page_size	= 256,		//256 byte
		.sector_size	= 256 * 16,	//4K byte
		.block_size	= 256 * 16 * 16,//64K byte
		.chip_size	= 256 * 16 * 16 * 256,//16 Mbyte
		.max_hz		= 50 * 1024 * 1024,
		.sf_inst	= &insts_array[7],
		.readID		= sf_readID1,
		.sector_erase	= sf_sector_erase,
		.block_erase	= sf_block_erase,
		.write		= sf_cpu_write,
		.clearSR	= sf_clear_WRSR,
		.ProtectSR	= sf_ProtectSR,
	},
};


int  ms_serial_flash_init(void)
{
	int i, cnt;
	u32 speed;
	
	cnt = sizeof(serial_flash_array) / sizeof(struct serial_flash);
	
	for(i = 0; i < cnt; i++) {
		if(serial_flash_init(&serial_flash_array[i])) {
			continue;
		}
		break;
	}

	if(i == cnt) {
		serial_printf("serial_flash initial failed\n");
		return -1;
	}

	serial_flash_current = &serial_flash_array[i];

	speed = AHB2_CLOCK / serial_flash_current->max_hz;
	if (speed > 1) {
		if (AHB2_CLOCK % serial_flash_current->max_hz)
			speed -= 1;
		else
			speed -= 2;
	}
	else
		speed = 0;
	//printf ("ahb2 speed = %dM, sf max = %dM, peed set = %d\n", gd->ahb2_clk/1000000, sf->max_hz/1000000, speed);
	sfSetMsSpeed (speed);

	serial_printf(serial_flash_current->name);
	serial_printf("\n");

	return 0;
}

static int ms_serial_flash_sector_erase(u32 offset, u32 len)
{
	serial_printf("sector Erase: offset 0x%08x, len 0x%08x\n", offset, len);

	return serial_flash_sector_erase(serial_flash_current,
		offset, len);
}

static int ms_serial_flash_block_erase(u32 offset, u32 len)
{
	serial_printf("block Erase: offset 0x%08x, len 0x%08x\n", offset, len);

	return serial_flash_block_erase(serial_flash_current,
		offset, len);
}

static int ms_serial_flash_write(u32 offset, uchar *addr, u32 len)
{	
	serial_printf("Write: offset 0x%08x, buf = 0x%08x , len 0x%08x\n",
		offset, addr, len);

	return serial_flash_write(serial_flash_current,
		offset, addr, len);
}

//////add on 2013-09-2
static int ms_serial_flash_protect(void)
{	
	serial_printf("Serial Flash Protected\n");
	
	return serial_flash_protect(serial_flash_current);
}
//////end

#define SPI_BLOCK_SIZE	0x10000
#define SPI_SECTOR_SIZE	0x1000

int ms_serial_flash_update(void)
{
	if(HW_SETTING_LEN > 0) {
		serial_printf("Write HW_Setting\n");

		if(ms_serial_flash_sector_erase(SPI_HW_SETTING_STR, 
			(SPI_U_BOOT_STR - SPI_HW_SETTING_STR)))
			return -1;

		if(ms_serial_flash_write(SPI_HW_SETTING_STR, (uchar *)HW_SETTING_START, 
			HW_SETTING_LEN))
			return -1;
	}

	if(U_BOOT_LEN > 0) {
		int offset=0;
		serial_printf("Write U-Boot\n");
		for (offset = SPI_U_BOOT_STR; offset < SPI_BLOCK_SIZE; offset += SPI_SECTOR_SIZE) {
			if(ms_serial_flash_sector_erase(offset, 
				SPI_SECTOR_SIZE))
				return -1;
		}

		if(ms_serial_flash_block_erase(SPI_BLOCK_SIZE, 
			(SPI_U_ENV_STR - SPI_BLOCK_SIZE)))
			return -1;

		if(ms_serial_flash_write(SPI_U_BOOT_STR, (uchar *)U_BOOT_START, 
			U_BOOT_LEN))
			return -1;
	}

#if 0
	if(U_ENV_LEN > 0) {
		serial_printf("Write U-Env\n");
		if(ms_serial_flash_sector_erase(SPI_U_ENV_STR, 
			(SPI_FLASH_LAYOUT_STR - SPI_U_ENV_STR)))
			return -1;

		if(ms_serial_flash_write(SPI_U_ENV_STR, (uchar *)U_ENV_START, 
			U_ENV_LEN))
			return -1;
	}
#else
	serial_printf("Write U-Env\n");
	/* erase env and flash-layout  for spansion flash*/
	if(ms_serial_flash_block_erase(SPI_U_ENV_STR, 
			(SPI_FACTORY_STR - SPI_U_ENV_STR)))
			return -1;
#endif

	if(FLASH_LAYOUT_LEN > 0) {
		serial_printf("Write Flash-Layout\n");
		/*if(ms_serial_flash_block_erase(SPI_FLASH_LAYOUT_STR, 
			(SPI_FACTORY_STR - SPI_FLASH_LAYOUT_STR)))
			return -1;*/

		if(ms_serial_flash_write(SPI_FLASH_LAYOUT_STR, (uchar *)FLASH_LAYOUT_START, 
			FLASH_LAYOUT_LEN))
			return -1;
	}

	if(FACTORY_LEN > 0) {
		serial_printf("Write Factory-Config\n");
		if(ms_serial_flash_block_erase(SPI_FACTORY_STR, 
			(SPI_KERNEL_STR - SPI_FACTORY_STR)))
			return -1;

		if(ms_serial_flash_write(SPI_FACTORY_STR, (uchar *)FACTORY_START, 
			FACTORY_LEN))
			return -1;
	}

	if(U_LOGO_LEN > 0) {
		serial_printf("Write U_LOGO\n");
		if(ms_serial_flash_sector_erase(SPI_U_LOGO_STR, 
			(SPI_U_LOGO_END - SPI_U_LOGO_STR + 1)))
			return -1;

		if(ms_serial_flash_write(SPI_U_LOGO_STR, (uchar *)U_LOGO_START, 
			U_LOGO_LEN))
			return -1;
	}

/*	if(RESCUE_LEN > 0) {
		serial_printf("Write Rescue\n");
		if(ms_serial_flash_block_erase(SPI_RESCUE_STR, 
			(SPI_RESCUE_END - SPI_RESCUE_STR + 1)))
			return -1;

		if(ms_serial_flash_write(SPI_RESCUE_STR, (uchar *)RESCUE_START, 
			RESCUE_LEN))
			return -1;
	}*/
	//////add on 2013-09-2

	ms_serial_flash_protect();
	///////end
	return 0;
}
