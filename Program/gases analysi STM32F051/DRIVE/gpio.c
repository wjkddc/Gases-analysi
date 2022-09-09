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
//	GPIO_SetBits(GPIOB, GPIO_Pin_12 ); //置1
//	GPIO_SetBits(GPIOB, GPIO_Pin_13 ); //置1
//	GPIO_ResetBits(GPIOB, GPIO_Pin_14 ); //置0
//	GPIO_ResetBits(GPIOB, GPIO_Pin_15 ); //置0
	
//	//硬件看门狗/WDI
//	GPIO_InitStruct.GPIO_Pin = WDI_PIN;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_Level_1;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(WDI_PORT, &GPIO_InitStruct);
	
	Yellow_OFF;
	Red_OFF;
	Orange_OFF;
//	Relay_Action(OFF);//继电器动作1：闭合  0：断开

}


void Relay_Action(RelayAction action)//继电器动作1：闭合  0：断开
{
	if(action==1)
	{
	//	Relay_ON;//继电器闭合
	//	Relay2_ON;//					报警开关作用
//		Relay3_ON;//继电器
	//	Relay4_ON;//继电器
	}
	if(action==0)
	{
	//	Relay_OFF;//继电器断开
	//	Relay2_OFF;//					报警开关作用
//		Relay3_OFF;//继电器关
	//	Relay4_OFF;//继电器关
	}	

}


