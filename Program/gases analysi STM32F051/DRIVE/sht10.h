#ifndef _SHT10_H
#define _SHT10_H

#include "stm32f0xx.h"


extern float TempResult;  //�¶�ת�����
extern float HumResult; //ʪ��ת�����






extern void SHT10_gpio(void);
extern void SHT10_Datachange(void);//��ʪ��ת��
extern void HMIdisplay_TempHumResult(void);//��ʾ��ʪ��

#endif


