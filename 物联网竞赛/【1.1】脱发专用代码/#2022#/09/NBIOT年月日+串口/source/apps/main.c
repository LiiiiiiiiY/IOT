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

unsigned int year=2022;//定义一个变量来表示年份
unsigned int month=9;//定义一个变量来表示月份
unsigned int day=28;//定义一个变量来表示天数

unsigned int year1;
unsigned int year2;

uint8_t yes[3]={0XFB,0X00,0XFE};
uint8_t no[3]={0XFB,0X01,0XFE};

int flag=0;//初始化界面和设置界面区分的标志位（当在初始化界面时，按下key4->flag=1->进入设置界面）
int count=2;
int Key4Flag=0;


/************************************判断闰年*********************************/
void day_count()
{
	if((year%4==0&&year%100!=0)||(year%400==0))
	{
		if(month==2)
		{
			if(day>29)day=1;
			else if(day==0)day=29;
		}
		else if(month==1||month==3||month==5||month==9||month==11)
		{
			if(day>30)day=1;
			else if(day==0)day=30;
		}
		else
		{
			if(day>31)day=1;
			else if(day==0)day=31;
		}
	}
	else
	{
	  if(month==2)
		{
			if(day>28)day=1;
			else if(day==0)day=28;
		}
		else if(month==1||month==3||month==5||month==9||month==11)
		{
			if(day>30)day=1;
			else if(day==0)day=30;
		}
		else
		{
			if(day>31)day=1;
			else if(day==0)day=31;
		}
	}
}

//

/************************************主界面的日期显示*********************************/
void time()
{
	OLED_ShowCHinese(0 ,0,0);//当
	OLED_ShowCHinese(16,0,1);//前
	OLED_ShowCHinese(32,0,2);//日
	OLED_ShowCHinese(48,0,3);//期
	OLED_ShowCHinese(64,0,7);//：
	OLED_ShowCHinese(32,3,4);//年
	OLED_ShowCHinese(16,6,5);//月
	OLED_ShowCHinese(48,6,6);//日
	OLED_ShowNum(0,3,year,4,16);
	OLED_ShowNum(0,6,month,2,16);
	OLED_ShowNum(32,6,day,2,16);
}
//


/******************************************设置界面显示************************************/
void Set_up()
{
	OLED_ShowCHinese(0 ,0,4);//年
	OLED_ShowCHinese(16,0,7);//：
	OLED_ShowCHinese(0 ,3,5);//月
	OLED_ShowCHinese(16,3,7);//：
	OLED_ShowCHinese(0 ,6,6);//日
	OLED_ShowCHinese(16,6,7);//：
	OLED_ShowNum(48,0,year,4,16);
	OLED_ShowNum(48,3,month,4,16);
	OLED_ShowNum(48,6,day,4,16);
}


/*************************************用Key2和Key3改变年月日****************************/
void timeChange()
{
	if(count==2)
	{
		OLED_ShowString(96,0,(uint8_t *)"@");
		OLED_ShowString(96,3,(uint8_t *)" ");
		OLED_ShowString(96,6,(uint8_t *)" ");
		if(isKey2Pressed())
		{
			year++;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			year--;
			resetKey3();
		}
	}
	if(count==3)
	{
		OLED_ShowString(96,0,(uint8_t *)" ");
		OLED_ShowString(96,3,(uint8_t *)"@");
		OLED_ShowString(96,6,(uint8_t *)" ");
		if(isKey2Pressed())
		{
			month++;
			if(month>12)month=1;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			month--;
			if(month==0)month=12;
			resetKey3();
		}
	}
	if(count==4)
	{
		OLED_ShowString(96,0,(uint8_t *)" ");
		OLED_ShowString(96,3,(uint8_t *)" ");
		OLED_ShowString(96,6,(uint8_t *)"@");
		if(isKey2Pressed())
		{
			day++;
			day_count();
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			day--;
			day_count();
			resetKey3();
		}
	}
}

