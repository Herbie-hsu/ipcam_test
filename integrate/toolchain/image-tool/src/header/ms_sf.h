#ifndef __SF_H__ 
#define __SF_H__ 


#define BASE_MS1            (0x90900000)
#ifndef ENABLE
#define ENABLE	1
#endif

#ifndef DISABLE
#define	DISABLE	0
#endif

//--------------------------------
//Register definition!
//--------------------------------
#define	SF_CTL      		0x00	
#define	SF_DMA_SIZE			0x04 	
#define	SF_CRC				0x08
#define	SF_MS_IO_I			0x30
#define	SF_MS_IO_O			0x34
#define	SF_MS_IO_OE			0x38	
#define	SF_DMA_BLKSU		0x48
#define	SF_TMCNT			0x4c	
#define	SF_MDMAECC			0x50
#define	SF_LBA				0x54	
#define	SF_DMA_ADDR			0x5c
#define	SF_REG_DATA			0x74

//--------------------------------
//SF_CTL 0x00 				
//--------------------------------
#define SF_CTL_MODE			0x00000007
#define SF_CTL_REGRW		0x00000008
#define SF_CTL_DMAEN		0x00000010
#define SF_CTL_DMARW		0x00000020
#define SF_CTL_EXTRAEN		0x00000040
#define SF_CTL_ECCEN		0x00000080
#define SF_CTL_MSRDY		0x00000100
#define SF_CTL_CRCEN		0x00000200
#define SF_CTL_MSSPEED		0xff000000

//--------------------------------
//SF_DMA_SIZE 0x04
//--------------------------------		
#define SF_DMASIZE_MSDMASIZE	0x00000fff
#define SF_CTL_W_MODE			0x00060000
#define SF_W_CMD_AAWDMA 		0xff000000	

//--------------------------------
//SF_CRC 0x08
//--------------------------------
#define SF_CRC_CRC16_L			0x000000ff
#define SF_CRC_CRC16_H			0x0000ff00

//--------------------------------
//SD_MS_IO 0x30 ~ 0x38   
//--------------------------------
#define SF_MS_IO_I_I			0x00007fff	
#define SF_MS_IO_O_O			0x00007fff	
#define SF_MS_IO_OE_OE			0x00007fff	
#define SF_MS_IO_O_8			0x00000100 // CS
#define SF_MS_IO_OE_8			0x00000100 // CS

//--------------------------------
//SF_DMA_BLKSU 0x48
//--------------------------------
#define SF_BLKSU_BLK			0x0000ffff
#define SF_BLKSU_SUBLK			0xffff0000

//--------------------------------
//SD_TMCNT 0x4c		  
//--------------------------------
#define SF_TMCNT_TMCNT			0x3fffffff	

//--------------------------------
//SF_MDMAECC 0x50
//--------------------------------
#define SF_MS_M_DMA_EN			0x00000001
#define SF_MS_M_DMA_OK			0x00000002
#define SF_MS_M_DMA_TIME_OUT	0x00000004

#define SF_MS_RDY_INTR_EN		0x00000100 
#define SF_MS_ERR_INTR_EN		0x00000200

#define SF_MS_RDY_FLAG			0x00010000
#define SF_MS_ERR_FLAG			0x00020000

#define SF_CLR_MS_RDY			0x01000000
#define SF_CLR_MS_ERR			0x02000000
#define SF_CLR_ECC_ERR			0x04000000

//-------------------------------
//SF LBA sd useful bits 0x54
//-------------------------------
#define SF_LBA_LBAW			0x007ffe00	

void sfSetMsMode (unsigned mode); 
void sfMsRegRWSwitch (u32 value); 
void sfMsDmaENSwitch (u32 isEnable);
void sfMsDmaRWSwitch (u32 value);
void sfExtraENSwitch (u32 isEnable);
void sfEccENSwitch (u32 isEnable);
int sfCheckMsRdy (void);
void sfCrcENSwitch(u32 isEnable);
void sfSetWMode (u32 sfwmode);
void sfSetMsSpeed (u32 msspeed);
void sfSetDmaSize (u32 size);
u32 sfReadCrc16 (void);
void sfSetLba (u32 value);
void sfSetWCmd (u32 sfCmd);
void sfDmaBlock (u32 dmaBlockNum);
u32 sfReadSUDmaBlock (void);
void sfSetTimeCount (u32 timeCnt);
void sfSetReadcmdCount (u32 readCmdCnt);
void sfMsMDmaENSwitch (u32 isEnable); 
int sfCheckMsMDmaOk (void);
int sfCheckMsMDmaTimeOut (void);
void sfMsRdyIntrENSwitch (u32 isEnable);
int sfCheckMsRdyFlag (void);
void sfClearMsRdyFlag (u32 value);
void sfMsErrIntrENSwitch (u32 isEnable);
int sfCheckMsErrFlag (void); 
void sfClearMsErrFlag (u32 value);
void sfSetDmaAddr (u32 addr);
void sfWriteData (u32 value);
u32 sfReadData (void);
void sfMsGpioOE0Switch (u32 isEnable);
void sfMsGpioO0Switch (u32 isEnable);
void sfChipDisable (void);
void sfChipEnable (void);

#endif
