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
#include "sht3x.h"
#include "adc_reader.h"
#include "math.h"
#include "NS_Radio.h"
extern uint16_t USART1_RX_COUNT;
void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
    setTimer2Callback(Time2Handler);
}
uint8_t URecv[255],USend[255],Light[2]={0xaa,0xa1};
uint8_t Red[9]={0xa5,0x06,0xff,0xa0,0xff,0x00,0x00,0xee,0x5a},Green[9]={0xa5,0x06,0xff,0xa0,0x00,0xff,0x00,0xee,0x5a},Blue[9]={0xa5,0x06,0xff,0xa0,0x00,0x00,0xff,0xee,0x5a};
float light=0,temp=0,humi=0;

void GetData()//获取数据并显示在LED屏上
{
	char str[10];
	AdcScanChannel();
	call_sht11(&temp,&humi);
	light = (uint16_t)(5/2.0)*(AdcReadCh0()*100);
	if(light>500)
	{
		light = pow(10,((1.78-log10(33/AdcReadCh0()-10))/0.6));
	}
	sprintf(str,"Temp:%.2f    ",temp);
	OLED_ShowString(2,1,(uint8_t*)str);
	sprintf(str,"Humi:%.2f    ",humi);
	OLED_ShowString(2,3,(uint8_t*)str);
	sprintf(str,"Light:%.2f   ",light);
	OLED_ShowString(2,5,(uint8_t*)str);
}

void bright()//光照低于200亮灯的逻辑控制
{
	if(light<200)
	{
		GpioWrite(&Led1,0);
		GpioWrite(&Led2,0);
		Light[1]=0xa0;
		SX1276Send(Light,2);
	}
	else if(light>=200)
	{
		GpioWrite(&Led1,1);
		GpioWrite(&Led2,1);
		Light[1]=0xa1;
		SX1276Send(Light,2);
	}
}

void SendData()//将数据拆分发给移动端
{
	temp=temp*100;
	humi=humi*100;
	light=light*100;
	USend[0] = 11;
	USend[1] = ((int)temp/100);
	USend[2] = ((int)temp%100);	
	USend[3] = ((int)humi/100);
	USend[4] = ((int)humi%100);
	USend[5] = ((int)light/1000000);
	USend[6] = (((int)light%1000000)/10000);
	USend[7] = (((((int)light%1000000)%10000))/100);
	USend[8] = (((((int)light%1000000)%10000))%100);
	USend[9] = 22;
	USART1_SendStr(USend,10);
}

/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	ADCS_Init();
	hal_temHumInit();
	USART1_Init(9600);
	OLED_Init_Inverse();
	NS_RadioInit(433000000,20,1000,1000);
	while( 1 )
	{
		GetData();
		bright();
		SendData();
		DelayMs(1);
		if(isKey2Pressed())
		{
			SX1276Send(Red,9);
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			SX1276Send(Green,9);
			resetKey3();
		}
		else if(isKey4Pressed())
		{
			SX1276Send(Blue,9);
			resetKey4();
		}
		int len = USART1_ReadRxBuffer(URecv);
		DelayMs(1);
		if(len>0)
		{
			if((URecv[0]==0xa5)&&(URecv[8]==0x5a))
			{
				USART1_SendStr(URecv,len);
				SX1276Send(URecv,9);
			}
			if(URecv[0]==0xb5)
			{
				USART1_SendStr(URecv,len);
				SX1276Send(URecv,2);
			}
			USART1_ReceiveClr();
		}
	}
}
