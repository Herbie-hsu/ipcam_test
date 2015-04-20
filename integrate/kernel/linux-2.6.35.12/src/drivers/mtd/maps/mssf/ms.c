#include <linux/module.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/mtd/compatmac.h>
#include <linux/mtd/mtd.h>

#include <asm/io.h>

#include "ms.h" 
#include "mssf.h"


void sf_SetInstruction (int sf_type, struct sf_instruction *sf_inst)
{
	switch (sf_type)
	{
		case SF_SST_AAI:
			//SST_AAI
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x90;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= SF_NULL;
			sf_inst->SE	= 0x20;
			sf_inst->BE	= 0x52;
			sf_inst->DP	= SF_NULL;
			sf_inst->RES	= SF_NULL;
			sf_inst->AAI	= 0xaf;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= 0x02;
			sf_inst->EWSR	= 0x50;
			sf_inst->TBP	= 0xfa;

			break;

		case SF_SST_AAW:
			//SST_AAW
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x90;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= SF_NULL;
			sf_inst->SE	= 0x20;
			sf_inst->BE	= 0x52;
			sf_inst->DP	= SF_NULL;
			sf_inst->RES	= SF_NULL;
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= 0xad;
			sf_inst->BP	= 0x02;
			sf_inst->EWSR	= 0x50;
			sf_inst->TBP	= 0x7d;

			break;

		case SF_SST_PP:
			//SST_PP
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x90;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= 0x02;
			sf_inst->SE	= 0x20;
			sf_inst->BE	= 0x52; //32KB:0x52; 64KB:0xd8
			sf_inst->DP	= SF_NULL;
			sf_inst->RES	= SF_NULL;
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= SF_NULL;
			sf_inst->EWSR	= 0x50;
			sf_inst->TBP	= SF_NULL;

			break;

		case SF_MXIC_AMIC:
			//MXIC&AMIC
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x9f;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= 0x02;
			sf_inst->SE	= 0x20;  //d8
			sf_inst->BE	= 0x52;  //32k=0x52,64k=0xd8
			sf_inst->DP	= 0xb9;
			sf_inst->RES	= 0xab;  //NULL	
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= SF_NULL;
			sf_inst->EWSR	= SF_NULL;
			sf_inst->TBP	= SF_NULL;

			break;

		case SF_PMC:
			//PMC
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0xab;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= 0x02;
			sf_inst->SE	= 0xd7;
			sf_inst->BE	= 0xd8;
			sf_inst->DP	= SF_NULL;
			sf_inst->RES	= SF_NULL;
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= SF_NULL;
			sf_inst->EWSR	= SF_NULL;
			sf_inst->TBP	= SF_NULL;

			break;

		case SF_EN:
			//EN
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x9f;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= 0x02;
			sf_inst->SE	= 0xd8;  
			sf_inst->BE	= 0xc7;  
			sf_inst->DP	= 0xb9;
			sf_inst->RES	= SF_NULL;
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= SF_NULL;
			sf_inst->EWSR	= SF_NULL;
			sf_inst->TBP	= SF_NULL;

			break;

		case SF_GD:
			//GD25Q128C & GD25Q16B
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x9f;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= 0x02;
			sf_inst->SE	= 0x20;
			sf_inst->BE	= 0x52;	// 32k = 0x52
			//sf_inst->BE	= 0xd8;	// 64k = 0xd8
			sf_inst->DP	= 0xb9;
			sf_inst->RES	= 0xab;
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= SF_NULL;
			sf_inst->EWSR	= SF_NULL;
			sf_inst->TBP	= SF_NULL;

			break;

		case SF_SPANSION:
			//S25FL127S
			sf_inst->typenum = sf_type;
			sf_inst->WREN	= 0x06;
			sf_inst->WRDI	= 0x04;
			sf_inst->RDID	= 0x9f;
			sf_inst->RDSR	= 0x05;
			sf_inst->WRSR	= 0x01;
			sf_inst->READSF	= 0x03;
			sf_inst->PP	= 0x02;
			sf_inst->SE	= 0x20; // 4K ,  4-kB Sector Erase
			sf_inst->BE	= 0xd8;	// 64k
			sf_inst->DP	= 0xb9;
			sf_inst->RES	= 0xab;
			sf_inst->AAI	= SF_NULL;
			sf_inst->AAW	= SF_NULL;
			sf_inst->BP	= SF_NULL;
			sf_inst->EWSR	= SF_NULL;
			sf_inst->TBP	= SF_NULL;

			break;
	}
}

/*0x00*/
/*SF CTL APIs	*/
void mssf_set_msmode(unsigned int mode)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= ~(MSSF_CTL_MODE); 
	*((volatile unsigned *)(MSSF_CTL)) = (data|mode);
}

