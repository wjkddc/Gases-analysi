#include "tim15.h"
#include "gpio.h"
#include "delay.h"
#include "uart1.h"

extern void Uart1_TRx(uint8_t ms);

static void TIM15_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;		
	NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn ;//TIM3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

void TIM15_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);	//TIM14外设时钟使能
//	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 6000000) - 1;	
	TIM_DeInit(TIM15);
	//初始化时间时基单元
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_Prescaler =0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM15,&TIM_TimeBaseStructure);	

	TIM_PrescalerConfig(TIM15, 48, TIM_PSCReloadMode_Immediate);	
	TIM_ITConfig(TIM15, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM15, ENABLE);		
	TIM15_NVIC_Config();
}

void TIM15_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM15,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM15,TIM_IT_Update);	
//		TIM_ITConfig(TIM15, TIM_IT_Update,DISABLE );
//    TIM_Cmd(TIM15, DISABLE);
		
//		Uart1_TRx(1);
	}
}
