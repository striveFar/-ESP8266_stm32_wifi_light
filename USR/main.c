/******************************************************************
 * 文件：main.c
 * 功能：主函数入口
 * 日期：2018-02-16
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 官博：http://fengmeitech.club
 * Copyright (C) 2017 zx. All rights reserved.
*******************************************************************/
#include "stdio.h"
#include "string.h"
#include "DELAY/Delay.h"
#include "ADC/ADC.h"
#include "LED/LED.h"
#include "NVIC/NVIC.h"
#include "IIC/IIC.h"
#include "OLED/OLED.h"
#include "DHT11/DHT11.h"
#include "UART/uart.h"
#include "NetWork.h"
#include "LIGHT/Light.h"
#include "TIMx/TIMx.h"
#include "DMA/DMA.h"
#include "TLink.h"
#include "FLASH/FlashData.h"

extern u16 RGB_buffer[69];
//DeviceSta_Strcture device = {0};

int main(void)
{
    SystemInit();
    u8 i = 0;
    u16 datafalsh = 0;
    /*初始化各外设*/
    initSysTick();
    initADC();
    initUART();
    initUART2();
    initLED();
    closeLED();
    initDHT11();
    initIIC();
    initOLED();
    initNVIC(NVIC_PriorityGroup_2); //开启UART2的接收和空闲中断
    initTIMx(TIM3, 7199, 9999, TIM_IT_Update, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /*设置定时器2时钟为10us,1KHz*/
    initTIMx(TIM2, 719, 100, TIM_IT_Update, DISABLE);
    initTIM2OC1(100);
    initTIM2OC2(100);

    /*设置定时器4时钟为10us,1KHz*/
    initTIMx(TIM4, 719, 100, TIM_IT_Update, DISABLE);
    initTIM4OC1(100);

    /*初始亮度为历史值*/
    initLight();

    formatScreen(0x00); //清屏

    /*打印ESP8266启动信息到OLED*/
    if (initESP8266() != 0)
    {
        showString(0, 0, "init ok!", FONT_16_EN);
    }
    else
    {
        showString(0, 0, "init error!", FONT_16_EN);
    }
    if (connectAP(SSID, PWD) != 0)
    {
        showString(0, 2, "connect AP ok!", FONT_16_EN);
    }
    else
    {
        showString(0, 2, "connect AP error!", FONT_16_EN);
    }
    if (connectTlink() != 0)
    {
        showString(0, 4, "connect TLink ok!", FONT_16_EN);
    }
    else
    {
        showString(0, 4, "connect TLink no!", FONT_16_EN);
    }

    Delay_ms(1000); //让数据保持一段时间
    formatScreen(0x00);
    showString(0, 0, "WIFI Smart Lamp", FONT_16_EN);
    showString(0, 2, "birghtless:", FONT_16_EN);
    showString(0, 4, "Fla:", FONT_16_EN);
    showString(0, 6, "Lux :", FONT_16_EN);
    device.Brightness = 0;
    while (1)
    {
        /*数据采集显示*/
        if (++i > 30)
        {
            i = 0;

            datafalsh = ReadFlash(FLASH_START_ADDR1);
            showNumber(40, 4, datafalsh, DEC, 3, FONT_16_EN); //读flash，显示亮度信息

            datafalsh = ReadFlash(FLASH_START_ADDR2);
            showNumber(70, 4, datafalsh, DEC, 3, FONT_16_EN); //读flash，显示亮度信息

            datafalsh = ReadFlash(FLASH_START_ADDR3);
            showNumber(100, 4, datafalsh, DEC, 3, FONT_16_EN); //读flash，显示亮度信息

            showNumber(40, 6, device.Lux, DEC, 4, FONT_16_EN); //ADC读值，显示光照强度
        }

        Delay_ms(10);
    }
}

void TIM3_IRQHandler(void)
{

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        device.Lux = getConvValueAve(10, 1000);
        sendDeviceStatus(&device);
    }
}