/*[value] 1: read mode , 0: write mode*/
void mssf_msreg_rw_switch(unsigned int value)
{
        unsigned int data;
        data = *((volatile unsigned *)(MSSF_CTL));
        data &= ~(MSSF_CTL_REGRW);
        data |= (value<<3);
        *((volatile unsigned *)(MSSF_CTL)) = data;
}

/*[value] 1: enable DMA , 0: disable DMA*/
void mssf_msdma_en_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= ~(MSSF_CTL_DMAEN);
	data |= (value <<4);
	*((volatile unsigned *)(MSSF_CTL)) = data;	
}

/*[value] 1: read mode , 0: write mode*/
void mssf_msdma_rw_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= ~(MSSF_CTL_DMARW);
	data |= (value <<5);
	*((volatile unsigned *)(MSSF_CTL)) = data;	
}

/*[value] 1: enable Extra ECC DATA enable , 0: disable*/
void mssf_extra_en_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= ~(MSSF_CTL_EXTRAEN);
	data |= (value <<6);
	*((volatile unsigned *)(MSSF_CTL)) = data;	
}

/*[value] 1: enable ECC or CRC, 0: disable*/
void mssf_ecc_en_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= ~(MSSF_CTL_ECCEN);
	data |= (value <<7);
	*((volatile unsigned *)(MSSF_CTL)) = data;	
}

/*return 1: ready*/
int mssf_check_msrdy(void)
{
	int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= MSSF_CTL_MSRDY;
	data = (data>> 8);
	return data;
}

//enable calculate CRC code
/*[value] 1: calculate crc code , 0: not calculate crc*/
void mssf_crc_cal_switch(unsigned int value)
{
        unsigned int data;
        data = *((volatile unsigned *)(MSSF_CTL));
        data &= ~(MSSF_CTL_CRC_CAL);
        data |= (value <<9);   
        *((volatile unsigned *)(MSSF_CTL)) = data;
}
                                       
//for sf normal mode and spi mode
void mssf_set_wmode(unsigned int sfwmode)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_DMA_SIZE));
	data &= ~(MSSF_CTL_W_MODE);
	data |= (sfwmode <<17);	
	*((volatile unsigned *)(MSSF_DMA_SIZE)) = data;	
}

void mssf_set_msspeed(unsigned int msspeed)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_CTL));
	data &= ~(MSSF_CTL_MSSPEED);
	data |= (msspeed <<24);	
	*((volatile unsigned *)(MSSF_CTL)) = data;	
}

/*0x04*/
/*SF DMA_SIZE APIs*/
void mssf_set_dmasize(unsigned int size)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_DMA_SIZE));
	data &= ~(MSSF_DMA_SIZE_MSDMASIZE);	
	data |= (size <<0);
	*((volatile unsigned *)(MSSF_DMA_SIZE)) = data;	
}

//0x08
/*SF CRC APIs*/
unsigned int mssf_read_crc16(void)
{
        unsigned int data;
        data = *((volatile unsigned *)(MSSF_CRC));
        data &= (MSSF_CRC_CRC16_L | MSSF_CRC_CRC16_H);
        data = (data>>0);
        return data;
}
                                        
//0x34, 0x38
void mssf_ms_io_oe_switch(unsigned int value)
{
	unsigned int data; 	
	data = *((volatile unsigned *)(MSSF_MS_IO_OE));
	data &= ~(MSSF_MS_IO_OE_8);
	data |= (value << 8);
	*((volatile unsigned *)(MSSF_MS_IO_OE)) = data;	
}


//for sfgpio
void mssf_ms_io_wt (unsigned int num,unsigned int mode ,unsigned int value)
{
	unsigned int data;

	if (mode == 1){
		data = *((volatile unsigned *)(MSSF_MS_IO_OE));
		data |= (1<<num);
		*((volatile unsigned *)(MSSF_MS_IO_OE)) = data;	

		data = *((volatile unsigned *)(MSSF_MS_IO_O));
		if (value==1){
			data |= (1<<num);
		}
		else{
			data &= ~(1<<num);
		}

		*((volatile unsigned *)(MSSF_MS_IO_O)) = data;	
	}
	else{
		data = *((volatile unsigned *)(MSSF_MS_IO_OE));
		data &= ~(1<<num);
		*((volatile unsigned *)(MSSF_MS_IO_OE)) = data;	
	}
}

unsigned int mssf_ms_io_rd (unsigned int num)
{
	unsigned int data_oe,data_i;
	unsigned int rdata;

	data_oe = *((volatile unsigned *)(MSSF_MS_IO_OE));
	data_oe = (data_oe>>num)&0x01;

	data_i = *((volatile unsigned *)(MSSF_MS_IO_I));
	data_i = (data_i>>num)&0x01;

	rdata = (data_oe<<1)|data_i;

	return rdata;
}

