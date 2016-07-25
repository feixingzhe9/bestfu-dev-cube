/*******************************************************************
文	件：    program.h
说	明：    STM32_Flash烧入
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-21
********************************************************************/
#ifndef __PROGRAM_H
#define __PROGRAM_H

#include "BF_type.h"
#include "flash.h"

#define PAGE_SIZE   (0x0100) //256bytes
#define APP_SIZE	0x7C00	//应用程序空间 31K

u8 Program_Erase(u32 addr, u32 size);
u8 Program_Read(u32 addr, u32 len, u8 *data);
u8 Program_Write(u32 addr, u32 len, u8 *data);
u32 Flash_DefaultValue(void);
u8 Program_Write_Ext(u32 addr, u32 len, u8 *data);
u8 Program_Read_Ext(u32 addr, u32 len, u8 *data);
#if (FLASH_ENCRYPTION_EN > 0u)
void FlashEncryptionInit(void);
#endif
#endif //program.h
