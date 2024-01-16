#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "bsp_adc.h"
#include "dht11.h"
#include "bodyir.h"
#include "timer.h"
#include "beep.h" 
#include "exti.h"

//模式0 仅仅灯光提示 模式1 灯光+蜂鸣器提示
#define warning_mode 0


extern __IO uint16_t ADC_ConvertedValue;
float ADC_ConvertedValueLocal;

u8 temp;
u8 humi;

u8 temp_warning;
u8 humi_warning;
float density_warning;

u8 temp_normal_flag=1;
u8 humi_normal_flag=1;
u8 density_normal_flag=1;

u8 num=0;
uint8_t data_select=0;


 int main(void)
 {	 
	 
	temp_warning=16;
	humi_warning=70;
	density_warning=1.4;
	 
	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 

 	LED_Init();			     //LED端口初始化
	LCD_Init();
	ADC1_Init();

	printf("初始化成功\r\n");

	 
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。
	 
	TIM3_Int_Init(99,7199);//10Khz的计数频率，计数到5000为500ms  
	 
 	BEEP_Init();         	//初始化蜂鸣器端口

	EXTIX_Init();		 	//外部中断初始化

	Bdir_init();//人体红外初始化

	while(DHT11_Init())
	{
		printf("DHT11 Error \r\n");
		delay_ms(1000);
	}
		 


	
  	while(1) 
	{		
		
		POINT_COLOR=GREEN;	        	
		
		LCD_ShowString(100,50,400,24,24,"my graduation project");
		LCD_ShowString(60,100,400,24,24,"my Student ID is 202004104215");	
		
		LCD_DrawRectangle(20,30,450,135);
		
		POINT_COLOR=BLUE;	

		ADC_ConvertedValueLocal = (float) ADC_ConvertedValue/4096*3.3;
		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal);
		
		LCD_ShowString(30,150,200,24,24,"data_name");	
		LCD_ShowString(180,150,200,24,24,"data_now");	
		LCD_ShowString(300,150,200,24,24,"data_warning");

		LCD_DrawRectangle(20,140,450,330);
		LCD_DrawLine(20,190,450,190);
		
		POINT_COLOR=RED;	        	
	
//横坐标 X	纵坐标 Y
		
		LCD_ShowString(30,200,200,24,24,"density");		
		LCD_ShowFloatNum(180,200,ADC_ConvertedValueLocal,5,24);
//		LCD_ShowFloatNum_Sprintf(180,200,ADC_ConvertedValueLocal,5,24);

		LCD_ShowFloatNum_Sprintf(300,200,density_warning,4,24);
				
		
		DHT11_Read_Data(&temp,&humi);//
		printf("temp %d ,humi %d\r\n",temp,humi);

		LCD_ShowString(30,250,200,16,24,"temp");		
		LCD_ShowNum(180,250,temp,4,24);
		LCD_ShowNum(300,250,temp_warning,4,24);
		
		LCD_ShowString(30,300,200,16,24,"humi");		
		LCD_ShowNum(180,300,humi,4,24);
		LCD_ShowNum(300,300,humi_warning,4,24);
		
		POINT_COLOR=MAGENTA;	        	

		if(temp_normal_flag==1) LCD_ShowString(30,350,200,16,24,"temp is normal"),LCD_Fill(30,400,30+400,400+16*2,WHITE);
		else if(temp_normal_flag==0) LCD_ShowString(30,400,400,16,24,"temp is exceed the standard"),LCD_Fill(30,350,230,350+16*2,WHITE);
		
		if(humi_normal_flag==1) LCD_ShowString(30,450,200,16,24,"humi is normal"),LCD_Fill(30,500,30+400,500+16*2,WHITE);
		else if(humi_normal_flag==0) LCD_ShowString(30,500,400,16,24,"humi is exceed the standard"),LCD_Fill(30,450,30+200,450+16*2,WHITE);
			
		if(density_normal_flag==1) LCD_ShowString(30,550,200,16,24,"density is normal"),LCD_Fill(30,600,30+400,600+16*2,WHITE);
		else if(density_normal_flag==0) LCD_ShowString(30,600,400,16,24,"density is exceed the standard"),LCD_Fill(30,550,30+200,550+16*2,WHITE);
			
		LCD_DrawRectangle(20,340,450,630);

		POINT_COLOR=BRRED;	        	
		
		LCD_ShowString(30,650,200,16,24,"Number of Passes");	
		LCD_ShowNum(250,650,num,4,24);
		
		LCD_DrawRectangle(20,640,450,680);
		
		POINT_COLOR=BLACK;	        	

		LCD_ShowString(30,700,150,16,24,"Data select");	

		LCD_ShowNum(160,700,data_select,3,24);

		LCD_ShowString(30,750,450,16,24,"A good beginning is half the battle");	



		POINT_COLOR=RED;	        	

		if(data_select==1)LCD_Draw_Circle(400,210,12),LCD_Fill(400-12,260-12,400+12,260+12,WHITE),LCD_Fill(400-12,310-12,400+12,310+12,WHITE);
		else if(data_select==2)LCD_Draw_Circle(400,260,12),LCD_Fill(400-12,210-12,400+12,210+12,WHITE),LCD_Fill(400-12,310-12,400+12,310+12,WHITE);
		else if(data_select==3)LCD_Draw_Circle(400,310,12),LCD_Fill(400-12,210-12,400+12,210+12,WHITE),LCD_Fill(400-12,260-12,400+12,260+12,WHITE);
		else if(data_select==0)LCD_Fill(400-12,310-12,400+12,310+12,WHITE),LCD_Fill(400-12,210-12,400+12,210+12,WHITE),LCD_Fill(400-12,260-12,400+12,260+12,WHITE);
		
			
			
		LED0=!LED0;				   		 
		delay_ms(1000);	

	} 
}
 



