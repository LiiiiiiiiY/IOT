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
#include "adc_reader.h"
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

float voltage;
int light;
/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);
	ADCS_Init();
	GpioWrite(&Led1,0);
  while(1)
  {
		AdcScanChannel();
		light = AdcReadCh0();
		//light = voltage;//(uint16_t)(5/2.0)*(voltage*100);
		char strBuf[64]={0};
		sprintf(strBuf ,"%d Lx  ",light);
		OLED_ShowString(64,0,(uint8_t *)strBuf);
		if(light<50)GpioWrite(&Led2,0);
		else GpioWrite(&Led2,1);
  }
}
