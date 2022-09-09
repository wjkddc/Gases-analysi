/***********************************************
  1.ʵʱ�¶������ʪ��=========================================GPIOA GPIO_Pin_6 GPIO_Pin_7
	Ϊ��֤������������0.1�棬SHTxx �ļ���ʱ
	�䲻Ҫ����10%�����磬��Ӧ12bit ���Ȳ�����
	ÿ��������2 �β�������
	
  2.�ⲿͨ�Žӿ�===============================================UART1

  3.���崫����=================================================
  MQ-135�������� ���� ���� ��ϵ���������� �����к����壩====GPIOA GPIO_Pin_0
  MQ-2B ��ȼ���� ���� ������Ȼ�� ��ȼ������=================GPIOA GPIO_Pin_1

  4.PM2.5������================================================UART2

	2016-09-05
	--1:����Ԥ�����־λ��ͨ����־λ����(�����ļ�config.h)
		A��AT24C16���� + �ڲ�RTC
		B���ⲿ����FM31256���棬ʱ��

  --2.����FM31256 �������Ź�
	--3.��������FM31256 ����������ڴ�оƬ��
***********************************************/
#include "stm32f0xx.h"
#include "gpio.h"
#include "rtc.h"
#include "tim3.h"
#include "tim14.h"
#include "tim15.h"
#include "uart1.h"
#include "uart2.h"
#include "bh1750.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SetClock.h"
#include "tim2.h"
#include "crc16.h"
#include "i2c_eeprom.h"//AT24X
#include "FM31256.h" //����
#include "config.h"	//�������Ʋ���
#include "pvd.h"
#include "IR433.h"
#include "HMI.h"
#include "adc.h"
#include "sht10.h"
#include "PM2_5.h"
#include "rtc.h"

#define DecHex(val)  (((val%256)&0xFF)+(val/256))
/******************************/
uint32_t systime,oldsystime;//ϵͳʱ��1ms
uint32_t syssec;//ϵͳʱ����
uint8_t SHT_time;//��ʪ��ת��ʱ��


struct LightAndTimeGrade RadiO;//�㲥��ʱʱ�䡢����ֵ	
union DATA16 LampCRC_Err;//ģ��CRCУ����¼
uint8_t  CRC_H,CRC_L,CRC_ID;//CRCУ��
uint32_t RanData;//�������������
/**************************************************/
uint8_t HexDec(uint8_t Hex)
{
	uint8_t s[] = "Hex";
	uint8_t i;
	uint8_t val;
	uint8_t Length = 8;
	for(i=0;i<Length;i++)
	{
		val += (s[i]<<(8*(Length-1-i)));
	}
	return val;
}

/*******************************************************************/
void IWDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(2000);// =LSI/32  2S��ι��
	IWDG_ReloadCounter();
	IWDG_Enable();
}
/****************************************/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
/*******************************************************************************************************/
	
void debug(void)//���� 
{
	uint8_t i;	
//===========================ϵͳʱ�� ����====================================================	
#if		EE_Driver		== 	1	//(AT24Cx)		
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//��ȡϵͳʱ������
	setTime.hour 	= RTC_TimeStruct.RTC_Hours;
	setTime.min		=	RTC_TimeStruct.RTC_Minutes;
	setTime.sec		=	RTC_TimeStruct.RTC_Seconds;
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//��ȡϵͳʱ������
	setTime.year	=		RTC_DateStruct.RTC_Year;
	setTime.month=		RTC_DateStruct.RTC_Month;
	setTime.day	=		RTC_DateStruct.RTC_Date;
	setTime.week=	RTC_DateStruct.RTC_WeekDay;
	printf("\r\n\r\n_____________________%2d/%2d/%2d(%d) %2d:%2d:%2d\r\n",setTime.year,setTime.month,setTime.day,setTime.week,setTime.hour,setTime.min,setTime.sec);	
	printf("_____________________%2d--%2d\r\n",LampCRC_Err.uni.dataH,LampCRC_Err.uni.dataL);
#elif EE_Driver		== 	2	//(����FM32256)
	Get_Realtime(FM_Datatemp);
	printf("%2d:%2d:%2d-(%2d)  %2d/%2d/%2d\r\n",FM_Datatemp[2],FM_Datatemp[1],FM_Datatemp[0],FM_Datatemp[3],FM_Datatemp[6],FM_Datatemp[5],FM_Datatemp[4]);
#else
  //Ĭ�ϰ汾	
#endif	
	//==========================ʵʱ�¶������ʪ��==============================================	
	delay_ms(5);
	printf("_____________________Temperature:%2.1fC  Humidity:%2.1f%%\r\n",TempResult,HumResult);
	
	//==========================���崫����=================================================
	//MQ-135�������� ���� ���� ��ϵ���������� �����к����壩
	//MQ-2B ��ȼ���� ���� ������Ȼ�� ��ȼ������
	for(i=0;i<10;i++)//-------------------------------��ӡ����
	{
		printf("  %03d",Gases1[i]);
		if(i==9)  printf("\r\n");
	}
	printf("Average_Gases1=%03d\r\n",Aver_Gases1);//��ӡƽ��ֵ
	for(i=0;i<10;i++)//-------------------------------��ӡ����
	{
		printf("  %03d",Gases2[i]);
		if(i==9)  printf("\r\n");
	}
	printf("Average_Gases2=%03d\r\n",Aver_Gases2);//��ӡƽ��ֵ
	//==========================PM2.5������================================================
	//��ӡ���
	for(i=0;i<32;i++)
	{
		printf(" %2x",Rx2Buffer[i]);
		if(i==31) printf("\r\n");
	}	
	printf("datacrc��%2x %2x\r\n",datacrc>>8,datacrc&0XFF);
	printf("PM1_0��%3d  PM2_5��%3d  PM10��%3d\r\n",PM1_0,PM2_5,PM10);	
}

