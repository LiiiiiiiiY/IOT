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
#include "flash.h"
#include "hal_oled.h"

uint16_t tenMSecClick=0;																		// 10毫秒计时器



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

unsigned int year;
unsigned int munth;
unsigned int day;
unsigned int flag=0;
unsigned int count=1;
unsigned int sure_flag;

void year_munth_day()
{
	if((year%4==0&&year%100!=0)||(year%400==0))
	{
		if(munth==2)
		{
			if(day>29)
			{
				day=1;
			}
			else if(day==0)
			{
				day=29;
			}
		}
		else if(munth==1||munth==3||munth==5||munth==7||munth==8||munth==10||munth==12)
		{
			if(day>31)
			{
				day=1;
			}
			else if(day==0)
			{
				day=31;
			}
		}
		else if(munth==4||munth==6||munth==9||munth==11)
		{
			if(day>30)
			{
				day=1;
			}
			else if(day==0)
			{
				day=30;
			}
		}
	}
	else
	{
		if(munth==2)
		{
			if(day>28)
			{
				day=1;
			}
			else if(day==0)
			{
				day=28;
			}
		}
		else if(munth==1||munth==3||munth==5||munth==7||munth==8||munth==10||munth==12)
		{
			if(day>31)
			{
				day=1;
			}
			else if(day==0)
			{
				day=31;
			}
		}
		else if(munth==4||munth==6||munth==9||munth==11)
		{
			if(day>30)
			{
				day=1;
			}
			else if(day==0)
			{
				day=30;
			}
		}
	}
	
}

void menu()
{
	OLED_ShowCHinese(0,0,0);//当
	OLED_ShowCHinese(16,0,1);//前
	OLED_ShowCHinese(32,0,2);//日
	OLED_ShowCHinese(48,0,3);//期
	OLED_ShowCHinese(64,0,4);//：
	OLED_ShowCHinese(32,3,5);//年
	OLED_ShowCHinese(16,6,6);//月
	OLED_ShowCHinese(48,6,2);//日
	OLED_ShowNum(0,3,year,4,16);//年份
	OLED_ShowNum(0,6,munth,2,16);//月份
	OLED_ShowNum(32,6,day,2,16);//日期
	if(isKey4Pressed())
	{
		if(flag==0)flag=1;
		OLED_Clear();
		resetKey4();
	}
}

void time_menu()
{
	OLED_ShowCHinese(0 ,0,5);//年
	OLED_ShowCHinese(16,0,4);//：
	OLED_ShowCHinese(0 ,3,6);//月
	OLED_ShowCHinese(16,3,4);//：
	OLED_ShowCHinese(0 ,6,2);//日
	OLED_ShowCHinese(16,6,4);//：
	OLED_ShowNum(32,0,year,4,16);//年份
	OLED_ShowNum(48,3,munth,2,16);//月份
	OLED_ShowNum(48,6,day,2,16);//日期
}

void move()//三角形移动
{
	if(sure_flag==0)
	{
		if(isKey3Pressed())
		{
			count++;
			if(count==5)count=2;
			resetKey3();
		}
		if(isKey2Pressed())
		{
			count--;
			if(count==1)count=4;
			else if(count==0)count=2;
			resetKey2();
		}
		switch(count)
		{
		case 2:OLED_ShowCHinese(80,0,8);OLED_ShowString(80,3,(uint8_t *)"  ");OLED_ShowString(80,6,(uint8_t *)"  ");break;
		case 3:OLED_ShowCHinese(80,3,8);OLED_ShowString(80,0,(uint8_t *)"  ");OLED_ShowString(80,6,(uint8_t *)"  ");break;
		case 4:OLED_ShowCHinese(80,6,8);OLED_ShowString(80,0,(uint8_t *)"  ");OLED_ShowString(80,3,(uint8_t *)"  ");break;
		}
	}
}

void shezhi()
{
	if(count==2)
	{
		if(isKey2Pressed())
		{
			++year;
			resetKey2();
		}
		if(isKey3Pressed())
		{
			--year;
			resetKey3();
		}
	}
	else if(count==3)
	{
		if(isKey2Pressed())
		{
			++munth;
			if(munth==13)munth=1;
			resetKey2();
		}
		if(isKey3Pressed())
		{
			--munth;
			if(munth==0)munth=12;
			resetKey3();
		}
	}
	else if(count==4)
	{
		if(isKey2Pressed())
		{
			++day;
			year_munth_day();
			resetKey2();
		}
		if(isKey3Pressed())
		{
			--day;
			year_munth_day();
			resetKey3();
		}
	}
}
unsigned int a=0;
void time_hand()
{
	move();
	if(isKey4Pressed())
	{
		if(sure_flag==1)
		{
			sure_flag=0;
			a=0;
			GpioWrite( &Led2, 1 );
			OLED_ShowChar(96,0,' ');
			OLED_ShowChar(96,3,' ');
			OLED_ShowChar(96,6,' ');
			WriteFlashTest(0x0800f400,day);
			WriteFlashTest(0x0800f700,munth);
			WriteFlashTest(0x0800f900,year);
		}
		else if(sure_flag==0)
		{
			sure_flag=1;
		}
		if(sure_flag==1)
		{
			switch(count)
			{
			case 2:OLED_ShowChar(96,0,'@');OLED_ShowChar(96,3,' ');OLED_ShowChar(96,6,' ');a=1;GpioWrite( &Led2, 0 );break;
			case 3:OLED_ShowChar(96,3,'@');OLED_ShowChar(96,0,' ');OLED_ShowChar(96,6,' ');a=1;GpioWrite( &Led2, 0 );break;
			case 4:OLED_ShowChar(96,6,'@');OLED_ShowChar(96,3,' ');OLED_ShowChar(96,0,' ');a=1;GpioWrite( &Led2, 0 );break;
			}
		}
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
	day = PrintFlashTest(0x0800f400);
	munth = PrintFlashTest(0x0800f700);
	year = PrintFlashTest(0x0800f900);
  while(1)
  {
		if(flag==0)
		{
			menu();
		}
		else if(flag==1)
		{
			time_menu();
			time_hand();
		}
		if(a==1)
		{
			shezhi();
		}

  }
}





