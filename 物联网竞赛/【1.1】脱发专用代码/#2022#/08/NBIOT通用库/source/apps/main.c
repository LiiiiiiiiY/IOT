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
#include "usart1-board.h"
#include "uart_receiver.h"

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
void handlerPre10Ms(void) 
{
	for (int delay = 0; delay < 30; delay++) 
	{
		HAL_Delay(1000);
	}
}

int year=2022;
int month=12;
int day=31;
int hours=23;
int minute=59;
int second=50;
int flag=0;//主界面切换到设置界面的标志位
int hand_Count=1;//设置界面的左箭头移动排数
int hand_flag=0;
extern int flag_Uart;
uint8_t send_successful[3]={0xFB,0X00,0XFE};
uint8_t send_failyre[3]={0xFB,0X01,0XFE};

void Leap_year()
{
	if((year%4==0&&year%100!=0)||(year%400==0))
	{
		if(month==2)
		{
			if(day==29)
			{
				day=1;
				month++;
			}
		}
	}
	else
	{
		if(month==2)
		{
			if(day==28)
			{
				day=1;
				month++;
			}
		}
	}
	if(month==1||month==3||month==5||month==9||month==11)
	{
		if(day==31)
		{
			day=1;
			month++;
		}
	}
	else if(month<13&&month!=2)
	{
		if(day==32)
		{
			day=1;
			month++;
		}
	}
}

void menu()
{
	OLED_ShowCHinese(0 ,0,0);//当
	OLED_ShowCHinese(16,0,1);//前
	OLED_ShowCHinese(32,0,2);//时
	OLED_ShowCHinese(48,0,3);//间
	OLED_ShowCHinese(64,0,4);//：
	OLED_ShowNum(0,3,year,4,16);//年
	OLED_ShowChar(32,3,'-');
	OLED_ShowNum(41,3,month,2,16);//月
	OLED_ShowChar(58,3,'-');
	OLED_ShowNum(66,3,day,2,16);//日
	OLED_ShowNum(0,6,hours,2,16);//时
	OLED_ShowChar(16,6,':');
	OLED_ShowNum(24,6,minute,2,16);//分
	OLED_ShowChar(40,6,':');
	OLED_ShowNum(48,6,second,2,16);//秒
}

void time()
{
	if(second>=60)
	{
		second=0;
		minute++;
	}
	if(minute>=60)
	{
		minute=0;
		hours++;
	}
	if(hours>=24)
	{
		hours=0;
		day++;
	}	
	Leap_year();
	if(month>12)
	{
		month=1;
		year++;
	}
}

void shezhi()
{
	OLED_ShowCHinese(0,0,5);//时
	OLED_ShowCHinese(16,0,4);//：
	OLED_ShowNum(32,0,hours,4,16);//小时数
	
	OLED_ShowCHinese(0,3,6);//分
	OLED_ShowCHinese(16,3,4);//：
	OLED_ShowNum(32,3,minute,4,16);//分钟数
	
	OLED_ShowCHinese(0,6,7);//秒
	OLED_ShowCHinese(16,6,4);//：
	OLED_ShowNum(32,6,second,4,16);//秒数
}

void hand()
{
	if(hand_flag==0)
	{
		if(isKey2Pressed())
		{
			hand_Count--;
			if(hand_Count==0)hand_Count=3;
			resetKey2();
		}
		if(isKey3Pressed())
		{
			hand_Count++;
			if(hand_Count==4)hand_Count=1;
			resetKey3();
		}
		switch(hand_Count)
		{
		case 1:OLED_ShowCHinese(64,0,8);OLED_ShowString(64,3,(uint8_t *)"  ");OLED_ShowString(64,6,(uint8_t *)"  ");break;
		case 2:OLED_ShowCHinese(64,3,8);OLED_ShowString(64,0,(uint8_t *)"  ");OLED_ShowString(64,6,(uint8_t *)"  ");break;
		case 3:OLED_ShowCHinese(64,6,8);OLED_ShowString(64,0,(uint8_t *)"  ");OLED_ShowString(64,3,(uint8_t *)"  ");break;
		}
	}
	else if(hand_flag==1)
	{
		if(hand_Count==1)
		{
			if(isKey2Pressed())
			{
				resetKey2();
				hours++;
				if(hours>=24)hours=0;
			}
			if(isKey3Pressed())
			{
				resetKey3();
				hours--;
				if(hours<0)hours=23;
			}
		}
		else if(hand_Count==2)
		{
			if(isKey2Pressed())
			{
				resetKey2();
				minute++;
				if(minute>=60)minute=0;
			}
			if(isKey3Pressed())
			{
				resetKey3();
				minute--;
				if(minute<0)minute=59;
			}
		}
		else if(hand_Count==3)
		{
			if(isKey2Pressed())
			{
				resetKey2();
				second++;
				if(second>=60)second=0;
			}
			if(isKey3Pressed())
			{
				resetKey3();
				second--;
				if(second<0)second=59;
			}
		}
	}
	if(isKey4Pressed())
	{
		if(hand_flag==0)
		{
			hand_flag=1;
			switch(hand_Count)
			{
			case 1:OLED_ShowChar(80,0,'@');OLED_ShowChar(80,3,' ');OLED_ShowChar(80,6,' ');break;
			case 2:OLED_ShowChar(80,3,'@');OLED_ShowChar(80,0,' ');OLED_ShowChar(80,6,' ');break;
			case 3:OLED_ShowChar(80,6,'@');OLED_ShowChar(80,0,' ');OLED_ShowChar(80,3,' ');break;
			}
		}
		else
		{
			hand_flag=0;
			OLED_ShowChar(80,0,' ');
			OLED_ShowChar(80,3,' ');
			OLED_ShowChar(80,6,' ');
			WriteFlashTest(0x0800f400,hours);
			WriteFlashTest(0x0800f500,minute);
			WriteFlashTest(0x0800f600,second);
			WriteFlashTest(0x0800f700,year);
			WriteFlashTest(0x0800f800,month);
			WriteFlashTest(0x0800f900,day);
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
	USART1_Init(115200);
	hours = PrintFlashTest(0x0800f400);
	minute = PrintFlashTest(0x0800f500);
	second = PrintFlashTest(0x0800f600);
	year = PrintFlashTest(0x0800f700);
	month = PrintFlashTest(0x0800f800);
	day = PrintFlashTest(0x0800f900);
	while(1)
	{
		if(USART1_RX_COUNT>0)
		{
			HAL_Delay(10);
			if(USART1_RX_BUF[0]==0xFB&&USART1_RX_BUF[1]==0x04&&USART1_RX_BUF[5]==0xFE)
			{
				hours = USART1_RX_BUF[2];
				hours = (hours/16)*10 + hours%16;
				WriteFlashTest(0x0800f400,hours);
		
				minute = USART1_RX_BUF[3];
				minute = (minute/16)*10 + minute%16;
				WriteFlashTest(0x0800f500,minute);
		
				second = USART1_RX_BUF[4];
				second = ((second/16)*10 + second%16);
				WriteFlashTest(0x0800f600,second);
			
				USART1_SendStr(send_successful,3);
				USART1_RX_COUNT=0;
				USART1_ReceiveClr();
			}
			else
			{
				USART1_SendStr(send_failyre,3);
				USART1_RX_COUNT=0;
				USART1_ReceiveClr();
			}
		}
		if(flag==0)
		{
			menu();
			HAL_Delay(1000);
			second++;
			time();
			if(isKey4Pressed())
			{
				OLED_Clear();
				flag=1;
				resetKey4();
			}
		}
		else if(flag==1)
		{
			shezhi();
			hand();
		}
	}
}
