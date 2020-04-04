/******************************************************************
 * 文件：TIMx.c
 * 功能：实现TIM3相关函数
 * 日期：2018-03-08
 * 作者：zx
 * 版本：Ver.1.0 | 最初版本
 * 
 * Copyright (C) 2018 zx. All rights reserved.
*******************************************************************/
#include "TIMx/TIMx.h"
#include "LED/LED.h"


/**
 * 功能：初始化定时器
 * 参数：
 *      TIMx：指定待设置的定时器，TIM1-TIM4
 *      prescaler：设置预分频值 0-65535
 *      period：设置中断周期，即设置重装载寄存器的值 0-65535
 *      IT_Source:中断源，比如更新中断，四个通道的输入比较中断,取值查看：TIM_interrupt_sources 
 *      NewState：是否使能IT_Source参数指定的中断，ENABLE,DISENABLE
 * 返回值：None
 */
void initTIMx(TIM_TypeDef* TIMx,u16 prescaler,u16 period,u16 IT_Source,FunctionalState NewState)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    switch((u32)TIMx)
    {
        case (u32)TIM1: RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);break; //开启定时器1时钟
        case (u32)TIM2: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);break; //开启定时器2时钟
        case (u32)TIM3: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);break; //开启定时器3时钟
        case (u32)TIM4: RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);break; //开启定时器4时钟
        /*其他密度的单片机对case进行删减即可兼容，本程序针对中密度单片机*/
        default  :                                                     		break;
    }

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //向上计数模式
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //设置时钟分频因子，该分频不是预分频值，要区分
 	TIM_TimeBaseStructure.TIM_Period = period;                                      //设置定时器周期
	TIM_TimeBaseStructure.TIM_Prescaler =prescaler;                                 //设置预分频值
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);                                 //生效更改

    TIM_Cmd(TIMx, ENABLE);                                                          //开启计数器

    TIM_ITConfig(TIMx,IT_Source,NewState);                                        //使能定时器更新中断      
}

/**
 * 功能：初始化定时器4输入捕获
 * 参数：
 *      ICPolarity: 信号触发沿极性，TIM_ICPolarity_Rising，TIM_ICPolarity_Falling
 * 返回值：None
 */
void initTIM4IC3(u16 ICPolarity)
{
    TIM_ICInitTypeDef  TIM_ICInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //开启GPIOB时钟

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;                //配置输入捕获通道3为输入通道
  	TIM_ICInitStructure.TIM_ICPolarity = ICPolarity;	            //设置触发极性
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //将通道3连接到TI3
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	        //无预分频
  	TIM_ICInitStructure.TIM_ICFilter = 0x00;                        //无滤波
  	TIM_ICInit(TIM4, &TIM_ICInitStructure);                         //生效输入捕获设置
}

/**
 * 功能：初始化定时器3输出比较通道1来生成PWM
 * 参数：
 *      duty：设置PWM输出的占空比
 * 返回值：None
 */
void initTIM3OC1(u16 duty)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;						   //定义输出比较初始化结构体			 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //设置PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStructure.TIM_Pulse = duty;						  //设置捕获比较寄存器的值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //设置有效电平为高电平

	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 					  //生效初始化设置

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);			  //使能输出比较预装载，即更改完输入捕获寄存器的值要等到更新事件发生才生效,该行加不加无所谓
}

/**
 * 功能：初始化定时器4输出比较通道2来生成PWM
 * 参数：
 *      duty：设置PWM输出的占空比
 * 返回值：None
 */
void initTIM4OC2(u16 duty)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;						   //定义输出比较初始化结构体			 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //设置PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStructure.TIM_Pulse = duty;						  //设置捕获比较寄存器的值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //设置有效电平为高电平

	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 					  //生效初始化设置

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);			  //使能输出比较预装载，即更改完输入捕获寄存器的值要等到更新事件发生才生效,该行加不加无所谓
}

/**
 * 功能：初始化定时器4输出比较通道1来生成PWM
 * 参数：
 *      duty：设置PWM输出的占空比
 * 返回值：None
 */
void initTIM4OC1(u16 duty)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;						  //定义输出比较初始化结构体			 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //设置PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStructure.TIM_Pulse = duty;						  //设置捕获比较寄存器的值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //设置有效电平为高电平

	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 					  //生效初始化设置

	TIM_OC1PreloadConfig(TIM4, ENABLE);			  			      //使能输出比较预装载功能

	TIM_DMACmd(TIM4,TIM_DMA_CC1,ENABLE);					      //使能捕获比较DMA请求
}
/**
 * 功能：初始化定时器2输出比较通道1来生成PWM 对应冷光
 * 参数：
 *      duty：设置PWM输出的占空比
 * 返回值：None
 */
void initTIM2OC1(u16 duty)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;						   //定义输出比较初始化结构体			 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //设置PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStructure.TIM_Pulse = duty;						  //设置捕获比较寄存器的值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //设置有效电平为高电平

	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 					  //生效初始化设置

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);			  //使能输出比较预装载，即更改完输入捕获寄存器的值要等到更新事件发生才生效,该行加不加无所谓
}

/**
 * 功能：初始化定时器2输出比较通道2来生成PWM 对应冷光
 * 参数：
 *      duty：设置PWM输出的占空比
 * 返回值：None
 */
void initTIM2OC2(u16 duty)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;						   //定义输出比较初始化结构体			 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //设置PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStructure.TIM_Pulse = duty;						  //设置捕获比较寄存器的值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //设置有效电平为高电平

	TIM_OC2Init(TIM2, &TIM_OCInitStructure); 					  //生效初始化设置

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);			  //使能输出比较预装载，即更改完输入捕获寄存器的值要等到更新事件发生才生效,该行加不加无所谓
}
/**
 * 功能：设置定时器周期，即设置重装载寄存器的值
 * 参数：
 *      TIMx：指定待设置的定时器，TIM1-TIM4
 *      period：设置中断周期，即设置重装载寄存器的值 0-65535
 * 返回值：None
 */

void setPeriod(TIM_TypeDef* TIMx,u16 period)
{
    TIM_SetAutoreload(TIMx, period);  //设置重装载值
}

/**
 * 功能：获取自动重装载寄存器中的值
 * 参数：
 *      	TIMx：指定待设置的定时器，TIM1-TIM4
 * 返回值：	自动重装载寄存器中的值
 */
u16 getPeriod(TIM_TypeDef* TIMx)
{
	return TIMx->ARR;
}
/**
 * 功能：设置定时器预分频值
 * 参数：
 *      TIMx：指定待设置的定时器，TIM1-TIM4
 *      period：设置中断周期，即设置重装载寄存器的值 0-65535
 * 返回值：None
 */
void setPrescaler(TIM_TypeDef* TIMx,u16 prescaler)
{
    /*设置预分频值等于fCK_PSC/(PSC[15:0]+1)
     *fCK_PSC为内部输入时钟，默认72MHz，假如我们想要72000倍分频，即时钟周期1ms  
     *则我们传入的值应该是71999*/
    TIM_PrescalerConfig(TIMx, prescaler,TIM_PSCReloadMode_Update); //设置定时器周期
}

/****************************************定时器中断服务函数************************************************/


/**
 * 功能：定时器4中断服务函数
 * 参数：None
 * 返回值：None
 */
void TIM4_IRQHandler(void)
{

} 
