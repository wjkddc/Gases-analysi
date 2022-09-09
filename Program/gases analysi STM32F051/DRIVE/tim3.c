
#include "tim3.h"
#include "gpio.h"
#include "delay.h"


uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0;

////四路PWM
//PWM1  	GPIO_Pin_6 GPIOA
//PWM2  	GPIO_Pin_7 GPIOA
//PWM3  	GPIO_Pin_0 GPIOB
//PWM4  	GPIO_Pin_1 GPIOB	
////==================================

void TIM3_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	 
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,ENABLE);
                   		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);//GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2); TIM2
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);//GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);	TIM2
	
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
	
	/* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
//TimerPeriod = (uint16_t)((SystemCoreClock / 17570 ) - 1);
//TimerPeriod = (uint16_t)((SystemCoreClock / 10000 ) - 1);
//Channel1Pulse = (uint16_t)(((uint32_t) 5 * (TimerPeriod - 1)) / 10);
	TIM_TimeBaseStructure.TIM_Prescaler = 32; 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//初始化定时器

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
 // TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//TIM_OCPolarity_High
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
  TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
//	TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure); 
	/* 便能预装寄存器*/
	TIM_OC1PreloadConfig(TIM3 , TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3 , TIM_OCPreload_Enable);
//	TIM_OC3PreloadConfig(TIM3 , TIM_OCPreload_Enable);
//	TIM_OC4PreloadConfig(TIM3 , TIM_OCPreload_Enable);
	//中断允许寄存器
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}


void PWMset(TIM_TypeDef* TIMx,uint32_t Compare)
{
	TIM_SetCompare1(TIMx,Compare*10);
	TIM_SetCompare2(TIMx,Compare*10);
//	TIM_SetCompare3(TIMx,Compare*10);
//	TIM_SetCompare4(TIMx,Compare*10);
}

											
















