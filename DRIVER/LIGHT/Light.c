/******************************************************************
 * 文件：Light.c
 * 功能：定义Light相关函数
 * 日期：2018-03-14
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "LIGHT/Light.h"
#include "DELAY/Delay.h"
#include "TIMx/TIMx.h"
#include "FLASH/FlashData.h"

u16 RGB_buffer[69] = {0};

u8 bed_flag = 0;
u8 living_flag = 0;
u8 kitchen_flag = 0;
u8 ctrl_bed = 0;
u8 ctrl_living = 0;
u8 ctrl_kitchen = 0;
/**
 * 功能：初始灯光
 * 参数：
 *     brightness:初始化亮度 10-100         
 * 返回值：None
 */
void initLight(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //禁止JTAG保留SWD
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);    //设置JTAG为定时器2部分映射,只使用SWD模式

    /*设置冷光灯*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*设置暖光灯*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*设置RGB*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //初次开启时使用历史亮度

    ReserveValue(1, 0, 0, FLASH_START_ADDR1);
    ReserveValue(0, 1, 0, FLASH_START_ADDR2);
    ReserveValue(0, 0, 1, FLASH_START_ADDR3);
    ReserveValue(0, 0, 0, FLASH_START_ADDR3); //此处调用是为了复位flag
}

/**
 * 功能：按照历史值初始化各个灯的亮度
 * 参数：
 *      bed:      开关标志用于设置亮度的判断 
 *      living:   开关标志用于设置亮度的判断
 *      kitchen:  开关标志用于设置亮度的判断
 *      addr：    历史值存放的flash地址
 * 返回值：None
 */
void ReserveValue(u8 bed, u8 living, u8 kitchen, u32 addr)
{
    u8 historybright = 0;
    bed_flag = bed;
    living_flag = living;
    kitchen_flag = kitchen;
    if (bed_flag | living_flag | kitchen_flag)
    {
        historybright = ReadFlash(addr);
        setLight(historybright);
    }
}

/**
 * 功能：根据开关/控制标志设置灯光亮度
 * 参数：
 *          brightness:亮度 10-100
 *            
 * 返回值：None
 */
void setLight(u8 brightness) //捕获寄存器中的值 < 自动重装在寄存器中的值时为有效电平
{
    /**
     * 无论是亮度还是色温，最终都体现在LED的亮度上
     * LED最终的亮度计算公式为：满占空比(重装载值) * 亮度百分比 * 色温百分比
     * 并且要保证冷光和暖光的色温比值之和为100%
     * */
    if (bed_flag | ctrl_bed) //灯打开或者处于控制模式 都需要用到亮度设置
    {
        TIM_SetCompare1(TIM2, getPeriod(TIM2) * brightness / 100); //设置卧室灯 对应PA15 TIM2_CH1
        FLASH_WriteByte(FLASH_START_ADDR1, brightness);            //卧室灯亮度存入flash
    }
    if (living_flag | ctrl_living)
    {
        TIM_SetCompare2(TIM2, getPeriod(TIM2) * brightness / 100); //设置客厅灯 对应PB3  TIM2_CH2
        FLASH_WriteByte(FLASH_START_ADDR2, brightness);            //客厅灯亮度存入flash
    }
    if (kitchen_flag | ctrl_kitchen)
    {
        TIM_SetCompare1(TIM4, getPeriod(TIM4) * brightness / 100); //设置厨房灯 对应PB6  TIM4_CH1
        FLASH_WriteByte(FLASH_START_ADDR3, brightness);            //厨房灯亮度存入flash
    }
}

