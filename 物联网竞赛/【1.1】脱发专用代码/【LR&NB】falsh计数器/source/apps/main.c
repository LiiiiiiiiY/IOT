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
unsigned int N,Y;
void arithmetic()
{
	if(isKey2Pressed())
	{
		if(N==0);
		else N--;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		N++;
		resetKey3();
	}
	else if(isKey4Pressed())
	{
		Y = N;
		OLED_ShowNum(64,6,Y,3,16);
		resetKey4();
		WriteFlashTest(0x0800f300,N);
		GpioWrite(&Led2,0);
		HAL_Delay(1000);
		GpioWrite(&Led2,1);
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	GpioWrite(&Led2,1);
	OLED_ShowCHinese(40,0,0);
	OLED_ShowCHinese(56,0,1);
	OLED_ShowCHinese(72,0,2);
	OLED_ShowCHinese(16,3,3);
	OLED_ShowCHinese(32,3,4);
	OLED_ShowCHinese(48,3,5);
	OLED_ShowCHinese(16,6,6);
	OLED_ShowCHinese(32,6,7);
	OLED_ShowCHinese(48,6,8);
	N = PrintFlashTest(0x0800f300);
  while(1)
  {
		arithmetic();
		OLED_ShowNum(64,3,N,3,16);
		OLED_ShowNum(64,6,Y,3,16);
  }
}
