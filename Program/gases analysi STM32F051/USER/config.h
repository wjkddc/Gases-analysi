#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f0xx.h"
#include "i2c_eeprom.h"//AT24X
#include "FM31256.h" //铁电



#define EE_Basic_Addarss 		0
/*********************************************************************************
---------------参数设置1：----------------------
Flash芯片选择
EE_Driver 	==	1
	1.AT24C512	数据贮存(第一页)
	2.MCU芯片内部RTC时钟
EE_Driver 	==	2
	1.FM31256		数据贮存(第一页)
	2.FM31256时钟
*********************************************************************************/	
#define EE_Driver				1 	//1:AT24CX	2:铁电FM32256


/********************************************************************************
---------------参数设置2：----------------------
IAP临时缓存选择
IAP_Driver 	==	1
	1.AT24C512 	IAP缓存(第一页除外)
IAP_Driver 	==	2
	1.FM31256		IAP缓存(第一页除外)
********************************************************************************/	
//#define IAP_Driver			1 	//
//默认------>> AT24C512(第一页除外)










#if		EE_Driver		== 	1
	#define EE_Page1		0xA0  //AT24Cx 地址线
#elif EE_Driver		== 	2
	#define EE_Page1		0xA0  //铁电FM32256 地址线
#endif







extern void EE_SAVE(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite);
extern uint8_t EE_GET(uint8_t Page,uint16_t ReadAddr);

#endif

