
#include "FLASH/FlashData.h"
#include "UART/uart.h"
#include <stdio.h>
#include "string.h"
#include "IIC/IIC.h"
#include "OLED/OLED.h"


/**
 * 功能：写入flash
 * 基本顺序：解锁->清除标志位（可以不要）->擦除->写半字->清楚标志位（也可以不要）->上锁。 
 * 参数：addr : flash地址
 *      参数值：FLASH_START_ADDR1
 *              FLASH_START_ADDR2
 *               FLASH_START_ADDR3
 *      flashdata1：待写入的数据 ：半字
 * 
 * 返回值：None
 */
void FLASH_WriteByte(u32 addr, u16 flashdata1)
{
    FLASH_Status FLASHstatus = FLASH_COMPLETE;
    FLASH_Unlock(); //解锁FLASH编程擦除控制器
                    // FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清除标志位
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    //********************************************************************
    //   FLASH_FLAG_BSY            FLASH忙标志位
    //    FLASH_FLAG_EOP            FLASH操作结束标志位
    //      FLASH_FLAG_PGERR          FLASH编写错误标志位
    //       FLASH_FLAG_WRPRTERR       FLASH页面写保护错误标净
    //***********************************************************************/
    FLASHstatus = FLASH_ErasePage(addr);                   //擦除指定地址页
    FLASHstatus = FLASH_ProgramHalfWord(addr, flashdata1); //从指定页的addr地址开始写
    //FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
    FLASH_Lock(); //锁定FLASH编程擦除控制
}

/**
 * 功能：读ReadFlash
 * addr1: 对应flash地址
 *      参数值：FLASH_START_ADDR1
 *              FLASH_START_ADDR2
 *               FLASH_START_ADDR3
 * 返回值：u16 读到的数据
 */  
u16 ReadFlash(u32 addr1)
{
    u16 rdata = 0;
    rdata = *(u16 *)addr1;
    return rdata;
}


