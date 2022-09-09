#ifndef _PM2_5_H
#define _PM2_5_H

#include "uart2.h"
#include "stm32f0xx.h"


extern uint16_t PM1_0;
extern uint16_t PM2_5;
extern uint16_t PM10;
extern uint16_t datacrc;//校验码





extern uint16_t HMI_PM2_5_IAQI;//PM2.5 IAQI空气质量分指数







extern void PM2_5_analysi(uint16_t *p);
uint16_t IAQI_analysi(uint16_t data); //空气质量分指数转换计算


extern void HMIdisplay_PM2_5(void);





#endif

