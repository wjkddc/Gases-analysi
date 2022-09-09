# Gases_analysis 特性
### 基础特性 ###
	1.温湿度检测 
		//SHT10
	2.可燃气体检测 
		//MQ-135空气质量 氨气 硫化物 苯系蒸气（烟雾 其它有害气体）
		//MQ-2B 可燃气体 丙烷 烟雾（天然气 可燃蒸气） 
	3.HIMI串口显示屏
		//TJC4024T032_011
### 基于STM32F051C8T6 设计而来###


	//SHT10
	#define SDA_H()         	GPIO_SetBits(GPIOA,GPIO_Pin_7)
	#define SDA_L()         	GPIO_ResetBits(GPIOA,GPIO_Pin_7)
	#define SCK_H()         	GPIO_SetBits(GPIOA,GPIO_Pin_6)
	#define SCK_L()         	GPIO_ResetBits(GPIOA,GPIO_Pin_6)
	//PM2.5通讯接口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;	//USART2 Tx(PA.2)	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//USART2 Rx(PA.3)	
	//LCD 通讯接口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;	//USART1 Tx(PA.9)	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//USART1 Rx(PA.10)
	//IIC
	#define  IIC_SCL_Hight 		GPIO_SetBits(GPIOB,GPIO_Pin_6)
	#define  IIC_SCL_Low   		GPIO_ResetBits(GPIOB,GPIO_Pin_6)
	#define  IIC_SDA_Hight 		GPIO_SetBits(GPIOB,GPIO_Pin_7)
	#define  IIC_SDA_Low   		GPIO_ResetBits(GPIOB, GPIO_Pin_7)


### 相关图片###
![](https://github.com/wjkddc/Gases_analysi/blob/main/%E7%95%8C%E9%9D%A2UI/bj_9.jpg)
![](https://github.com/wjkddc/Gases_analysi/blob/main/%E7%95%8C%E9%9D%A2UI/bj_10.png)
![](http://www.thinksoc.cn/wp-content/uploads/2022/09/bj.jpg)
![](http://www.thinksoc.cn/wp-content/uploads/2022/09/bj_9.jpg)



### Adobe AI设计稿 ###
    可根据需求二次开发UI界面
