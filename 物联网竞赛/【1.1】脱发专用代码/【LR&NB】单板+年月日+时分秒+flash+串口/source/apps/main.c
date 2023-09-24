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
#include "usart1-board.h"
#include "uart_receiver.h"
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
		
    Tim3McuInit(4000);
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

int year,month,day,hour,minus,pointer_count=0,ensure=0,medianData;
extern int second,interface_flag;
char Show_data[64];
uint8_t win[3]={0xFB,0x00,0xFE};
uint8_t lose[3]={0xFB,0x01,0xFE};

void rule_()
{
	if(second>59)
	{
		second=0;
		minus++;
	}
	if(minus>59)
	{
		minus=0;
		hour++;
	}
	if(hour>23)
	{
		hour=0;
		day++;
	}

	if(((year%4==0&&year%100!=0)||(year%400==0))&&(month==2))
	{
		if(day>29)
		{
			day=1;
			month++;
		}
	}
	else if(month==2)
	{
		if(day>28)
		{
			day=1;
			month++;
		}
	}
	
	if(month==1||month==3||month==5||month==7||month==8||month==11)
	{
		if(day>31)
		{
			day=1;
			month++;
		}
	}
	else if(month==4||month==6||month==9||month==12)
	{
		if(day>30)
		{
			day=1;
			month++;
		}
	}
	if(month>12)
	{
		month=1;
		year++;
	}
}

void rule()
{
	if(second>59)second=1;
	else if(second<0)second=59;
	
	if(minus>59)minus=0;
	else if(minus<0)minus=59;
	
	if(hour>23)hour = 0;
	else if(hour<0)hour=23;
	
	if(month>12)month=1;
	
	if(((year%4==0&&year%100!=0)||(year%400==0))&&(month==2))
	{
		if(day>29)day=1;
	}
	else if(month==2)
	{
		if(day>28)day=1;
	}
	
	if(month==1||month==3||month==5||month==7||month==8||month==11)
	{
		if(day>31)day=1;
	}
	else if(month==4||month==6||month==9||month==12)
	{
		if(day>30)day=1;
	}
}

void mune()
{
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);
	OLED_ShowString(64,0,(uint8_t *)":");
	
	sprintf(Show_data,"%04d-%02d-%02d  ",year,month,day);
	OLED_ShowString(0,3,(uint8_t *)Show_data);
	sprintf(Show_data,"%02d:%02d:%02d  ",hour,minus,second);//02d中的【2】代表保留两位整数，【0】代表不够两位的向左加0
	OLED_ShowString(0,6,(uint8_t *)Show_data);
}


void KeyScan_interface()
{
	if(isKey4Pressed())
	{
		OLED_Clear();
		interface_flag=1;
		resetKey4();
	}
}	

void change_interface()
{
	OLED_ShowCHinese(0,0,4);
	OLED_ShowCHinese(0,3,5);
	OLED_ShowCHinese(0,6,6);
	OLED_ShowString(16,0,(uint8_t *)":");
	OLED_ShowString(16,3,(uint8_t *)":");
	OLED_ShowString(16,6,(uint8_t *)":");
	OLED_ShowNum(48,0,hour,4,16);
	OLED_ShowNum(48,3,minus,4,16);
	OLED_ShowNum(48,6,second,4,16);
}


void pointer()
{
	if(isKey2Pressed())
	{
		OLED_Clear_Page(pointer_count);
		if(pointer_count==0)pointer_count=6;
		else pointer_count=pointer_count-3;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		OLED_Clear_Page(pointer_count);
		if(pointer_count==6)pointer_count=0;
		else pointer_count=pointer_count+3;
		resetKey3();
	}
	else if(isKey4Pressed())
	{
		ensure=1;
		OLED_ShowString(112,pointer_count,(uint8_t *)"@");
		GpioWrite(&Led2,0);
		resetKey4();
	}
	OLED_ShowCHinese(84,pointer_count,7);
}


void changed()
{
	if(isKey2Pressed())
	{
		medianData++;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		medianData--;
		resetKey3();
	}
}

void median()
{
	if(pointer_count==0)
	{
		medianData = hour;
		changed();
		hour = medianData;
	}
	else if(pointer_count==3)
	{
		medianData = minus;
		changed();
		minus = medianData;
	}
	else if(pointer_count==6)
	{
		medianData = second;
		changed();
		second = medianData;
	}
}

void change_Key4()
{
	if(isKey4Pressed())
	{
		ensure=0;
		OLED_ShowString(112,pointer_count,(uint8_t *)" ");
		GpioWrite(&Led2,1);
		resetKey4();
		WriteFlashTest(0x0800f300,hour);
		WriteFlashTest(0x0800f400,minus);
		WriteFlashTest(0x0800f500,second);
		WriteFlashTest(0x0800f600,year);
		WriteFlashTest(0x0800f700,month);
		WriteFlashTest(0x0800f800,day);
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	USART1_Init(115200);
	OLED_Init();
	hour = PrintFlashTest(0x0800f300);
	minus = PrintFlashTest(0x0800f400);
	second = PrintFlashTest(0x0800f500);
	year = PrintFlashTest(0x0800f600);
	month = PrintFlashTest(0x0800f700);
	day = PrintFlashTest(0x0800f800);
  while(1)
  {
		if(USART1_RX_COUNT>0)
		{
			HAL_Delay(10);
			if((USART1_RX_BUF[0]==0xfb) && (USART1_RX_BUF[1]==0x04) && (USART1_RX_BUF[5]==0xfe))
			{
				hour = USART1_RX_BUF[2];
				hour = (hour/16)*10+(hour%16);
				WriteFlashTest(0x0800f300,hour);
				
				minus = USART1_RX_BUF[3];
				minus = (minus/16)*10+(minus%16);
				WriteFlashTest(0x0800f400,minus);
				
				second = USART1_RX_BUF[4];
				second = (second/16)*10+(second%16);
				WriteFlashTest(0x0800f500,second);
				
				USART1_SendStr((uint8_t *)win,3);
				USART1_ReceiveClr();
			}
			else
			{
				USART1_SendStr((uint8_t *)lose,3);
				USART1_ReceiveClr();
			}
		}
		
		if(interface_flag==0)
		{
			rule_();
			mune();
			setTimer3Callback(Time3Handler);
			KeyScan_interface();
		}
		else if(interface_flag==1)
		{
			if(ensure==0)pointer();
			if(ensure==1)
			{
				median();
				change_Key4();
			}
			rule();
			change_interface();
		}
  }
}
