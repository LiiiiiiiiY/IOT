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
#include "sht3x.h"
#include "adc_reader.h"
#include "NS_Radio.h"
#include "math.h"
#include "usart1-board.h"
void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
}

int light,light0,light1,temp,hum;
float Air;
uint8_t send[255],recv[255],Usend[255];

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init_Inverse();
	hal_temHumInit();
	ADCS_Init();
	NS_RadioInit(450000000,10,1000,1000);
	USART1_Init(9600);
  while(1)
  {
		int len = ReadRadioRxBuffer(recv);
		if(len>0)
		{
			GpioWrite(&Led1,0);
			HAL_Delay(1);
			light0 = recv[5];
			light1 = recv[6];
			light = (light0*200) + light1;
			temp = recv[7];
			hum = recv[8];
			
//			Usend[0] = (light-0x30)/0x10;
//			Usend[1] = (light-0x30)%0x10;
//			Usend[2] = temp-0x30;
//			Usend[3] = hum-0x30;
			for(int i=0;i<4;i++)
			{
				Usend[i] = recv[5+i];
			}
			AdcScanChannel();
			Air = pow(10,((0.2552-log10((5/AdcReadCh0()-1)/10))/0.5829));
			Usend[4] = (int)Air/10;
			Usend[5] = (int)Air%10;
			send[0] = (int)Air/10;
			send[1] = (int)Air%10;
			Radio.Send(send,2);
			USART1_SendStr(Usend,6);
			USART1_ReceiveClr();
			GpioWrite(&Led1,1);
			
		}
		char str[10];
		sprintf(str,"Lit:%5d",light);
		OLED_ShowString(0,0,(uint8_t *)str);
		sprintf(str,"Tem:%5d",(int)temp);
		OLED_ShowString(0,2,(uint8_t *)str);
		sprintf(str,"Hum:%5d",(int)hum);
		OLED_ShowString(0,4,(uint8_t *)str);
		sprintf(str,"Air:%5d",(int)Air);
		OLED_ShowString(0,6,(uint8_t *)str);
  }
}
