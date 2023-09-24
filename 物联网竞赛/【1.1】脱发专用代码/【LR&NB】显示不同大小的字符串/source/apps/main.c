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

void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
}

extern unsigned char faceMap1[],faceMap2[],faceMap3[],heartMap1[],heartMap2[];
/*=====================32x32======32x32======32x32======30x32=======17x16===*/
unsigned int hour=12,minute=0,secoud=0,TimeX=24,Count_X=0;
char str[20];

/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init_Inverse();
	Count_X = TimeX+16;
	while(1)
	{
		if(secoud==10)Count_X+=16;
		else if(secoud==100)Count_X+=16;
		else if(secoud==1000)TimeX-=16;
		else if(secoud==10000)
		{
			secoud=0;
			OLED_Clear();
			TimeX=24;
			Count_X = TimeX+16;
		}
		sprintf(str,"%d",secoud);
		OLED_ShowStringSize(TimeX,2,(uint8_t*)str,32);
		OLED_ShowStringSize(Count_X,4,(uint8_t*)"/count",16);
		//DelayMs(1);
		//secoud++;
	}
}
