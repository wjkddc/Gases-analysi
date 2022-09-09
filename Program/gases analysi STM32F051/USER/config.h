#ifndef _CONFIG_H
#define _CONFIG_H

#include "stm32f0xx.h"
#include "i2c_eeprom.h"//AT24X
#include "FM31256.h" //����



#define EE_Basic_Addarss 		0
/*********************************************************************************
---------------��������1��----------------------
FlashоƬѡ��
EE_Driver 	==	1
	1.AT24C512	��������(��һҳ)
	2.MCUоƬ�ڲ�RTCʱ��
EE_Driver 	==	2
	1.FM31256		��������(��һҳ)
	2.FM31256ʱ��
*********************************************************************************/	
#define EE_Driver				1 	//1:AT24CX	2:����FM32256


/********************************************************************************
---------------��������2��----------------------
IAP��ʱ����ѡ��
IAP_Driver 	==	1
	1.AT24C512 	IAP����(��һҳ����)
IAP_Driver 	==	2
	1.FM31256		IAP����(��һҳ����)
********************************************************************************/	
//#define IAP_Driver			1 	//
//Ĭ��------>> AT24C512(��һҳ����)










#if		EE_Driver		== 	1
	#define EE_Page1		0xA0  //AT24Cx ��ַ��
#elif EE_Driver		== 	2
	#define EE_Page1		0xA0  //����FM32256 ��ַ��
#endif







extern void EE_SAVE(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite);
extern uint8_t EE_GET(uint8_t Page,uint16_t ReadAddr);

#endif

