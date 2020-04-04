/******************************************************************
 * 文件：uart.h
 * 功能：实现uart相关函数
 * 日期：2018-02-27
 * 作者：
 * 版本：
 *       Ver.1.0 | 最初版本
 *       Ver.1.1 | 添加UART2初始化函数
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "UART/uart.h"
#include <stdio.h>
#include "NetWork.h"
#include "TLink.h"


extern u8 RXBuffer[RXBUFFER_LEN];
extern DeviceSta_Strcture device;

/**
 * 功能：初始化UART
 * 参数：None
 * 返回值：None
 */
void initUART(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);  //使能USART1时钟
    /*********************GPIO Config***************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                      //发送管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                //复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                     //接收管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /********************UART Config*****************************/ 
    USART_InitStructure.USART_BaudRate = 9600;                                     //设置波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8bits数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1bit停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不使用硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                //发送接收均使能
    USART_Init(USART1, &USART_InitStructure);                                      //设置生效
	USART_Cmd(USART1, ENABLE);                                                     //使能串口外设
}

/**
 * 功能：初始化UART2
 * 参数：None
 * 返回值：None
 */
void initUART2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*配置USART2和GPIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/*GPIO配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* UART2配置 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 
	USART_Cmd(USART2, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);	
}
/**
 * 功能：指定某个UART发送一个字节
 * 参数：USARTx：使用的目标串口x为1-3
 *       byte：待发送字节
 * 返回值：None
 */
void sendByte(USART_TypeDef *USARTx, u16 byte)
{
	USART_ClearFlag(USARTx, USART_FLAG_TC);             //软件清除发送完成标志位
    USART_SendData(USARTx, byte);                       //发送一个字节
    while (!USART_GetFlagStatus(USARTx, USART_FLAG_TC));//等待发送完成
    USART_ClearFlag(USARTx, USART_FLAG_TC);             //软件清除发送完成标志位
}

/**
 * 功能：指定某个串口发送字符串
 * 参数：USARTx：使用的目标串口x为1-3
 *       str：字符串指针
 * 返回值：None
 */
void sendString(USART_TypeDef *USARTx, char *str)
{
    while (*str)
    {
        sendByte(USARTx,*str++);
    }
}

/****************************中断服务函数****************************************/
/**
 * 功能：串口1中断服务函数
 * 参数：None
 * 返回值：None
 */
void USART1_IRQHandler(void)
{
  
}

/**
 * 功能：串口2中断服务函数
 * 参数：None
 * 返回值：None
 */
void USART2_IRQHandler(void)
{
    static u8 i = 0;

    if(USART_GetITStatus(USART2, USART_IT_RXNE)) //判断接收数据寄存器是否有数据
    {
        RXBuffer[i++] = USART_ReceiveData(USART2);
        if(i==RXBUFFER_LEN)                     //超出接收缓冲范围,可能的情形是ESP8266复位,为防止溢出必须设置索引                  
        {
            i = RXBUFFER_LEN-1;
        }
    }

    if(USART_GetITStatus(USART2, USART_IT_IDLE))
    {
        USART_ReceiveData(USART2);              //读一次UART可以清除空闲标志位
        i = 0;
        processDeviceStatus(&device);
    } 
}

/**
 * 功能：串口3中断服务函数
 * 参数：None
 * 返回值：None
 */
void USART3_IRQHandler(void)
{
}

/****************************重定向fputc函数****************************************/
/**
 * 功能：使用UART1实现printf函数功能
 * 参数：ch:待写入字符 我们无需关心
 *       f：文件指针 我们无需关心
 * 返回值：None
 */
int fputc(int ch, FILE *f)
{
    /* 将Printf内容发往串口 */
    USART_SendData(USART1, (unsigned char)ch);
    while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    USART_ClearFlag(USART1, USART_FLAG_TC);

    return (ch);
}
