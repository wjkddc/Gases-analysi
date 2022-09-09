#ifndef _IR433_H
#define _IR433_H

#include "stm32f0xx.h"


extern const uint8_t RF433_Freq[20][3];//RF433频率参数设置
extern const uint8_t RF433_Kbps[6];//空中速率

//433设置
extern uint8_t RF433_Buffer[];
extern uint8_t RF433_Buffer_Write[];
extern uint8_t Uart_Freq,Uart_Kbps;













extern void RF433_Txd(void);//RF433读参数
extern void RF433_Sig_Txd(void);//RF433读信号强度

extern void RF433_Txd_Wtite(void);
extern void RF433Return(void);










#endif

