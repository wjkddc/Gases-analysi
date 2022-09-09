#include "stdio.h"
#include "adc.h"
#include "gpio.h"
#include "HMI.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DMA_BUFFER_SIZE     4 
 uint16_t RegularConvData_Tab[DMA_BUFFER_SIZE]={0,0};//������������
 uint16_t Gases1[10];
 uint16_t Gases2[10];
 uint16_t	Aver_Gases1;//Gases1ƽ��ֵ
 uint16_t	Aver_Gases2;//Gases2ƽ��ֵ
uint8_t ADC_finish=0;//ADC������־
uint8_t AD_index=0; //ADC ת������

//=======================��������ʾ����==========================================
////////////���崫����//////////////////////////////////
//MQ-135�������� ���� ���� ��ϵ���������� �����к����壩
//MQ-2B ��ȼ���� ���� ������Ȼ�� ��ȼ������
//printf("ADC0=%02d   ADC1=%02d\r\n",RegularConvData_Tab[0],RegularConvData_Tab[1]);
uint16_t HMI_gases1;//����1
uint16_t HMI_gases2;//����2
//===============================================================================


void ADCInit_gases(void)
{
  ADC_InitTypeDef     ADC_InitStruct;
//	GPIO_InitTypeDef    GPIO_InitStruct;
//	
//	/* Enable  GPIOA clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
// /* Configure PA.01  as analog input */
//  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStruct.GPIO_OType =GPIO_OType_OD;
//	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_UP;// GPIO_PuPd_DOWN;//GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOA, &GPIO_InitStruct);				// PC1,����ʱ������������

	 /* ADC1 DeInit */  
  ADC_DeInit(ADC1);
	/* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStruct);
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;//12 bits 
  ADC_InitStruct.ADC_ContinuousConvMode =ENABLE ; //����ת��ģʽ  DISABLE
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//������ʽ
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//�����Ҷ���
  ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Upward;//����ɨ��    ����ɨ��  ADC_ScanDirection_Backward
  ADC_Init(ADC1, &ADC_InitStruct); 
 
//   /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
//   ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_55_5Cycles);  
//   ADC_TempSensorCmd(ENABLE);
  ADC_OverrunModeCmd(ADC1, ENABLE); //ʹ�����ݸ���ģʽ
  /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_0|ADC_Channel_1, ADC_SampleTime_13_5Cycles); //���ò���ͨ����ʱ��  
//	ADC_VrefintCmd(ENABLE);
	ADC_GetCalibrationFactor(ADC1); /* ADC Calibration У׼*/
  ADC_Cmd(ADC1, ENABLE);  	/* Enable ADC1 */
	/* Wait the ADCEN falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
	ADC_DMACmd(ADC1, ENABLE);	/* ʹ��DMA*/	
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); //DMA����ģʽΪѭ��ģʽ
	/* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
//  ADC_VrefintCmd(ENABLE);
	
	ADC_DMA_Config();
}

void ADC_DMA_Config(void)
{
  DMA_InitTypeDef     DMA_InitStruct; 
	NVIC_InitTypeDef 		nvic_init_structure; 
	/* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	nvic_init_structure.NVIC_IRQChannel = DMA1_Channel1_IRQn;       //??DMA1????  
	nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //????  
	nvic_init_structure.NVIC_IRQChannelPriority = 3;                //?????0  
	NVIC_Init(&nvic_init_structure);  
	
	
	 /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1); //��λ
	DMA_InitStruct.DMA_BufferSize =DMA_BUFFER_SIZE;//DMA���������С
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA���� ������Ϊ����Դ 
	
  DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&RegularConvData_Tab[0];//��������������ʼ��ַ
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݴ�СΪHalfWord   DMA_MemoryDataSize_Byte
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//;//�ڴ��ַ����DMA_MemoryInc_Enable
	
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//ȡֵ�������ַ 
	DMA_InitStruct.DMA_PeripheralDataSize =DMA_MemoryDataSize_HalfWord ;//���ݴ�СΪHalfWord   DMA_MemoryDataSize_Byte
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ��������  DMA_PeripheralInc_Enable
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular ;//DMAѭ��ģʽ������ɺ����¿�ʼ����
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;//DMA���ȼ�����Ϊ��
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//�ڴ浽�ڴ����
  DMA_Init(DMA1_Channel1, &DMA_InitStruct);//
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);                  //ʹ��DMA�ж�
	DMA_ClearITPendingBit(DMA_IT_TC);                                //���DMA�ж�
  DMA_Cmd(DMA1_Channel1, ENABLE); /* DMA1 Channel1 enable */
	
}



uint16_t ADC_gases_Analysi(uint16_t *p,uint8_t Leght)
{
	uint8_t i,j;
	uint16_t temp;
	uint16_t Sum=0;
	uint16_t average=0;
	for(i=0;i<Leght-1;i++)	//ð�ݷ�����
		for(j=i+1;j<Leght;j++)
		{
			if(p[j]<p[i])
			{
				temp=p[j];
				p[j]=p[i];
				p[i]=temp;
			}
		}
//	for(i=0;i<Leght;i++)//---------------------------------��ӡ����
//	{
//		printf("  %02d",p[i]);
//		if(i==(Leght-1))  printf("\r\n");
//	}
	for(i=2;i<=7;i++)//���ֵ ��Сֵ����ȥ��ȡƽ��
	{
		Sum += p[i];
	}
	average=Sum/6;
	return average;
}
							
void DMA1_Channel1_IRQHandler()  
{  
	if(DMA_GetITStatus(DMA_IT_TC))                    
	{  
		if(ADC_finish==0)
		{
			Gases1[AD_index]=RegularConvData_Tab[0];
			Gases2[AD_index]=RegularConvData_Tab[1];
			AD_index++;
		}
		if(AD_index>=10)
		{
			AD_index=0;
			ADC_finish=1;
			//DMA_Cmd(DMA1_Channel1, DISABLE); ����DMA�����ݴ�����ȥ

//			for(i=0;i<10;i++)//---------------------------------��ӡ����
//			{
//				if(i==0)  printf("Gases1");
//				printf("  %03d",Gases1[i]);
//				if(i==9)  printf("\r\n");
//			}
//			for(i=0;i<10;i++)//---------------------------------��ӡ����
//			{
//				if(i==0)  printf("Gases2");
//				printf("  %03d",Gases2[i]);
//				if(i==9)  printf("\r\n");
//			}
		}
	}  
	DMA_ClearITPendingBit(DMA_IT_TC); //�����־λ
}  

//MQ-135�������� ���� ���� ��ϵ���������� �����к����壩
//MQ-2B ��ȼ���� ���� ������Ȼ�� ��ȼ������				
void HMIdisplay_Gases(void)
{
	if((HMI_gases1!= Aver_Gases1)||(HMI_gases2 != Aver_Gases2))//�ٴ����ݲ����ʱ��ˢ����ʾ
	{
		HMI_gases1	= Aver_Gases1;
		HMI_gases2	= Aver_Gases2;
		sprintf(Data_tmp,"t0.txt=\"%5d\"",HMI_gases1);
		HMI_sendform(USART1,Data_tmp);
		sprintf(Data_tmp,"t1.txt=\"%5d\"",HMI_gases2);
		HMI_sendform(USART1,Data_tmp);
	}

}


