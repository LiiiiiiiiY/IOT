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
#include "sht3x.h"
#include "NS_Radio.h"
#include "math.h"
#include "led_light.h"
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
	 
	Tim3McuInit(4000);
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
int main( void )
{
	Init();
	NS_RadioInit(433333000,20,1000,1000);
	OLED_Init();
	setBreathLedArg(128,100);
	stopLedBreath();
	char str[10];
	extern unsigned char light1[],light2[],light3[];
	extern int interfaceFlag,Tcount;
	uint8_t count=0,len=0,light,temp,hum;
	uint8_t Recv[255];
	while(1)
	{
		len = ReadRadioRxBuffer(Recv);
		if(len==1)
		{
			OLED_Clear();
			if(Recv[0]==0x01)interfaceFlag=1;
			else if(Recv[0]==0x02)interfaceFlag=2;
			else if(Recv[0]==0x03)interfaceFlag=3;
			else if(Recv[0]==0x04)interfaceFlag=4;
			else if(Recv[0]==0x05)interfaceFlag=5;
			else if(Recv[0]==0x06)interfaceFlag=6;
			else if(Recv[0]==0x07)interfaceFlag=7;
		}
		else if(interfaceFlag==1)
		{
			int lightSH,lightH,lightL;
			lightSH = Recv[1]<<8|Recv[2];
			sprintf(str,"%dLx   ",lightSH);
			OLED_ShowString(48,4,(uint8_t *)str);
		}
		else if(interfaceFlag==2)
		{
			temp=Recv[1];
			sprintf(str,"%d",temp);
			OLED_ShowString(48,4,(uint8_t *)str);
			OLED_ShowCHinese(80,4,13);
		}
		else if(interfaceFlag==3)
		{
			hum=Recv[1];
			sprintf(str,"%d",hum);
			OLED_ShowString(48,4,(uint8_t *)str);
			OLED_ShowCHinese(80,4,14);
			OLED_ShowString(96,4,(uint8_t *)"RH");
		}
		else if(interfaceFlag==4)
		{
			OLED_DrawBMP(32,4,32,32,light3);
			GpioWrite(&Led1,0);
			GpioWrite(&Led2,1);
		}
		else if(interfaceFlag==5)
		{
			OLED_DrawBMP(32,4,32,32,light1);
			GpioWrite(&Led1,1);
			GpioWrite(&Led2,1);
		}
		else if(interfaceFlag==6)
		{
			setTimer3Callback(Time3Handler);
			switchLed2Breath();
			breathLed();
		}
		else if(interfaceFlag==7)
		{
			setTimer3Callback(Time3Handler);
		}
	}
}
