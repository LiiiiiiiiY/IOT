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

void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
		setTimer3Callback(Time3Handler);
    Tim3McuInit(4000);
}
uint8_t Count=0,Hreat_count=0;
float light,Time=0;
char string[10];
uint16_t T3Time;
extern unsigned char Smile[],Boring[],Crying[],Heartbeat_Big[],Heartbeat_Small[];
extern uint8_t hour,min,sec,Flag;

void Face(uint8_t count)
{
	switch(Count)
	{
		case 0:OLED_DrawBMP(48,2,32,32,Smile);break;
		case 1:OLED_DrawBMP(48,2,32,32,Boring);break;
		case 2:OLED_DrawBMP(48,2,32,32,Crying);break;
	}
}

void Hreat(uint8_t time)
{
	switch(Hreat_count)
	{
		case 0:
			OLED_DrawBMP(49,2,30,32,Heartbeat_Big);
			Delay(time);
		break;
			
		case 1:
			OLED_DrawBMP(49,2,30,32,Heartbeat_Small);
			Delay(time);
		break;
	}
	Hreat_count^=1;
}
/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init_Inverse();
	ADCS_Init();
	OLED_DrawBMP(48,2,32,32,Smile);
	OLED_ShowCHinese(48,6,0);
	OLED_ShowCHinese(64,6,1);
	while( 1 )
	{
		if(Flag==0)
		{
			if(isKey2Pressed())
			{
				Count++;
				if(Count>=3)Count=0;
				Face(Count);
				resetKey2();
			}
			else if(isKey3Pressed())
			{
				Flag=1;
				OLED_Clear();
				OLED_ShowCHinese(24,6,2);
				OLED_ShowCHinese(40,6,3);
				resetKey3();
			}
		}
		else if(Flag==1)
		{
			AdcScanChannel();
			light = pow(10, ((1.78-log10(33/AdcReadCh0()-10)) / 0.6));
			if(light<200)light = (5/2.0)*(100*AdcReadCh0());
			sprintf(string,":%.0flux  ",light);
			OLED_ShowStringSize(56,6,16,(uint8_t*)string);
			if(light<100)Time = 100;
			else Time=500;
			Hreat(Time);
			if(isKey2Pressed())
			{
				OLED_Clear();
				OLED_ShowCHinese(48,6,0);
				OLED_ShowCHinese(64,6,1);
				OLED_DrawBMP(48,2,32,32,Smile);
				Flag=0;
				Face(Count);
				Tim3McuInit(T3Time);//定时器初始化，设置定时中断1ms中断一次
				resetKey2();
			}
		}
	}
}
