#include "tim2.h"
#include "gpio.h"
#include "delay.h"


extern uint32_t systime;//ϵͳʱ��1ms





extern uint8_t  ZD_Flag;

static void TIM2_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;//TIM3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

void TIM2_Config(uint16_t arr,uint16_t psc)//  TIM2_Config(1000-1,48-1);   //1KHZ����
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);	//TIM3����ʱ��ʹ��
//	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 6000000) - 1;	
	TIM_DeInit( TIM2);
	//��ʼ��ʱ��ʱ����Ԫ
	TIM_TimeBaseStructure.TIM_Period = arr;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;//psc//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  47+1����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
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


