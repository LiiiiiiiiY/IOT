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

unsigned int count=0;


void OLED_Begin()
{
  OLED_ShowString(2,0,(uint8_t *)"LED1:");
	OLED_ShowCHinese(48,0,0);//LED1 开
  OLED_ShowCHinese(80,0,1);//LED1 关
		
  OLED_ShowString(2,4,(uint8_t *)"LED2:");
  OLED_ShowCHinese(48,4,0);//LED2 开
	OLED_ShowCHinese(80,4,1);//LED2 关
}

void Hand()
{
  if(isKey2Pressed())
	{
		OLED_Clear();
		count++;
		if(count==5)count=1;
		
	  resetKey2();
	}
	if(isKey3Pressed())
	{
		OLED_Clear();
		if(count==0)count=2;
		count--;
		if(count==0)count=4;
	  resetKey3();
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	NS_RadioInit(433000000,10,1000,1000);
	uint8_t str[64]={00};
  while( 1 )
  {
    OLED_Begin();
		Hand();
		switch(count)
    {
		case 1:OLED_ShowCHinese(48,2,2);break;//LED1 开
		case 2:OLED_ShowCHinese(80,6,2);break;//LED2 关
		case 3:OLED_ShowCHinese(48,6,2);break;//LED2 开
		case 4:OLED_ShowCHinese(80,2,2);break;//LED1 关
    }
		if(isKey4Pressed())
	  {
	    switch(count)
      {
		  case 1:GpioWrite( &Led1, 0 );str[0]=1;break;//LED1 开
		
		  case 2:GpioWrite( &Led2, 1 );str[1]=0;break;//LED2 关
		
      case 3:GpioWrite( &Led2, 0 );str[1]=1;break;//LED2 开
			
	    case 4:GpioWrite( &Led1, 1 );str[0]=0;break;//LED1 关
      }
		  resetKey4();
	  }
		Radio.Send(str,3);
  }
}
