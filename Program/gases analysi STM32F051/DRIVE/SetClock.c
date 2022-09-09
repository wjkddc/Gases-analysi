#include "SetClock.h"


void SetupClock(void)
{
  ErrorStatus HSEStartUpStatus;
  
  RCC_DeInit();//将外设RCC寄存器重设为缺省值
	
  RCC_HSEConfig(RCC_HSE_ON);//设置外部高速晶振（HSE） 
  RCC_LSEConfig(RCC_LSE_ON);//设置外部低速晶振（LSE） 
  
  HSEStartUpStatus = RCC_WaitForHSEStartUp();//等待HSE起振
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		
  if(HSEStartUpStatus == SUCCESS)
  {
		/* Enable Prefetch Buffer and Flash 0 wait state */
    FLASH->ACR = FLASH_ACR_PRFTBE;
		
    /* HCLK = SYSCLK / 1 */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK = HCLK / 1 */
    RCC_PCLKConfig(RCC_HCLK_Div1); 

    /* PLLCLK = 8MHz * 1 = 8 MHz */
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_2);
		
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
	
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOC,ENABLE);//开启GPIOA的外设时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//UART1
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_TIM2,ENABLE);//UART2,TIM2
}

