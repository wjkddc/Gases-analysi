/************************************************************
 FM31256��һ�ֻ���I2C���ߡ����������弼���Ķ๦�ܴ洢оƬ��
 ���˷���ʧ�洢���⣬������������ʵʱʱ�ӡ��͵�ѹ��λ��
 ���Ź�������������ʧ���¼����������������Ĵ������ֱ�
 ʶ�ȶ��ֹ��ܡ�
*************************************************************/
#ifndef _FM31256_H_
#define _FM31256_H_

#include "i2c_eeprom.h"


extern uint8_t Updata_FG;////����ʱ����±�־λ
extern uint8_t FM_Datatemp[];//����ʱ������

#define IIC_WRITE			0xa0 //оƬ���ſ�ѡӲ����ַ��Ϊ0
#define IIC_READ			0xa1  //оƬ���ſ�ѡӲ����ַ��Ϊ0
#define REG_READ			0Xd1  //register read //оƬ���ſ�ѡӲ����ַ��Ϊ0
#define REG_WRITE			0Xd0 //register write//оƬ���ſ�ѡӲ����ַ��Ϊ0

#define WRITEPROTECT	0X0B
#define CONTROL				0X01	
#define FLAGS					0x00	//��־/����λ
#define WDTFLAG				0x09
#define WDTTIME				0x0a


extern uint8_t hex_to_bcd(uint8_t condata);
extern uint8_t bcd_to_hex(uint8_t condata);
extern void WriteFRAM(uint16_t Address,uint8_t Wdata);
extern uint8_t ReadFRAM(uint16_t Address);
extern uint8_t FM31256_ReadOneByte_Page(uint8_t Page,uint16_t ReadAddr);
extern void FM31256_WriteOneByte_Page(uint8_t Page,uint16_t WriteAddr,uint8_t DataToWrite);
extern void Set_Realtime(uint8_t *realtime);//uint8_t *realtimeʱ���ʽ�����ֽ�--���ֽ�  �룬�֣�ʱ�����ڣ��գ��£��ꡣ
extern void Uart1_Set_Realtime(void);//ͨ��������ʱ �ο����������ն�ͨ��Э��V0.4��
extern void Get_Realtime(uint8_t *realtime);//uint8_t *realtime
extern void WatchDog_Enable(uint8_t timer);
extern void WatchDog_Clear(void);    //ι��
extern void WatchDog_Disable(void);
extern void Fm31256_init(void);	

extern void debug_time(void);//���Ի�ȡʱ��(void);//��ȡ����ʱ��







#endif


