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
    for (int delay = 0; delay < 1000; delay++) {
        HAL_Delay(10);
       
    }
}
unsigned int count=0;
/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	OLED_ShowString(55,3,(uint8_t *)"12");
  while( 1 )
  {
     if(isKey2Pressed())
		 {
		   count++;
			 OLED_Clear();
			 if(count==3)count=1;
			 resetKey2();
		 }
		 if(count==1)
		 {
			 OLED_ShowString(32,3,(uint8_t *)"LED2");
		   OLED_ShowCHinese(64,3,0);
			 OLED_ShowCHinese(80,3,1);
		   GpioWrite(&Led2,0);
			 HAL_Delay(1000);
			 GpioWrite(&Led2,1);
			 HAL_Delay(1000);
		 }
		 else if(count==2)
		 {
		   GpioWrite(&Led2,1);
			 OLED_ShowString(55,3,(uint8_t *)"12");
		 }
  }
}
