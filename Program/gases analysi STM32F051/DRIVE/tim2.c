#include "tim2.h"
#include "gpio.h"
#include "delay.h"


extern uint32_t systime;//系统时间1ms





extern uint8_t  ZD_Flag;

static void TIM2_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;//TIM3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

void TIM2_Config(uint16_t arr,uint16_t psc)//  TIM2_Config(1000-1,48-1);   //1KHZ周期
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//TIM3外设时钟使能
//	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 6000000) - 1;	
	TIM_DeInit( TIM2);
	//初始化时间时基单元
	TIM_TimeBaseStructure.TIM_Period = arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//psc//设置用来作为TIMx时钟频率除数的预分频值  47+1不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);	

//	TIM_PrescalerConfig(TIM2, 32, TIM_PSCReloadMode_Immediate);	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);		
	TIM2_NVIC_Config();
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

		systime++;
		

	}
}


