#ifndef __RTC_H
#define __RTC_H	

#include "stm32f0xx.h" 


struct Datatime{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t week;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
};

extern struct Datatime setTime;

void My_RTC_Init(void);						//RTC初始化
ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);			//RTC时间设置
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week); 		//RTC日期设置


#endif

















