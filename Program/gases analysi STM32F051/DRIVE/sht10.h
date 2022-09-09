#ifndef _SHT10_H
#define _SHT10_H

#include "stm32f0xx.h"


extern float TempResult;  //温度转换结果
extern float HumResult; //湿度转换结果






extern void SHT10_gpio(void);
extern void SHT10_Datachange(void);//温湿度转换
extern void HMIdisplay_TempHumResult(void);//显示温湿度

#endif