void data_warning_check(u8 temp,u8 humi ,float ADC_ConvertedValueLocal)
{
	if(temp>=temp_warning)
	{
		#if warning_mode==1
		BEEP=1;
		R_LED=1; 
		#elif warning_mode==0
		R_LED=1; 
		#endif	
		temp_normal_flag=0;		
	}	
	else if(temp<temp_warning)
	{
		BEEP=0;
		R_LED=0; 
		temp_normal_flag=1;	
	}

		if(humi>=humi_warning)
	{
		#if warning_mode==1
		BEEP=1;
		G_LED=1; 
		#elif warning_mode==0
		G_LED=1; 
		#endif		
		humi_normal_flag=0;
	}	
	else if(humi<humi_warning)
	{
		BEEP=0;
		G_LED=0; 
		humi_normal_flag=1;

	}
	
		if(ADC_ConvertedValueLocal>=density_warning)
	{
		#if warning_mode==1
		BEEP=1;
		B_LED=1; 
		#elif warning_mode==0
		B_LED=1; 
		#endif		
		density_normal_flag=0;
	}	
	else if(ADC_ConvertedValueLocal<density_warning)
	{
		BEEP=0;
		B_LED=0; 
		density_normal_flag=1;

	}	


	

}	

u8 time=0;
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
	
			
		u8 value;
		value=GPIO_ReadInputDataBit(BDYIR_PORT, BDYIR_GPIO_PIN); //读取PB9引脚
			
	
		if(value == 1)
		{
			time++;
			if(time==20)	time=0,num++;			
		}

		
		data_warning_check(temp,humi,ADC_ConvertedValueLocal);	
			
//		G_LED=~G_LED;	
		}
}


//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	
	delay_ms(10);//消抖
	if(WK_UP==1)	 	 //WK_UP按键
	{				 

		data_select++;
		if(data_select==0);
		else if	(data_select==1)	;
		else if	(data_select==2)	;
		else if	(data_select==3)	;
		else if	(data_select==4) data_select=0 ;		
		
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}

//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 
		if(data_select==2)
		{
			temp_warning++;
		}
		else if(data_select==3)
		{
			humi_warning++;
		}	
		else if(data_select==1)
		{
			density_warning=density_warning+0.1;
		}			
		
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0
	{
		
		if(data_select==2)
		{
			temp_warning--;
		}
		else if(data_select==3)
		{
			humi_warning--;
		}	
		else if(data_select==1)
		{
			density_warning=density_warning-0.1;
		}	
		
	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
 

