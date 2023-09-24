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

#define KEY2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)

/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init_Inverse();
	while(1)
	{
		if(KEY2==0)
		{
			DelayMs(50);
			if(KEY2==0)
			{
				while(!KEY2)
				{
					GpioWrite(&Led1,0);
				}
				GpioWrite(&Led1,1);
			}
		}
	}
}
