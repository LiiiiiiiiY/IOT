/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  */
#include <string.h>
#include "board.h"
#include "hal_key.h"
#include "tim-board.h"
#include "timer_handles.h"
#include "adc_reader.h"
#include "math.h"
#include "usart1-board.h"

#define KEY2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)
//#include "sx1276.h"
/**********************************************************************************************
*函数：void Init( void )
*功能：平台初始化
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void Init() {
    // 开发板平台初始化
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
}


uint8_t str[9]={0xA5,0X06,0XFF,0XA0,0,0,0,0XEE,0X5A};
int red=0x00,green=0x00,blue=0x00,point=0,pointFlag=0,mediumCount;

void oled()
{
	OLED_ShowString(0,0,(uint8_t *)"Red:");
	OLED_ShowString(0,3,(uint8_t *)"Green:");
	OLED_ShowString(0,6,(uint8_t *)"Bule:");
	OLED_ShowNum(48,0,red,3,16);
	OLED_ShowNum(48,3,green,3,16);
	OLED_ShowNum(48,6,blue,3,16);
}

void data2medium()
{
	switch(point)
	{
		case 0:mediumCount=red;break;
		case 3:mediumCount=green;break;
		case 6:mediumCount=blue;break;
	}
}

void medium2data()
{
	switch(point)
	{
		case 0:red=mediumCount;break;
		case 3:green=mediumCount;break;
		case 6:blue=mediumCount;break;
	}
	OLED_ShowNum(48,0,red,3,16);
	OLED_ShowNum(48,3,green,3,16);
	OLED_ShowNum(48,6,blue,3,16);
}

void pointChange()
{
	OLED_ShowString(80,point,(uint8_t*)"<");
	if(pointFlag==0)
	{
		if(isKey2Pressed())
		{
			OLED_Clear_Page(point);
			if(point==0)point=6;
			else point-=3;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			OLED_Clear_Page(point);
			if(point==6)point=0;
			else point+=3;
			resetKey3();
		}
		else if(isKey4Pressed())
		{
			pointFlag=1;
			OLED_ShowString(96,point,(uint8_t*)"@");
			resetKey4();
		}
		
	}
	else if(pointFlag==1)
	{
		if(KEY2==0)
		{
			HAL_Delay(10); 
			if(KEY2==0)
			{
				data2medium();
				while(!KEY2)
				{
					HAL_Delay(100); 
					if(mediumCount==255)mediumCount=0;
					else mediumCount++;
					medium2data();
				}
				resetKey2();
			}
		}
		else if(KEY3==0)
		{
			data2medium();
			while(!KEY3)
			{
				HAL_Delay(100); 
				if(mediumCount==0)mediumCount=255;
				else mediumCount--;
				medium2data();
			}
			resetKey3();
		}
		else if(isKey4Pressed())
		{
			OLED_ShowString(96,point,(uint8_t*)" ");
			str[4]=red;
			str[5]=green;
			str[6]=blue;
			USART1_SendStr(str,9);
			pointFlag=0;
			resetKey4();
		}
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init_Inverse();
	USART1_Init(9600);
	/*
		在程序烧写之前
		先将OLED下方的拨片拨至J6方向(右边)
		让U1串口连接上RS485通道
	*/
	while(1)
	{
		oled();
		pointChange();
	}
}
