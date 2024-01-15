#ifndef DHT11_H
#define DHT11_H

#include "sys.h"

#define DHT11_IO_IN()  {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=8<<20;}//上拉下拉输入
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFF0FFFFF;GPIOA->CRL|=3<<20;}//输出，50MHz

////PA11
//#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=8<<4;}
//#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFF0F;GPIOA->CRL|=3<<4;}


//IO操作函数   
#define	DHT11_DQ_OUT PAout(5)
#define	DHT11_DQ_IN  PAin(5) 

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取数据
u8 DHT11_Read_Byte(void);//读取一个字节
u8 DHT11_Read_Bit(void);//读取一位
u8 DHT11_Check(void);//检测DHT11
void DHT11_Rst(void);//复位DHT11   

#endif
