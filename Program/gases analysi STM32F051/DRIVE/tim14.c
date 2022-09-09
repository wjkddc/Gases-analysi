#include "tim14.h"
#include "gpio.h"
#include "delay.h"
//#include "uart2.h"
//#include "uart1.h"

extern void UartRx(uint8_t ms);

static void TIM14_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;		
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn ;//TIM3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

void TIM14_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);	//TIM14外设时钟使能
//	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 6000000) - 1;	
	TIM_DeInit(TIM14);
	//初始化时间时基单元
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler =0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);	

	TIM_PrescalerConfig(TIM14, 48, TIM_PSCReloadMode_Immediate);	
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM14, ENABLE);		
	TIM14_NVIC_Config();
}

void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14,TIM_IT_Update);	
		//UartRx(1);
	}
}
