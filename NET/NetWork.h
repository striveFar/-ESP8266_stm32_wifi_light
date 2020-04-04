/******************************************************************
 * 文件：NetWork.c
 * 功能：声明TCP、UDP通信相关函数
 * 日期：2018-04-06
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "stm32f10x.h"
#include "UART/uart.h"

/*连接AP宏定义*/
#define SSID "FAST_1D44" //WIFI或热点名
#define PWD  "17616152690" //WIFI或热点密码

// #define SSID "XXXXXXXXX" //WIFI或手机热点名
// #define PWD  "XXXXXXXXX" //WIFI或手机热点密码

/*连接服务器宏定义*/
#define TCP "TCP"
#define UDP "UDP"
#define IP  "192.168.xx.xx"
#define PORT 22958

/*发送接收缓冲区长度宏定义*/
#define TXBUFFER_LEN 50
#define RXBUFFER_LEN 50

u8 findStr(u8* dest,u8* src,u16 retry_cn);

u8 checkESP8266(void);
u8 initESP8266(void);
void restoreESP8266(void);

u8 connectAP(u8* ssid,u8* pwd);

u8 connectServer(u8* mode,u8* ip,u16 port);
void sendBuffertoServer(u8* buffer);
void processServerBuffer(void);
u8 disconnectServer(void);
