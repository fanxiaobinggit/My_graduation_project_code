#include "stm32f10x.h"
#include "bodyir.h"

void Bdir_init(void)//�������ģ��ܽ�����
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	
	RCC_APB2PeriphClockCmd(BDYIR_GPIO_CLK,ENABLE);//����ʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//���ø�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//����ʱ��Ƶ��
	GPIO_InitStructure.GPIO_Pin=BDYIR_GPIO_PIN;//����ܽ�
	
	GPIO_Init(BDYIR_PORT,&GPIO_InitStructure);//��ʼ��

	
}

