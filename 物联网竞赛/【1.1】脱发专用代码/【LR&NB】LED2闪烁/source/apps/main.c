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


/**
 * Main application entry point.
 */

extern int count;
int main( void )
{
  Init();
	OLED_Init();
	Tim3McuInit(2000);
  while( 1 )
  {
		if(isKey2Pressed())
		{
			count++;
			OLED_Clear();
			resetKey2();
		}
		switch(count%2)
		{
			case 0:
				GpioWrite(&Led2,1);
				OLED_ShowString(0,0,(uint8_t *)"05");
				break;
			
			default:
				OLED_ShowString(0,0,(uint8_t *)"Led2");
				OLED_ShowCHinese(32,0,0);
				OLED_ShowCHinese(48,0,1);
				setTimer3Callback(Time3Handler);
				break;
		}
  }
}
