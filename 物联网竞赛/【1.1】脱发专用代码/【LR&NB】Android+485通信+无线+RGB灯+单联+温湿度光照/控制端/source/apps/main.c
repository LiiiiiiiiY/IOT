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
#include "usart1-board.h"
#include "NS_Radio.h"

Gpio_t Relay1;
#define Relay_1 PB_2

void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
}
uint8_t Radio_Recv[255],flag=0,Red=0x00,Blue=0x00,Green=0x00;
uint8_t Black[9]={0xa5,0x06,0xff,0xa0,0x00,0x00,0x00,0xee,0x5a};
void RGB()
{
	char str[20];
	sprintf(str,"R: %3d",Red);
	OLED_ShowString(0,0,(uint8_t*)str);
	sprintf(str,"G: %3d",Green);
	OLED_ShowString(0,2,(uint8_t*)str);
	sprintf(str,"B: %3d",Blue);
	OLED_ShowString(0,4,(uint8_t*)str);
	int Relay= GpioRead(&Relay1);
	if(Relay==0)
	{
		OLED_ShowString(0,6,(uint8_t*)"Fan: Off");
	}
	else if(Relay==1)
	{
		OLED_ShowString(0,6,(uint8_t*)"Fan: On ");
	}
}

void NS_Radio_Recv()
{
	int len = ReadRadioRxBuffer(Radio_Recv);
	if(len>0)
	{
		if(Radio_Recv[0]==0xaa)
		{
			if(Radio_Recv[1]==0xa0)
			{
				GpioWrite(&Led1,0);
				GpioWrite(&Led2,0);
			}
			else
			{
				GpioWrite(&Led1,1);
				GpioWrite(&Led2,1);
			}
		}
		else if(Radio_Recv[0]==0xa5&&Radio_Recv[8]==0x5a)
		{
			Red = Radio_Recv[4];
			Green = Radio_Recv[5];
			Blue = Radio_Recv[6];
			USART1_SendStr(Radio_Recv,9);
		}
		else if(Radio_Recv[0]==0xb5)
		{
			if(Radio_Recv[1]==0x01)
			{
				GpioWrite(&Relay1,1);
			}
			else
			{
				GpioWrite(&Relay1,0);
			}
		}
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	USART1_Init(9600);
	OLED_Init_Inverse();
	USART1_SendStr(Black,9);
	NS_RadioInit(433000000,20,1000,1000);
	//uint8_t Radio_Recv[255],flag=0,Red=0x00,Blue=0x00,Green=0x00;
	GpioInit(&Relay1,Relay_1,PIN_OUTPUT,PIN_PUSH_PULL,PIN_NO_PULL,0);
	while( 1 )
	{
		RGB();
		NS_Radio_Recv();
		if(isKey2Pressed())
		{
			GpioWrite(&Relay1,1);	//打开继电器1
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			GpioWrite(&Relay1,0);	//关闭继电器1
			resetKey3();
		}
		
	}
}
