/************************************************************
 FM31256是一种基于I2C总线、采用铁电体技术的多功能存储芯片。
 除了非易失存储器外，该器件还具有实时时钟、低电压复位、
 看门狗计数器、非易失性事件计数器、可锁定的串行数字标
 识等多种功能。
*************************************************************/
#ifndef _FM31256_H_
#define _FM31256_H_

#include "i2c_eeprom.h"


extern uint8_t Updata_FG;////贮存时间更新标志位
extern uint8_t FM_Datatemp[];//日期时间数组

#define IIC_WRITE			0xa0 //芯片引脚可选硬件地址均为0
#define IIC_READ			0xa1  //芯片引脚可选硬件地址均为0
#define REG_READ			0Xd1  //register read //芯片引脚可选硬件地址均为0
#define REG_WRITE			0Xd0 //register write//芯片引脚可选硬件地址均为0

#define WRITEPROTECT	0X0B
#define CONTROL				0X01	
#define FLAGS					0x00	//标志/控制位
#define WDTFLAG				0x09
#define WDTTIME				0x0a


extern uint8_t hex_to_bcd(uint8_t condata);
extern uint8_t bcd_to_hex(uint8_t condata);
extern void WriteFRAM(uint16_t Address,uint8_t Wdata);
extern uint8_t ReadFRAM(uint16_t Address);
extern uint8_t FM31256_ReadOneByte_Page(uint8_t Page,uint16_t ReadAddr);
extern void FM31256_WriteOneByte_Page(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite);
extern void Set_Realtime(uint8_t *realtime);//uint8_t *realtime时间格式：低字节--高字节  秒，分，时，星期，日，月，年。
extern void Uart1_Set_Realtime(void);//通过串口授时 参考《集中器终端通信协议V0.4》
extern void Get_Realtime(uint8_t *realtime);//uint8_t *realtime
extern void WatchDog_Enable(uint8_t timer);
extern void WatchDog_Clear(void);    //喂狗
extern void WatchDog_Disable(void);
extern void Fm31256_init(void);	

extern void debug_time(void);//测试获取时间(void);//获取日期时间







#endif


