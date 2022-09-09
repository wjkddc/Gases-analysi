#include "stm32f0xx.h"



extern RTC_TimeTypeDef RTC_TimeStruct;//实时时间
extern RTC_DateTypeDef RTC_DateStruct;//实时时期

extern char Data_tmp[];//数据缓存



void HMISendstart(void);
void HMISends(char *buf1);		  //字符串发送函数
void HMISendb(char k)		 ;        //字节发送函数




void page_sele(void);// 页面标记
void HMISend_s(USART_TypeDef* USARTx,char *buf1); //字符串发送函数
void HMISend_b(USART_TypeDef* USARTx,char k);	 //字节发送函数
void HMI_sendform(USART_TypeDef* USARTx, char *Data);



extern void HMI_display(void);//数据解析显示界面


