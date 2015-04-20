/*
 * linux/include/asm-arm/arch-sn986xx/regs-mssf.h
 *
 * Copyright (c) 2008 SONIX Limited.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Nora 10/14/2008 created
*/

#ifndef __ASM_ARCH_REGS_MTDRAM_H
#define __ASM_ARCH_REGS_MTDRAM_H

#include <mach/platform.h>

/*
 * SONiX Mass Storage Controller (serial flash)  virtual addresses
*/
#define		MSSF_CTL				IO_ADDRESS(SNX_MSSF_CTL)
#define 	MSSF_CTL_MODE				0x00000003
#define 	MSSF_CTL_REGRW				0x00000008
#define 	MSSF_CTL_DMAEN          		0x00000010
#define 	MSSF_CTL_DMARW          		0x00000020
#define 	MSSF_CTL_EXTRAEN        		0x00000040
#define 	MSSF_CTL_ECCEN          		0x00000080
#define 	MSSF_CTL_MSRDY          		0x00000100
#define 	MSSF_CTL_CRC_CAL                        0x00000200
#define 	MSSF_CTL_W_MODE         		0x00060000
#define 	MSSF_CTL_MSSPEED        		0xff000000
#define  	MSSF_DMA_SIZE				IO_ADDRESS(SNX_MSSF_DMA_SIZE)
#define 	MSSF_DMA_SIZE_MSDMASIZE			0x00000fff
#define		MSSF_CRC				IO_ADDRESS(SNX_MSSF_CRC)			
#define 	MSSF_CRC_CRC16_L                        0x000000ff
#define 	MSSF_CRC_CRC16_H                        0x0000ff00
#define		MSSF_MS_IO_I				IO_ADDRESS(SNX_MSSF_MS_IO_I)		
#define 	MSSF_MS_IO_I_I				0x00007fff
#define		MSSF_MS_IO_O				IO_ADDRESS(SNX_MSSF_MS_IO_O)		
#define 	MSSF_MS_IO_O_O                  	0x00007fff
#define 	MSSF_MS_IO_O_0 	                	0x00000001
#define 	MSSF_MS_IO_O_8 	                	0x00000100
#define		MSSF_MS_IO_OE				IO_ADDRESS(SNX_MSSF_MS_IO_OE)		
#define 	MSSF_MS_IO_OE_OE                	0x00007fff
#define 	MSSF_MS_IO_OE_0 	               	0x00000001
#define	MSSF_MS_IO_OE_8 	               	0x00000100
#define        	MSSF_SF_WCMD				IO_ADDRESS(SNX_MSSF_SF_WCMD)
#define 	MSSF_W_CMD_AAWDMA                       0xff000000
#define  	MSSF_DMA_BLKSU				IO_ADDRESS(SNX_MSSF_DMA_BLKSU)		
#define 	MSSF_DMA_BLKSU_BLK        	     	0x0000ffff
#define 	MSSF_DMA_BLKSU_SUBLK            	0xffff0000
#define  	MSSF_TMCNT				IO_ADDRESS(SNX_MSSF_TMCNT)		
#define 	MSSF_TMCNT_TMCNT               		0x3fffffff
#define		MSSF_MDMAECC				IO_ADDRESS(SNX_MSSF_MDMAECC)		
#define 	MSSF_MDMAECC_MS_M_DMA_EN    		0x00000001
#define 	MSSF_MDMAECC_MS_M_DMA_OK    		0x00000002
#define 	MSSF_MDMAECC_MS_M_DMA_TIME_OUT 		0x00000004
#define		MSSF_MDMAECC_MS_RDY_INTR_EN 		(0x1<<8)
#define 	MSSF_MDMAECC_MS_RDY_FLAG		(0x1<<16)
#define	MSSF_MDMAECC_MS_RDY_clear		(0x1<<24)
#define 	MSSF_MDMAECC_MS_ERR_INTR_EN     	(0x1<<9)
#define 	MSSF_MDMAECC_MS_ERR_FLAG     		(0x1<<17)
#define  MS_SD_ERR_FLAG_CLEAR            (0x1<<25) 
#define  	MSSF_LBA				IO_ADDRESS(SNX_MSSF_LBA)			
#define 	MSSF_LBA_LBAW                   	0x007ffe00
#define  	MSSF_DMA_ADDR	 			IO_ADDRESS(SNX_MSSF_DMA_ADDR)
#define   	MSSF_REG_DATA				IO_ADDRESS(SNX_MSSF_REG_DATA)		

/*
 * SONiX Mass Storage Controller (serial flash) physical addresses
 */
#define SNX_MSSFREG(x) (SNX_MS1_BASE + (x))

#define		SNX_MSSF_CTL			SNX_MSSFREG(MSSF_CTL_OFFSET)
#define        	SNX_MSSF_DMA_SIZE		SNX_MSSFREG(MSSF_DMA_SIZE_OFFSET)
#define        	SNX_MSSF_CRC			SNX_MSSFREG(MSSF_CRC_OFFSET)
#define        	SNX_MSSF_MS_IO_I		SNX_MSSFREG(MSSF_MS_IO_I_OFFSET)
#define        	SNX_MSSF_MS_IO_O		SNX_MSSFREG(MSSF_MS_IO_O_OFFSET)
#define        	SNX_MSSF_MS_IO_OE		SNX_MSSFREG(MSSF_MS_IO_OE_OFFSET)
#define        	SNX_MSSF_SF_WCMD		SNX_MSSFREG(MSSF_SF_WCMD_OFFSET)
#define        	SNX_MSSF_DMA_BLKSU		SNX_MSSFREG(MSSF_DMA_BLKSU_OFFSET)
#define        	SNX_MSSF_TMCNT		SNX_MSSFREG(MSSF_TMCNT_OFFSET)
#define        	SNX_MSSF_MDMAECC		SNX_MSSFREG(MSSF_MDMAECC_OFFSET)
#define        	SNX_MSSF_LBA			SNX_MSSFREG(MSSF_LBA_OFFSET)
#define        	SNX_MSSF_DMA_ADDR		SNX_MSSFREG(MSSF_DMA_ADDR_OFFSET)
#define        	SNX_MSSF_REG_DATA		SNX_MSSFREG(MSSF_REG_DATA_OFFSET)

/* Mass Storage controller registers (serial flash) offset */
#define        MSSF_CTL_OFFSET			  0x00  
#define        MSSF_DMA_SIZE_OFFSET		  0x04  
#define        MSSF_CRC_OFFSET                    0x08          
#define        MSSF_MS_IO_I_OFFSET                0x30
#define        MSSF_MS_IO_O_OFFSET                0x34
#define        MSSF_MS_IO_OE_OFFSET               0x38  
#define        MSSF_SF_WCMD_OFFSET                0x40
#define        MSSF_DMA_BLKSU_OFFSET              0x48  
#define        MSSF_TMCNT_OFFSET                  0x4c  
#define        MSSF_MDMAECC_OFFSET                0x50
#define        MSSF_LBA_OFFSET                    0x54  
#define        MSSF_DMA_ADDR_OFFSET               0x5c
#define        MSSF_REG_DATA_OFFSET               0x74

#endif /* __ASM_ARCH_REGS_MSSF_H */
