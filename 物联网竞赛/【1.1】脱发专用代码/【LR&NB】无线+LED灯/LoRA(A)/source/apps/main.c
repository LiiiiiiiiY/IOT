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
#include "NS_Radio.h"
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
unsigned int count=0;
uint8_t Send[4]={0xAB,1,1,0xBA};
void jiemian()
{
	OLED_ShowString(0,0,(uint8_t *)"LED1:");
	OLED_ShowString(0,4,(uint8_t *)"LED2:");
	OLED_ShowCHinese(48,0,0);
	OLED_ShowCHinese(80,0,1);
	OLED_ShowCHinese(48,4,0);
	OLED_ShowCHinese(80,4,1);
}

void key()
{
	if(count==0)
	{
		if(isKey2Pressed()||isKey3Pressed())
		{
			count=1;
			resetKey2();resetKey3();
		}
	}
	else
	{
		if(isKey2Pressed())
		{
			if(count==1||count==2)OLED_Clear_Page(2);
			else if(count==3||count==4)OLED_Clear_Page(6);
			
			if(count==1)count=4;
			else count--;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			if(count==1||count==2)OLED_Clear_Page(2);
			else if(count==3||count==4)OLED_Clear_Page(6);
			
			if(count==4)count=1;
			else count++;
			resetKey3();
		}
	}
	switch(count)
	{
		case 1:OLED_ShowCHinese(48,2,2);break;
		case 2:OLED_ShowCHinese(80,2,2);break;
		case 3:OLED_ShowCHinese(48,6,2);break;
		case 4:OLED_ShowCHinese(80,6,2);break;
	}
	if(isKey4Pressed())
	{
		switch(count)
		{
			case 1:
				Send[1]=0;
			GpioWrite(&Led1,0);break;
			case 2:
				Send[1]=1;
			GpioWrite(&Led1,1);break;
			case 3:
				Send[2]=0;
			GpioWrite(&Led2,0);break;
			case 4:
				Send[2]=1;
			GpioWrite(&Led2,1);break;
		}
		Radio.Send(Send,4);
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
	NS_RadioInit(433003000,10,1000,1000);
	jiemian();
	while(1)
	{
		key();
	}
}
