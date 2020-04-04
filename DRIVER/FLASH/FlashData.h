#ifndef __FLASHDATA_H
#define __FLASHDATA_H

#include "stm32f10x_flash.h"
//#define FLASH_START_ADDR1 0x8000000+2048*255
//#define FLASH_START_ADDR2 0x8000000+(2048*254)
//#define FLASH_START_ADDR3 0x8000000+(2048*253)
#define FLASH_START_ADDR1 0x8000000+1024*63
#define FLASH_START_ADDR2 0x8000000+(1024*61)
#define FLASH_START_ADDR3 0x8000000+(1024*64)
void FLASH_WriteByte(u32 addr, u16 flashdata1);
u16 ReadFlash(u32 addr);

#endif

