#include "stm32f0xx.h"



extern RTC_TimeTypeDef RTC_TimeStruct;//ʵʱʱ��
extern RTC_DateTypeDef RTC_DateStruct;//ʵʱʱ��

extern char Data_tmp[];//���ݻ���



void HMISendstart(void);
void HMISends(char *buf1);		  //�ַ������ͺ���
void HMISendb(char k)		 ;        //�ֽڷ��ͺ���




void page_sele(void);// ҳ����
void HMISend_s(USART_TypeDef* USARTx,char *buf1); //�ַ������ͺ���
void HMISend_b(USART_TypeDef* USARTx,char k);	 //�ֽڷ��ͺ���
void HMI_sendform(USART_TypeDef* USARTx, char *Data);



extern void HMI_display(void);//���ݽ�����ʾ����


