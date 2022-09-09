#include "stm32f0xx.h"
#include <stdio.h>

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* дһ���ֽڵ�USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ����ͽ��� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��scanff�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}







////��main�ļ����ض���<fputc>���� ���£�
//// ��������
//int fputc(int ch, FILE *f)
//{
//	USART_SendData(USART1, (unsigned char) ch);// USART1 ���Ի��� USART2 ��
//	while (!(USART1->ISR & USART_FLAG_TXE));
//	return (ch);
//}
//// ��������
//int GetKey (void) {
//	while (!(USART1->ISR & USART_FLAG_RXNE));
//	return ((int)(USART1->TDR & 0x1FF));
//}
