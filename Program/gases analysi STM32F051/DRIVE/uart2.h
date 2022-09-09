#ifndef __UART2_H_
#define __UART2_H_

#include "stm32f0xx.h"


/* --------------------------------------------*/
/* Size of Trasmission buffer */
#define TX2BUFFERSIZE                      32
/* Size of Reception buffer */
#define RX2BUFFERSIZE                      TX2BUFFERSIZE
extern uint16_t Tx2Buffer[];
extern uint16_t Rx2Buffer[];
extern uint8_t Uart2_sync;//接收不同步标志位




void USART2_Config(void);
void USART2_IRQHandler(void);
void UART2_send_byte(uint8_t byte);
void UART2_Send(uint8_t *Buffer, uint32_t Length);




#endif
