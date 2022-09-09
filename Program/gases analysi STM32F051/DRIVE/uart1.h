#ifndef __UART1_H_
#define __UART1_H_

#include "stm32f0xx.h"
#include <stdio.h>


/* --------------------------------------------*/
/* Size of Trasmission buffer */
#define TX1BUFFERSIZE                      21
/* Size of Reception buffer */
#define RX1BUFFERSIZE                      TX1BUFFERSIZE

extern uint8_t Uart1_Rx_Sta; //��ȷ���ݿ�ʼ���
extern uint16_t Tx1Buffer[];
extern uint16_t Rx1Buffer[];






void USART1_Config(void);
void USART1_IRQHandler(void); //����1�жϷ������

extern void UART1_send_byte(uint8_t byte);//����1�ֽ�����
extern void UART1_Send(uint8_t *Buffer, uint32_t Length);//��������
extern void USART1_SendTxBuf(uint16_t transmitsize,uint16_t *Data);
extern void CLS_array(uint16_t *p,uint8_t lenght); //�������
/* ----------------------------------------------------------------*/
void Uart1_TRx(uint8_t ms);


#endif
