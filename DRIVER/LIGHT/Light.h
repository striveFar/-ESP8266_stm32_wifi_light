/******************************************************************
 * 文件：Light.h
 * 功能：声明Light相关函数
 * 日期：2018-03-14
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#ifndef __LIGHT_H
#define __LIGHT_H
#include "stm32f10x.h"

void initLight(void);
void setLight(u8 brightness);
void ReserveValue(u8 bed, u8 living, u8 kitchen, u32 addr);

extern u8 bed_flag;
extern u8 living_flag;
extern u8 kitchen_flag;
extern u8 ctrl_bed;
extern u8 ctrl_living;
extern u8 ctrl_kitchen;

#endif
