#ifndef DHT11_H
#define DHT11_H

#include "sys.h"

#define DHT11_IO_IN()  {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=8<<20;}//������������
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=3<<20;}//�����50MHz

////PA11
//#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=8<<4;}
//#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=3<<4;}


//IO��������   
#define	DHT11_DQ_OUT PAout(5)
#define	DHT11_DQ_IN  PAin(5) 

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ����
u8 DHT11_Read_Byte(void);//��ȡһ���ֽ�
u8 DHT11_Read_Bit(void);//��ȡһλ
u8 DHT11_Check(void);//���DHT11
void DHT11_Rst(void);//��λDHT11   

#endif