/******************************在设置界面按下Key4开启更改和确认更改*******************/
void Key4_Scan()
{
	if(count!=1)
	{
		if(isKey4Pressed())
		{
			if(Key4Flag==0)
			{
				Key4Flag=1;
				
			}
		else if(Key4Flag==1)
		{
			Key4Flag=0;
			GpioWrite(&Led2,1);
			WriteFlashTest(0x0800f500,year);
			WriteFlashTest(0x0800f600,month);
			WriteFlashTest(0x0800f700,day);
			OLED_ShowString(96,0,(uint8_t *)" ");
			OLED_ShowString(96,3,(uint8_t *)" ");
			OLED_ShowString(96,6,(uint8_t *)" ");
		}
		resetKey4();
	}
	if(Key4Flag==1)timeChange();
	}
}
/**************在设置界面移动箭头选择更改年月日*********************/
void Pointer()
{
		if(isKey2Pressed())
		{
			count--;
			if(count==1)count=4;
			resetKey2();
		}
		if(isKey3Pressed())
		{
			count++;
			if(count==5)count=2;
			resetKey3();
		}
		switch(count)
		{
		case 2:OLED_ShowCHinese(80,0,8);OLED_ShowString(80,3,(uint8_t *)"  ");OLED_ShowString(80,6,(uint8_t *)"  ");break;
		case 3:OLED_ShowCHinese(80,3,8);OLED_ShowString(80,6,(uint8_t *)"  ");OLED_ShowString(80,0,(uint8_t *)"  ");break;
		case 4:OLED_ShowCHinese(80,6,8);OLED_ShowString(80,3,(uint8_t *)"  ");OLED_ShowString(80,0,(uint8_t *)"  ");break;
		}
}

/**
 * Main application entry point.
 */

//uint8_t send[3]={0xFB,0x00,0xFE};
//uint8_t recv_send[3]={0XFE,0X01,0XFE};
int main( void )
{
  Init();
	USART1_Init(115200);
	OLED_Init();
	year = PrintFlashTest(0x0800f500);
	month = PrintFlashTest(0x0800f600);
	day = PrintFlashTest(0x0800f700);
  while( 1 )
  {
		if(USART1_RX_COUNT>0)
		{
			HAL_Delay(10);
			if((USART1_RX_BUF[0]==0xFB) && (USART1_RX_BUF[1]==0x01) && (USART1_RX_BUF[4]==0xFE))//如果接受到更改年份的数据
			{
				year1 = USART1_RX_BUF[2];//将数组第三位的数据赋值给年1
				year2 = USART1_RX_BUF[3];//将数组第四位的数据赋值给年2
				year1 = (year1/16)*10+(year1%16);//年1——>接收16进制转10进制
				year2 = (year2/16)*10+(year2%16);//年2——>接收16进制转10进制
				year  = (year1*100) + year2;//最终年份=年1的十进制*100+年2的十进制
				WriteFlashTest(0x0800f500,year);//将最终得到的年份数据存放在flash的0x0800f500中
				USART1_SendStr((uint8_t *)yes,3);//向上位机发出成功的信号
				USART1_ReceiveClr();
			}
			else if((USART1_RX_BUF[0]==0xFB) && (USART1_RX_BUF[1]==0x02) && (USART1_RX_BUF[3]==0xFE))//如果接受到更改月份的数据
			{
				month = USART1_RX_BUF[2];//将数组第三位的数据赋值给月份值
				month = (month/16)*10 + (month%16);//月份值——>接收16进制转10进制
				WriteFlashTest(0x0800f600,month);//将最终得到的月份数据存放在flash的0x0800f600中
				USART1_SendStr((uint8_t *)yes,3);//向上位机发出成功的信号
				USART1_ReceiveClr();
			}
			else if( (USART1_RX_BUF[0]==0xFB) && (USART1_RX_BUF[1]==0x03) && (USART1_RX_BUF[3]==0xFE) )//如果接受到更改日期的数据
			{
				day = USART1_RX_BUF[2];		//将数组第三位的数据赋值给日期值
				day = (day/16)*10 + (day%16);//日期值——>接收16进制转10进制
				WriteFlashTest(0x0800f700,day);//将最终得到的日期数据存放在flash的0x0800f700中
				USART1_SendStr((uint8_t *)yes,3);//向上位机发出成功的信号
				USART1_ReceiveClr();
			}
			else//如果以上条件都不满足，那表示没有接收到正确的消息（即：失败）
			{
				USART1_SendStr((uint8_t *)no,3);//向上位机发送失败的信号
				USART1_ReceiveClr();
			}
		}
		if(flag==0)
		{
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
			Set_up();
			if(Key4Flag==0)Pointer();
			Key4_Scan();
		}
  }
}
