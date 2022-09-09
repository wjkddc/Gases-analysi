/***********************************************
  1.实时温度与相对湿度=========================================GPIOA GPIO_Pin_6 GPIO_Pin_7
	为保证自身温升低于0.1℃，SHTxx 的激活时
	间不要超过10%（例如，对应12bit 精度测量，
	每秒最多进行2 次测量）。
	
  2.外部通信接口===============================================UART1

  3.气体传感器=================================================
  MQ-135空气质量 氨气 硫化物 苯系蒸气（烟雾 其它有害气体）====GPIOA GPIO_Pin_0
  MQ-2B 可燃气体 丙烷 烟雾（天然气 可燃蒸气）=================GPIOA GPIO_Pin_1

  4.PM2.5传感器================================================UART2

	2016-09-05
	--1:增加预编译标志位，通过标志位设置(配置文件config.h)
		A：AT24C16贮存 + 内部RTC
		B：外部铁电FM31256贮存，时间

  --2.铁电FM31256 开启看门狗
	--3.增加铁电FM31256 数据贮存放在此芯片中
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
#include "FM31256.h" //铁电
#include "config.h"	//工程配制参数
#include "pvd.h"
#include "IR433.h"
#include "HMI.h"
#include "adc.h"
#include "sht10.h"
#include "PM2_5.h"
#include "rtc.h"

#define DecHex(val)  (((val%256)&0xFF)+(val/256))
/******************************/
uint32_t systime,oldsystime;//系统时间1ms
uint32_t syssec;//系统时间秒
uint8_t SHT_time;//温湿度转换时间


struct LightAndTimeGrade RadiO;//广播定时时间、亮度值	
union DATA16 LampCRC_Err;//模块CRC校验错记录
uint8_t  CRC_H,CRC_L,CRC_ID;//CRC校验
uint32_t RanData;//产生随机数数据
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
  IWDG_SetReload(2000);// =LSI/32  2S内喂狗
	IWDG_ReloadCounter();
	IWDG_Enable();
}
/****************************************/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
/*******************************************************************************************************/
	
void debug(void)//调试 
{
	uint8_t i;	
//===========================系统时间 日期====================================================	
#if		EE_Driver		== 	1	//(AT24Cx)		
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);//获取系统时间日期
	setTime.hour 	= RTC_TimeStruct.RTC_Hours;
	setTime.min		=	RTC_TimeStruct.RTC_Minutes;
	setTime.sec		=	RTC_TimeStruct.RTC_Seconds;
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);//获取系统时间日期
	setTime.year	=		RTC_DateStruct.RTC_Year;
	setTime.month=		RTC_DateStruct.RTC_Month;
	setTime.day	=		RTC_DateStruct.RTC_Date;
	setTime.week=	RTC_DateStruct.RTC_WeekDay;
	printf("\r\n\r\n_____________________%2d/%2d/%2d(%d) %2d:%2d:%2d\r\n",setTime.year,setTime.month,setTime.day,setTime.week,setTime.hour,setTime.min,setTime.sec);	
	printf("_____________________%2d--%2d\r\n",LampCRC_Err.uni.dataH,LampCRC_Err.uni.dataL);
#elif EE_Driver		== 	2	//(铁电FM32256)
	Get_Realtime(FM_Datatemp);
	printf("%2d:%2d:%2d-(%2d)  %2d/%2d/%2d\r\n",FM_Datatemp[2],FM_Datatemp[1],FM_Datatemp[0],FM_Datatemp[3],FM_Datatemp[6],FM_Datatemp[5],FM_Datatemp[4]);
#else
  //默认版本	
