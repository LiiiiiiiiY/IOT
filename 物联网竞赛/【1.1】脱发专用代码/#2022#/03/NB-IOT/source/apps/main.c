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
#include "oledfont.h"
#include "hal_oled.h"

uint16_t tenMSecClick=0;																		// 10毫秒计时器
int i=1;
int j;
char buffer[2]={0};
char buffer2[2]={0};


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
    for (int delay = 0; delay < 100; delay++) {
        HAL_Delay(10);  
    }
}

void OLED(){
  OLED_ShowCHinese(40,0,0);
	OLED_ShowCHinese(56,0,1);
	OLED_ShowCHinese(72,0,2);
	
	OLED_ShowCHinese(30,3,3);
	OLED_ShowCHinese(46,3,4);
	OLED_ShowString (62,3,(uint8_t *)":");	
	
	OLED_ShowCHinese(30,6,5);
	OLED_ShowCHinese(46,6,6);
	OLED_ShowString (62,6,(uint8_t *)":");
}

void KeyScan(){
  if(isKey2Pressed()){
  	j--;resetKey2();
	}
	if(isKey3Pressed()){
		j++;resetKey3();
	}
  if(isKey4Pressed()){
	  GpioWrite(&Led2,0);
	 	HAL_Delay(1000);
	  GpioWrite(&Led2,1);
	 	resetKey4();
  }
}


int main( void )
{
  Init();
	OLED_Init();
	OLED();
	//memset(buffer,'0',64);//申请内存
	sprintf(buffer,"%d",i);
	OLED_ShowString (70,3,(uint8_t *)buffer);
	j=i;
  while(1)
  {
		KeyScan();
	//	memset(buffer,'0',64);//申请内存
		sprintf(buffer2,"%d",j);
		OLED_ShowString(70,6,(uint8_t *)buffer2);
  }
}
