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
#include "hal_oled.h"
#include "flash.h"

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

/**********************************************************************************************
*函数：void KeyDownHandler( void )
*功能：按钮事件监听
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void KeyDownHandler(void) {
	
}

/**********************************************************************************************
*函数：void handlerPre10Ms( void )
*功能：10毫秒循环
*输入：无
*输出：无
*特殊说明：循环处理总时长300ms
**********************************************************************************************/
void handlerPre10Ms(void) {
    for (int delay = 0; delay < 30; delay++) {
        HAL_Delay(10);
    }
}

int year,month,day;
int interface=0;

void rule()
{
	
	if((year%4==0&&year%100!=0)||(year%400==0))
	{
		if(month==2)
		{
			if(day>29)day=1;
			else if(day==0)day=29;
		}
	}
	else
	{
		if(month==2)
		{
			if(day>28)day=1;
			else if(day==0)day=28;
		}
	}
	
	if(month==1||month==3||month==5||month==7||month==8||month==11)
	{
		if(day>31)day=1;
		else if(day==0)day=31;
	}
	else if(month==4||month==6||month==9||month==12)
	{
		if(day>30)day=1;
		else if(day==0)day=30;
	}
	
	if(month>12)month=1;
	else if(month==0)month=12;
}

void mune()
{
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);
	OLED_ShowString(64,0,(uint8_t *)":");
	OLED_ShowNum(0,3,year,4,16);
	OLED_ShowCHinese(32,3,4);
	OLED_ShowNum(0,6,month,2,16);
	OLED_ShowCHinese(16,6,5);
	OLED_ShowNum(32,6,day,2,16);
	OLED_ShowCHinese(48,6,6);
}

void Second_Screen()
{
	OLED_ShowCHinese(0,0,4);
	OLED_ShowCHinese(0,3,5);
	OLED_ShowCHinese(0,6,6);
	OLED_ShowString(16,0,(uint8_t *)":");
	OLED_ShowString(16,3,(uint8_t *)":");
	OLED_ShowString(16,6,(uint8_t *)":");
	OLED_ShowNum(32,0,year,4,16);
	OLED_ShowNum(32,3,month,4,16);
	OLED_ShowNum(32,6,day,4,16);
}

int count=0;
void pointer()
{
	if(isKey2Pressed())
	{
		OLED_Clear();
		if(count==0)count=6;
		else count=count-3;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		OLED_Clear();
		if(count==6)count=0;
		else count=count+3;
		resetKey3();
	}
	else if(isKey4Pressed())
	{
		interface = 2;
		GpioWrite(&Led2,0);
		OLED_ShowString(112,count,(uint8_t *)"@");
		resetKey4();
	}
	OLED_ShowCHinese(85,count,7);
}
int middle;//中间变量
void add_subtract()
{
	if(isKey2Pressed())
	{
		middle++;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		middle--;
		resetKey3();
	}
}

void change()
{
	if(count==0)
	{
		middle=year;
		add_subtract();
		year = middle;
		rule();
	}
	else if(count==3)
	{
		middle=month;
		add_subtract();
		month = middle;
		rule();
	}
	else if(count==6)
	{
		middle=day;
		add_subtract();
		day = middle;
		rule();
	}
	if(isKey4Pressed())
	{
		interface=1;
		GpioWrite(&Led2,1);
		OLED_ShowString(112,count,(uint8_t *)" ");
		WriteFlashTest(0x0800f300,day);
		WriteFlashTest(0x0800f400,month);
		WriteFlashTest(0x0800f500,year);

		resetKey4();
	}
}

void exchange()
{
	if(isKey4Pressed())
	{
		interface=1;
		OLED_Clear();
		resetKey4();
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	day = PrintFlashTest(0x0800f300);
	month = PrintFlashTest(0x0800f400);
	year = PrintFlashTest(0x0800f500);
	mune();
  while(1)
  {
		if(interface == 0)
		{
			exchange();
		}
		else if(interface == 1)
		{
			Second_Screen();
			pointer();
		}
		else if(interface == 2)
		{
			change();
			Second_Screen();
		}
  }
}