#endif	
	//==========================实时温度与相对湿度==============================================	
	delay_ms(5);
	printf("_____________________Temperature:%2.1fC  Humidity:%2.1f%%\r\n",TempResult,HumResult);
	
	//==========================气体传感器=================================================
	//MQ-135空气质量 氨气 硫化物 苯系蒸气（烟雾 其它有害气体）
	//MQ-2B 可燃气体 丙烷 烟雾（天然气 可燃蒸气）
	for(i=0;i<10;i++)//-------------------------------打印数组
	{
		printf("  %03d",Gases1[i]);
		if(i==9)  printf("\r\n");
	}
	printf("Average_Gases1=%03d\r\n",Aver_Gases1);//打印平均值
	for(i=0;i<10;i++)//-------------------------------打印数组
	{
		printf("  %03d",Gases2[i]);
		if(i==9)  printf("\r\n");
	}
	printf("Average_Gases2=%03d\r\n",Aver_Gases2);//打印平均值
	//==========================PM2.5传感器================================================
	//打印输出
	for(i=0;i<32;i++)
	{
		printf(" %2x",Rx2Buffer[i]);
		if(i==31) printf("\r\n");
	}	
	printf("datacrc：%2x %2x\r\n",datacrc>>8,datacrc&0XFF);
	printf("PM1_0：%3d  PM2_5：%3d  PM10：%3d\r\n",PM1_0,PM2_5,PM10);	
}

int main(void) 
{	
	SystemInit();	
	IWDG_Init();//看门狗初始化	
	GPIO_Init_Io(); //GPIO初始化	
	SHT10_gpio();//温湿度
	delay_init();
	USART1_Config();//USART1初始化用于LCD 通讯接口
	USART2_Config();//USART2初始化外部PM2.5通讯接口 
	IIC_Init(); //I2C初始化	
#if		EE_Driver		== 	1	//(AT24Cx)
	My_RTC_Init();	//初始化RTC----------------	
#elif EE_Driver		== 	2	//(铁电FM32256)
	Fm31256_init();	//FM31256芯片---------------
#endif
	TIM2_Config(1000-1,48-1);   //1KHZ周期
	ADCInit_gases();//ADC采样
	ADC_DMA_Config();
	TIM15_Config(); //TIM15初始化		
//	TIM3_Config();  //TIM3初始化
//	TIM_Cmd(TIM3, ENABLE);
//	TIM14_Config(); //TIM14初始化		
	
	while(1)
	{
		IWDG_Feed();
		//温湿度检测-------------------------------
		if((SHT_time>=2)) //单次转换0.2974S 
		{
			Red_ON;
			SHT_time=0;//重新计时	
			SHT10_Datachange();//温湿度转换
			//systime=0;//系统时基重新计时
		}
		if(systime !=oldsystime)//1ms
		{
			oldsystime=systime;	
			if(systime>= 0x01F4) //500ms
			{
				systime= systime-0x01F4;
				Yellow_Run;	
				SHT_time++;//温湿度转换时间间隔
				//PM2.5检测-----------------------------------------------
//				USART_Cmd(USART2,ENABLE);	//使能USART2	
//				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
				PM2_5_analysi(Rx2Buffer); //占用150ms
//				USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关闭中断
//				USART_Cmd(USART2,DISABLE);	//关闭使能USART2
				
				HMI_display();//显示
				Red_OFF;
				//debug();//调试 占用450ms
			}
			if(systime%0x0064==0) //100
			{
				page_sele();// 页面标记
				Orange_OFF;
			}
			if(systime%0x0096==0) //150
			{
				//空气质量检测-------------------------------------------------
				if(ADC_finish==1)//占用150ms
				{
					Aver_Gases1= ADC_gases_Analysi(Gases1,10);
					//printf("Average_Gases1=%03d\r\n",Aver_Gases1);//打印平均值
					Aver_Gases2= ADC_gases_Analysi(Gases2,10);
					//printf("Average_Gases2=%03d\r\n",Aver_Gases2);//打印平均值
					ADC_finish=0;
				}
				//WatchDog_Clear();//FM31256芯片----喂狗 
			}
		}	
  }//while(1)
}//main

