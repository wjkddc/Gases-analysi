#ifndef __PVD_H
#define __PVD_H

#include "stm32f0xx.h"

void PVD_Configuration(void);
void EXTI_Configuration(void);
static void NVIC_Configuration(void);
void PVD_Init(void);
void  Test_Bright(void);

#endif
