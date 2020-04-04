/******************************************************************
 * 文件：TLink.h
 * 功能：声明TLink云平台的TCP连接相关函数
 * 日期：2018-04-11
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#ifndef __TLINK_H
#define __TLINK_H
#include "stm32f10x.h"

#define TLINK_IP    "tcp.tlink.io"
#define TLINK_PORT  8647
#define APP_ID      "BB9D0MPM7I0CN216"

typedef struct 
{
    u8 Brightness; //set 亮度用，不上传
    //以下需上传
    u8 relay_bed;
    u8 relay_living;
    u8 relay_kitchen;

    u8 Ctrl_bed;
    u8 Ctrl_living;
    u8 Ctrl_kitchen;

    u16 Lux;
}DeviceSta_Strcture;

extern DeviceSta_Strcture device;
u8 connectTlink(void);
void sendDeviceStatus(DeviceSta_Strcture * pdevsta);
void processDeviceStatus(DeviceSta_Strcture * pdevsta);

#endif

