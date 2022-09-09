#include "gpio.h"
#include "stm32f0xx.h"

void GPIO_Init_Io(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOF, ENABLE);

	//Yellow Red
  GPIO_InitStruct.GPIO_Pin = Yellow_PIN|Red_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Yellow_PORT, &GPIO_InitStruct);
	//Orange
  GPIO_InitStruct.GPIO_Pin = Orange_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(Orange_PORT, &GPIO_InitStruct);

  /* Configure ADC as analog input */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;//
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;// GPIO_PuPd_DOWN;// ; GPIO_PuPd_UP GPIO_PuPd_DOWN  GPIO_PuPd_UP
  GPIO_Init(GPIOA, &GPIO_InitStruct);				
	
//	/* Configure PB*/
//  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_AN
//	GPIO_InitStruct.GPIO_OType =GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
//	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_UP;// ;//GPIO_PuPd_NOPULL ;GPIO_PuPd_DOWN
//  GPIO_Init(GPIOB, &GPIO_InitStruct);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_AN
//	GPIO_InitStruct.GPIO_OType =GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_3;
//	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_DOWN;// ;//GPIO_PuPd_NOPULL ;GPIO_PuPd_DOWN
//  GPIO_Init(GPIOB, &GPIO_InitStruct);
//	GPIO_SetBits(GPIOB, GPIO_Pin_12 ); //��1
//	GPIO_SetBits(GPIOB, GPIO_Pin_13 ); //��1
//	GPIO_ResetBits(GPIOB, GPIO_Pin_14 ); //��0
//	GPIO_ResetBits(GPIOB, GPIO_Pin_15 ); //��0
	
//	//Ӳ�����Ź�/WDI
//	GPIO_InitStruct.GPIO_Pin = WDI_PIN;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_1;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(WDI_PORT, &GPIO_InitStruct);
	
	Yellow_OFF;
	Red_OFF;
	Orange_OFF;
//	Relay_Action(OFF);//�̵�������1���պ�  0���Ͽ�

}


void Relay_Action(RelayAction action)//�̵�������1���պ�  0���Ͽ�
{
	if(action==1)
	{
	//	Relay_ON;//�̵����պ�
	//	Relay2_ON;//					������������
//		Relay3_ON;//�̵���
	//	Relay4_ON;//�̵���
	}
	if(action==0)
	{
	//	Relay_OFF;//�̵����Ͽ�
	//	Relay2_OFF;//					������������
//		Relay3_OFF;//�̵�����
	//	Relay4_OFF;//�̵�����
	}	

}


