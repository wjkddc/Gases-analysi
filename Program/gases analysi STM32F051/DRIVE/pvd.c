#include "pvd.h"
#include "i2c_eeprom.h"
#include "gpio.h"

extern uint8_t Radar_Bright;

void PVD_Configuration(void)
{
	PWR_PVDLevelConfig(PWR_PVDLevel_6);
  PWR_PVDCmd(ENABLE);
}

void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
  
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
    EXTI_Init(&EXTI_InitStructure);    
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
 
    NVIC_InitStructure.NVIC_IRQChannel =PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
    NVIC_Init(&NVIC_InitStructure);
}

void PVD_Init(void)
{
	NVIC_Configuration();
	EXTI_Configuration();
  PVD_Configuration();
}

void PVD_IRQHandler(void)
{
    if(PWR_GetFlagStatus(PWR_FLAG_PVDO))     
    {
      AT24CXX_WriteOneByte_Page(0xa4,0,Radar_Bright);//写第三页，0;
    }
    EXTI_ClearITPendingBit(EXTI_Line16);        //清除中断
}

void  Test_Bright(void)
{
	uint8_t a;
  a = AT24CXX_ReadOneByte_Page(0xa4,0);
	if(a>=10){
		Relay_H;
		TIM_SetCompare2(TIM2,a*10);
		Radar_Bright = a;
	}
}
