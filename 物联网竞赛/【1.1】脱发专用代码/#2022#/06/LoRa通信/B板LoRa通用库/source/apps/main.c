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
extern unsigned char Lamp1;
extern unsigned char Lamp2;
extern unsigned char Lamp3;


void LED_Light()
{
  OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
}


void LED_str2()
{
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp2);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp3);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
}

void LED_str3()
{
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp2);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp3);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
}

void LED_str23()
{
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
  OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp2);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp2);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp3);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp3);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
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
  while(1)
  {
		ReadRadioRxBuffer(str);
		if((str[0]==1)&&(str[1]==1))
		{
		  LED_str23();
		}
		else if(str[0]==1)
		{
			LED_str2();
		}
		else if(str[1]==1)
		{
			LED_str3();
		}
		else if((str[0]==0)&&(str[1])==0)
		{
		  LED_Light();
		}
  }
}
