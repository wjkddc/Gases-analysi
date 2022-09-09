#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f0xx.h"

void TIM3_Config(void);
void PWMset(TIM_TypeDef* TIMx,uint32_t Compare);



#endif
