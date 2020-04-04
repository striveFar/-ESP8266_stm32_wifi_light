/******************************************************************
 * 文件：TLink.c
 * 功能：实现TLink云平台的TCP连接相关函数
 * 日期：2018-04-11
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "stdlib.h"
#include "TLink.h"
#include "NetWork.h"
#include "LIGHT/Light.h"
#include "stdio.h"
#include "DELAY/Delay.h"
#include "string.h"
#include "LED/LED.h"

extern u8 RXBuffer[RXBUFFER_LEN];
DeviceSta_Strcture device = {0};
/**
 * 功能：连接TLink云平台
 * 参数：None
 * 返回值：非0表示连接成功,0表示失败
 */
u8 connectTlink(void)
{
    u8 sta = 0;

    sta = connectServer(TCP, TLINK_IP, TLINK_PORT); //连接服务器

    if (sta != 0)
    {
        printf("\r\nLinked TLink!");
        sendBuffertoServer(APP_ID); //发送注册序号
    }
    else
    {
        printf("\r\nLink fail!");
    }

    return sta;
}

/**
 * 功能：发送设备状态(传感器数据以及继电器状态)
 * 参数：pdevsta 设备参数结构体指针
 * 返回值：None
 */
void sendDeviceStatus(DeviceSta_Strcture *pdevsta)
{
    char buffer[60] = {0};

    /*格式化数据帧*/
    sprintf(buffer, "FM:%d,%d,%d,%d,%d,%d,%d,%d,%d.#", pdevsta->relay_bed, pdevsta->relay_living, pdevsta->relay_kitchen, 0, 0, pdevsta->Lux, pdevsta->Ctrl_bed, pdevsta->Ctrl_living, pdevsta->Ctrl_kitchen);
    // sprintf(buffer, "FM:%d,%d,%d,%d,%d,%d,%d,%d,%d.#", 0, 0, 0, 0, 0, pdevsta->Lux, 0, 0, 0);

    /*发送数据到TLink服务器*/
    sendBuffertoServer(buffer);
}

/**
 * 功能：处理从服务器接收的控制数据
 * 参数：pdevsta 设备参数结构体指针
 * 返回值：None
 * 说明：该函数应该被串口空闲中断调用,已达到最佳实时性能
 */
void processDeviceStatus(DeviceSta_Strcture *pdevsta)
{
    u8 i = 0;

    /*掉线重连*/
    if ((u8)strstr(RXBuffer, "CLOSE"))
    {
        printf("\r\nDisconnected!");
        connectTlink();
        memset(RXBuffer, 0, RXBUFFER_LEN);
        return;
    }
    /*对应开关控制*/
    if ((u8)strstr(RXBuffer, "bedroom_LIGHT_ON"))
    {
        i = 1;
        pdevsta->relay_bed = 1;
        bed_flag = 1;
        pdevsta->Brightness = 50; //初始亮度为50
        setLight(pdevsta->Brightness);
    }
    if ((u8)strstr(RXBuffer, "bedroom_LIGHT_OFF"))
    {
        i = 1;
        pdevsta->relay_bed = 0;
        pdevsta->Brightness = 0; //亮度为0
        setLight(pdevsta->Brightness);
        bed_flag = 0;
    }
    if ((u8)strstr(RXBuffer, "living_LIGHT_ON"))
    {
        i = 1;
        pdevsta->relay_living = 1;
        living_flag = 1;
        pdevsta->Brightness = 50; //初始亮度为50
        setLight(pdevsta->Brightness);
    }
    if ((u8)strstr(RXBuffer, "living_LIGHT_OFF"))
    {
        i = 1;
        pdevsta->relay_living = 0;
        pdevsta->Brightness = 0; //亮度为0
        setLight(pdevsta->Brightness);
        living_flag = 0;
    }
    if ((u8)strstr(RXBuffer, "kitchen_LIGHT_ON"))
    {
        i = 1;
        pdevsta->relay_kitchen = 1;
        kitchen_flag = 1;
        pdevsta->Brightness = 50; //初始亮度为50
        setLight(pdevsta->Brightness);
    }
    if ((u8)strstr(RXBuffer, "kitchen_LIGHT_OFF"))
    {
        i = 1;
        pdevsta->relay_kitchen = 0;
        pdevsta->Brightness = 0; //亮度为0
        setLight(pdevsta->Brightness);
        kitchen_flag = 0;
    }
    /*对应调节or保存模式*/

    if ((u8)strstr(RXBuffer, "ctrl_bed"))
    {
        i = 1;
        ctrl_bed = 1;
        pdevsta->Ctrl_bed = 1; //卧室灯处于调节模式
    }
    if ((u8)strstr(RXBuffer, "save_bed"))
    {
        i = 1;
        ctrl_bed = 0; //卧室灯状态保存
        pdevsta->Ctrl_bed = 0;
    }
    if ((u8)strstr(RXBuffer, "ctrl_living"))
    {
        i = 1;
        ctrl_living = 1;
        pdevsta->Ctrl_living = 1; //客厅灯处于调节模式
    }
    if ((u8)strstr(RXBuffer, "save_living"))
    {
        i = 1;
        ctrl_living = 0;
        pdevsta->Ctrl_living = 0; //客厅灯状态保存
    }

    if ((u8)strstr(RXBuffer, "ctrl_kitchen"))
    {
        i = 1;
        ctrl_kitchen = 1; //厨房灯处于调节模式
        pdevsta->Ctrl_kitchen = 1;
    }
    if ((u8)strstr(RXBuffer, "save_kitchen"))
    {
        i = 1;
        ctrl_kitchen = 0; //厨房灯状态保存
        pdevsta->Ctrl_kitchen = 0;
    }

    /*对应亮度调节*/
    if ((u8)strstr(RXBuffer, "BRIGHTNESSUP"))
    {
        i = 1;

        if ((bed_flag && ctrl_bed) | (living_flag && ctrl_living) | (kitchen_flag && ctrl_kitchen)) //灯打开且为控制模式时允许调节亮度
        {
            pdevsta->Brightness += 10;
            if (pdevsta->Brightness >= 100)
            {
                pdevsta->Brightness = 100; //最亮为100
            }
            setLight(pdevsta->Brightness);
            pdevsta->Lux = device.Lux;
        }
    }

    if ((u8)strstr(RXBuffer, "BRIGHTNESSDOWN"))
    {
        i = 1;
        if ((bed_flag && ctrl_bed) | (living_flag && ctrl_living) | (kitchen_flag && ctrl_kitchen)) //灯打开且为控制模式时允许调节亮度
        {
            pdevsta->Brightness -= 10;
            if (pdevsta->Brightness <= 10)
            {
                pdevsta->Brightness = 10; //最暗为10
            }
            setLight(pdevsta->Brightness);
            pdevsta->Lux = device.Lux;
        }
    }

    /*清除接收缓冲并更新继电器状态到服务器*/
    if (i != 0)
    {
        printf("RXBuffer is %s.", RXBuffer);
        memset(RXBuffer, 0, RXBUFFER_LEN);
        Delay_ms(200);
        //blinkLED(3, 300, 300);
        sendDeviceStatus(pdevsta);
    }
}
