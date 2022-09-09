#ifndef __ADC_H
#define	__ADC_H

#include "stm32f0xx.h"


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����flash

extern  uint16_t  RegularConvData_Tab[];//������������
extern  uint16_t Gases1[];
extern  uint16_t Gases2[];
extern  uint16_t	Aver_Gases1;//Gases1ƽ��ֵ
extern  uint16_t	Aver_Gases2;//Gases2ƽ��ֵ
extern uint8_t ADC_finish;//ADC������־

	
	
extern void ADCInit_gases(void);
extern void ADC_DMA_Config(void);
	

extern uint16_t ADC_gases_Analysi(uint16_t *p,uint8_t Leght);	
extern	void HMIdisplay_Gases(void);
	
	
#endif /* __ADC_H */
