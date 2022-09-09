#include "IR433.h"
#include "uart1.h"
#include "gpio.h"
#include "crc16.h"
#include "delay.h"

//433设置
uint8_t RF433_Buffer[7];
uint8_t RF433_Buffer_Write[15];
uint8_t Uart_Freq,Uart_Kbps;

const uint8_t RF433_Freq[20][3]={//RF433频率参数设置
/*0*/		  {0x06,0x9B,0x68},//433.0
/*1*/		  {0x06,0x9B,0xCC},//433.1
/*2*/		  {0x06,0x9C,0x30},//433.2
/*3*/		  {0x06,0x9C,0x94},//433.3
/*4*/		  {0x06,0x9C,0xF8},//433.4
/*5*/		  {0x06,0x9D,0x5C},//433.5
/*6*/		  {0x06,0x9D,0xC0},//433.6
/*7*/		  {0x06,0x9D,0x24},//433.7
/*8*/		  {0x06,0x9E,0x88},//433.8
/*9*/		  {0x06,0x9e,0xec},//433.9
/*10*/		{0x07,0x2b,0xf0},//470.0
/*11*/		{0x07,0x2c,0x54},//470.1
/*12*/		{0x07,0x2c,0xb8},//470.2
/*13*/		{0x07,0x2d,0x1c},//470.3
/*14*/		{0x07,0x2d,0x80},//470.4
/*15*/		{0x07,0x2d,0xe4},//470.5
/*16*/		{0x07,0x2e,0x48},//470.6
/*17*/		{0x07,0x2e,0xac},//470.7
/*18*/		{0x07,0x2f,0x10},//470.8
/*19*/		{0x07,0x2f,0x74},//470.9
};
const uint8_t RF433_Kbps[6]={//空中速率
						/*0*/			0x00,//0.81k
						/*1*/			0x01,//1.46k
						/*2*/			0x02,//2.6k
						/*3*/			0x03,//4.56k
						/*4*/			0x04,//9.11k
						/*5*/			0x05,//18.23k
};



/************************RF433********************************************************/
void RF433_Txd(void)//RF433读参数
{
	uint8_t i;
	RF433_Buffer[0] = 0xFF;
	RF433_Buffer[1] = 0x56;
	RF433_Buffer[2] = 0xAE;
	RF433_Buffer[3] = 0x35;
	RF433_Buffer[4] = 0xA9;
	RF433_Buffer[5] = 0x55;
	RF433_Buffer[6] = 0xF0;
	for(i=0;i<=6;i++){
		USART_SendData(USART1,RF433_Buffer[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
}
void RF433_Sig_Txd(void)//RF433读信号强度
{
	uint8_t i;
	RF433_Buffer[0] = 0xFF;
	RF433_Buffer[1] = 0x56;
	RF433_Buffer[2] = 0xAE;
	RF433_Buffer[3] = 0x35;
	RF433_Buffer[4] = 0xA9;
	RF433_Buffer[5] = 0x55;
	RF433_Buffer[6] = 0x77;
	for(i=0;i<=6;i++){
		USART_SendData(USART1,RF433_Buffer[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
}
//RF433写
/***********************************************************
*************************功率*******************************
****0x07-20dbm****0x06-18dbm****0x05-16dbm****0x04-14dbm****
****0x03-12dbm****0x02-10dbm****0x01-8dbm*****0x00-6dbm*****
************************************************************
***********************波特率*******************************
****0x07-115200bps****0x06-57600bps****0x05-38400bps********
****0x04-19200bps*****0x03-9600bps*****0x02-4800bps*********
****0x01-2400bps******0x00-1200bps**************************
************************************************************
***********************睡眠时间*****************************
****00 05-50ms******01 00-100ms*****02 00-200ms*************
****03 00-400ms*****04 00-600ms*****05 00-1000ms************
****06 00-1500ms****07 00-2000ms****08 00-2500ms************
****09 00-3000ms****0a 00-4000ms****0b 00-5000ms************
***********************************************************/
//void RF433_Txd_Wtite(void)
//{
//	uint8_t w;
//	RF433_Buffer_Write[0]=0xFF;
//	RF433_Buffer_Write[1]=0x56;
//	RF433_Buffer_Write[2]=0xAE;
//	RF433_Buffer_Write[3]=0x35;
//	RF433_Buffer_Write[4]=0xA9;
//	RF433_Buffer_Write[5]=0x55;
//	RF433_Buffer_Write[6]=0x90;
//	RF433_Buffer_Write[11]=0x07;//功率20dbm
//	RF433_Buffer_Write[12]=0x03;//9600bps
//	RF433_Buffer_Write[13]=0x00;//睡眠时间
//	RF433_Buffer_Write[14]=0x05;//睡眠时间
//	for(w=0;w<=14;w++){
//		USART_SendData(USART1,RF433_Buffer_Write[w]);
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//	}
//}
//void RF433Return(void)
//{
//	uint8_t i;
//  uint16_t Lamp_CRC;	
//	LampData[0]=0xEE;
//	Lamp_CRC = CRC16(LampData,18);
//  LampData[18] = Lamp_CRC >> 8;
//  LampData[19] = Lamp_CRC;
//	LampData[20]=0xEF;
//	if(1-(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))){
//  		DIOA_H;		
//			for(i=0;i<UART1_BufData;i++){
//				USART_SendData(USART1,LampData[i]);
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//			}
//		}
//	 delay_ms(80);
//	 CLS_array(LampData,UART1_BufData);
//	 DIOA_L;
//}

/******************************************************************************/

































