#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f0xx.h"



//============20160722==============================
union DATA16{ //结构联合体
	struct 
	{
		uint8_t dataL;
		uint8_t dataH;
	}uni;
	uint16_t Data16;
};

struct LightAndTimeGrade{//时间段与亮度值
	uint8_t Hour1;
	uint8_t Min1;
	uint8_t Light1;
	uint8_t Hour2;
	uint8_t Min2;
	uint8_t Light2;
	uint8_t Hour3;
	uint8_t Min3;
	uint8_t Light3;
	uint8_t Hour4;
	uint8_t Min4;
	uint8_t Light4;
	uint8_t Hour5;
	uint8_t Min5;
	uint8_t Light5;
	uint8_t Hour6;
	uint8_t Min6;
	uint8_t Light6;
};

typedef enum {OFF=0,ON=!OFF}RelayAction; //枚举继电器动作

//系统指示灯
#define Red_PIN			 	GPIO_Pin_4 	//执行 LED1
#define Yellow_PIN		GPIO_Pin_3	//运行	LED2
#define Orange_PIN		GPIO_Pin_15	//故障	LED3
#define Red_PORT 			GPIOB
#define Yellow_PORT 	GPIOB
#define Orange_PORT 	GPIOA
#define Red_OFF  			GPIO_SetBits(Red_PORT, Red_PIN ) //置1
#define Red_ON  			GPIO_ResetBits(Red_PORT, Red_PIN )//置0
#define Yellow_OFF  	GPIO_SetBits(Yellow_PORT, Yellow_PIN )
#define Yellow_ON  		GPIO_ResetBits(Yellow_PORT, Yellow_PIN )
#define Orange_OFF  	GPIO_SetBits(Orange_PORT, Orange_PIN )
#define Orange_ON  		GPIO_ResetBits(Orange_PORT, Orange_PIN )

#define Red_Run 			GPIO_WriteBit(Red_PORT,Red_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Red_PORT,Red_PIN))));//闪烁
#define Yellow_Run 		GPIO_WriteBit(Yellow_PORT,Yellow_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Yellow_PORT,Yellow_PIN))));
#define Orange_Run 		GPIO_WriteBit(Orange_PORT,Orange_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Orange_PORT,Orange_PIN))));
//===================================
//二路继电器
//#define Relay_PIN  		GPIO_Pin_0 
//#define Relay_PORT  	GPIOA
//#define Relay2_PIN   	GPIO_Pin_1 
//#define Relay2_PORT 	GPIOA
//#define Relay3_PIN   	GPIO_Pin_4 
//#define Relay3_PORT 	GPIOA
//#define Relay4_PIN   	GPIO_Pin_5 
//#define Relay4_PORT 	GPIOA
//#define Relay_OFF 		GPIO_SetBits(Relay_PORT, Relay_PIN )
//#define Relay_ON 			GPIO_ResetBits(Relay_PORT, Relay_PIN)
//#define Relay2_OFF 		GPIO_SetBits(Relay2_PORT, Relay2_PIN )
//#define Relay2_ON 		GPIO_ResetBits(Relay2_PORT, Relay2_PIN)
//#define Relay3_OFF 		GPIO_SetBits(Relay3_PORT, Relay3_PIN )
//#define Relay3_ON 		GPIO_ResetBits(Relay3_PORT, Relay3_PIN)
//#define Relay4_OFF 		GPIO_SetBits(Relay4_PORT, Relay4_PIN )
//#define Relay4_ON 		GPIO_ResetBits(Relay4_PORT, Relay4_PIN)

//#define Relay_Run  		GPIO_WriteBit(Relay_PORT,Relay_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Relay_PORT,Relay_PIN))));
//#define Relay2_Run 		GPIO_WriteBit(Relay2_PORT,Relay2_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Relay2_PORT,Relay2_PIN))));
//#define Relay3_Run 		GPIO_WriteBit(Relay3_PORT,Relay3_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Relay3_PORT,Relay3_PIN))));
//#define Relay4_Run 		GPIO_WriteBit(Relay4_PORT,Relay4_PIN,(BitAction)((1-GPIO_ReadOutputDataBit(Relay4_PORT,Relay4_PIN))));
//===================================

//硬件看门狗
//#define WDI_PIN  		GPIO_Pin_0 
//#define WDI_PORT  	GPIOA
//#define WDI_H  			GPIO_SetBits(WDI_PORT, WDI_PIN )
//#define WDI_L  			GPIO_ResetBits(WDI_PORT, WDI_PIN)



extern void GPIO_Init_Io(void);
extern void Relay_Action(RelayAction action);//继电器动作1：闭合  0：断开


#endif
