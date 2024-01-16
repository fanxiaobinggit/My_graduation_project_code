#include "bsp_adc.h"
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)  //����ADC���ڴ��ַ
__IO uint16_t ADC_ConvertedValue;
static void ADC1_GPIO_Config(void) //ADC�˿�����
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO��ʼ���ṹ�壻
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA��ʱ�ӣ�
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);//��ADC1��GPIOC��ʱ�ӣ�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//����PC0���ţ�
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//���ù���ģʽΪģ�����룻
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC��
}
static void ADC1_Mode_Config(void)  //����ADC1��ģʽ
{
	/********�������й�DMA���������*************/
	
	DMA_InitTypeDef DMA_InitStructure;//DMA��ʼ���ṹ�嶨��DMA��ʼ������
	
	ADC_InitTypeDef ADC_InitStructure;//ADC��ʼ���ṹ�嶨��ADC��ʼ������
	
	DMA_DeInit(DMA1_Channel1);//����DMA1ͨ��1
	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//�趨ADC�ĵ�ַ��
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//�ڴ��ַ���ɼ������ݴ������
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//��ΪԴ����ʾ�����Ǵ���������ģ�
	
	DMA_InitStructure.DMA_BufferSize = 1;//��Ϊһ��ֻ����һ������������Ϊ1��
	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//��Ϊֻ�漰һ·���ݵĲɼ���������ڴ��ַ����
	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��Ϊֻ�漰һ·���ݵĲɼ�������������ַ����
	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//��������Ҫ���ּ�16λ�ſ�������Ҫ��
	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ�����Ҫ���ּ�16λ�ſ�������Ҫ��
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMAģʽΪѭ�����䣬��ΪҪ�ɼ����;
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//����Ϊ�ߡ��С������ȼ���������Ϊֻ��һ·�ڲɼ�
	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�ر��ڴ浽�ڴ�Ĵ��䣬��Ϊ������Ҫ�������贫���ڴ�Ĵ���
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);//DMA1ͨ��1����ʼ��
	
	DMA_Cmd(DMA1_Channel1, ENABLE);//ʹ��DMA1��ͨ��1��
	

	
	/********�������й�ADC���������*************/
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//����Ϊ����ADCģʽ����Ϊ��ɼ�ֻ��һ��ͨ����
	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ֹɨ��ģʽ��ɨ��ģʽ�����ڶ�ͨ���ɼ�
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��������ת�����Բ�ͣ�ؽ���ADCת��
	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ʹ���ⲿ����ת������ʹ���ڲ��������
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�ɼ������Ҷ���
	
	ADC_InitStructure.ADC_NbrOfChannel = 1;//ADC number of channel,��Ҫת����ͨ����Ŀ��
	
	ADC_Init(ADC1, &ADC_InitStructure);//����ADC��ʼ���⺯��
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//����ADCʱ��Ϊ8��Ƶ��9MHZ
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);//����ADC1��ͨ��Ϊ55.5���������ڣ�
	
	ADC_DMACmd(ADC1, ENABLE);//ʹ��ADC1��DMA����
	
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1;
	
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�У׼�Ĵ�����λ��ɣ�
	
	ADC_StartCalibration(ADC1);//����У׼������ʼADCУ׼��
	
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�У׼�Ĵ�����λ��ɣ�
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ǰ�治�����ⲿ���������ǲ����ڲ�����������˴�ʹ���������
	

}

void ADC1_Init(void)
{
	 ADC1_GPIO_Config();
	 ADC1_Mode_Config();
		
}	