int main(void) 
{	
	SystemInit();	
	IWDG_Init();//���Ź���ʼ��	
	GPIO_Init_Io(); //GPIO��ʼ��	
	SHT10_gpio();//��ʪ��
	delay_init();
	USART1_Config();//USART1��ʼ������LCD ͨѶ�ӿ�
	USART2_Config();//USART2��ʼ���ⲿPM2.5ͨѶ�ӿ� 
	IIC_Init(); //I2C��ʼ��	
#if		EE_Driver		== 	1	//(AT24Cx)
	My_RTC_Init();	//��ʼ��RTC----------------	
#elif EE_Driver		== 	2	//(����FM32256)
	Fm31256_init();	//FM31256оƬ---------------
#endif
	TIM2_Config(1000-1,48-1);   //1KHZ����
	ADCInit_gases();//ADC����
	ADC_DMA_Config();
	TIM15_Config(); //TIM15��ʼ��		
//	TIM3_Config();  //TIM3��ʼ��
//	TIM_Cmd(TIM3, ENABLE);
//	TIM14_Config(); //TIM14��ʼ��		
	
	while(1)
	{
		IWDG_Feed();
		//��ʪ�ȼ��-------------------------------
		if((SHT_time>=2)) //����ת��0.2974S 
		{
			Red_ON;
			SHT_time=0;//���¼�ʱ	
			SHT10_Datachange();//��ʪ��ת��
			//systime=0;//ϵͳʱ�����¼�ʱ
		}
		if(systime !=oldsystime)//1ms
		{
			oldsystime=systime;	
			if(systime>= 0x01F4) //500ms
			{
				systime= systime-0x01F4;
				Yellow_Run;	
				SHT_time++;//��ʪ��ת��ʱ����
				//PM2.5���-----------------------------------------------
//				USART_Cmd(USART2,ENABLE);	//ʹ��USART2	
//				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
				PM2_5_analysi(Rx2Buffer); //ռ��150ms
//				USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//�ر��ж�
//				USART_Cmd(USART2,DISABLE);	//�ر�ʹ��USART2
				
				HMI_display();//��ʾ
				Red_OFF;
				//debug();//���� ռ��450ms
			}
			if(systime%0x0064==0) //100
			{
				page_sele();// ҳ����
				Orange_OFF;
			}
			if(systime%0x0096==0) //150
			{
				//�����������-------------------------------------------------
				if(ADC_finish==1)//ռ��150ms
				{
					Aver_Gases1= ADC_gases_Analysi(Gases1,10);
					//printf("Average_Gases1=%03d\r\n",Aver_Gases1);//��ӡƽ��ֵ
					Aver_Gases2= ADC_gases_Analysi(Gases2,10);
					//printf("Average_Gases2=%03d\r\n",Aver_Gases2);//��ӡƽ��ֵ
					ADC_finish=0;
				}
				//WatchDog_Clear();//FM31256оƬ----ι�� 
			}
		}	
  }//while(1)
}//main