void mssf_ms_io_release(void)
{
	*((volatile unsigned *)(MSSF_MS_IO_OE)) = 0;	
	*((volatile unsigned *)(MSSF_MS_IO_O)) = 0;
}


void mssf_chip_enable_switch(unsigned int value)
{
        
        unsigned int data;
        //mssf_ms_io_oe_switch(MSSF_ENABLE);//chip enable or disable in ms_io output mode
	data = *((volatile unsigned *)(MSSF_MS_IO_OE));
	data &= ~(MSSF_MS_IO_OE_8);
	data |= (1 << 8);
	*((volatile unsigned *)(MSSF_MS_IO_OE)) = data;	
	data =0;
        data = *((volatile unsigned *)(MSSF_MS_IO_O));
        data &= ~(MSSF_MS_IO_O_8);
        data |= (value<<8);
        *((volatile unsigned *)(MSSF_MS_IO_O)) = data;
}

//0x04
void mssf_set_wcmd(unsigned int sfCmd)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_DMA_SIZE));
	data &= ~(MSSF_W_CMD_AAWDMA);	
	data |= (sfCmd <<24);
	*((volatile unsigned *)(MSSF_DMA_SIZE)) = data;	
}

//0x48
//DMA_BLKSU
void mssf_dmablock(unsigned int dmaBlockNum)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_DMA_BLKSU));
	data &= ~(MSSF_DMA_BLKSU_BLK);
	data |= (dmaBlockNum <<0);
	*((volatile unsigned *)(MSSF_DMA_BLKSU)) = data;	
}

unsigned int mssf_read_sudmablock(void)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_DMA_BLKSU));
	data &= MSSF_DMA_BLKSU_SUBLK;
	data = (data>> 16);
	return data;
}

//0x4c
//TMCNT
void mssf_set_timecount(unsigned int timeCnt)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_TMCNT));		
	data &= ~(MSSF_TMCNT_TMCNT);								 
	data |= (timeCnt <<0); 
	*((volatile unsigned *)(MSSF_TMCNT)) = data;	 
}

 //0x50
//MDMAECC
void mssf_msmdma_en_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &= ~(MSSF_MDMAECC_MS_M_DMA_EN);
	data |= (value >>0);
	*((volatile unsigned *)(MSSF_MDMAECC)) = data;	
}

int mssf_check_msmdma_ok(void)
{
	int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &=MSSF_MDMAECC_MS_M_DMA_OK;
	data =(data>>1);
	return data;	
}

int mssf_check_msmdma_timeout(void)
{
	int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &=MSSF_MDMAECC_MS_M_DMA_TIME_OUT;
	data =(data>>2);
	return data;	
}

void mssf_msrdy_Intr_en_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &= ~(MSSF_MDMAECC_MS_RDY_INTR_EN);
	data |= (value <<24);
	*((volatile unsigned *)(MSSF_MDMAECC)) = data;	
}

int mssf_check_msrdy_flag(void)
{
	int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &=MSSF_MDMAECC_MS_RDY_FLAG;
	data =(data>>25);
	return data;	
}

//write 1 to clear
void mssf_clear_msrdy_flag(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	//data &= ~(MSSF_MDMAECC_MS_RDY_FLAG);
	data |= (value>>24);
	*((volatile unsigned *)(MSSF_MDMAECC)) = data;	
} 

void mssf_mserr_Intr_en_switch(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &= ~(MSSF_MDMAECC_MS_ERR_INTR_EN);
	data |= (value <<26);
	*((volatile unsigned *)(MSSF_MDMAECC)) = data;	
}

int mssf_check_mserr_flag(void)
{
	int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	data &=MSSF_MDMAECC_MS_ERR_FLAG;
	data =(data>>27);
	return data;	
}

//write 1 to clear
void mssf_clear_mserr_flag(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_MDMAECC));
	//data &= ~(MS_SD_ERR_FLAG_CLEAR);
	data |= (1>>25);
	*((volatile unsigned *)(MSSF_MDMAECC)) = data;	
} 

//0x54
//LBA 
void mssf_set_lba(unsigned int value)
{
	unsigned int data;
	data = *((volatile unsigned *)(MSSF_LBA));
	data &= ~(MSSF_LBA_LBAW);
	data |= (value << 9);
	*((volatile unsigned *)(MSSF_LBA)) = data;	
}

//0x5c
void mssf_set_dmaaddr(unsigned int addr)
{
	*((volatile unsigned *)(MSSF_DMA_ADDR)) = addr;
}

//0x74
void mssf_write_data(unsigned int value)
{
      *((volatile unsigned *)(MSSF_REG_DATA)) = value;
}

unsigned int mssf_read_data(void)
{
	unsigned int value;
	value = *((volatile unsigned *)(MSSF_REG_DATA));
	return value;
}	

