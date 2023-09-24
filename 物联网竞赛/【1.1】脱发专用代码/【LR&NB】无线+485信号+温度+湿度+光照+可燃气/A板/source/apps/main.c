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
void Init() {
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//������ʼ��
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��
}

float light,temp,hum,air;
int light0,light1;
uint8_t Send[255],recv[255];

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
  while(1)
  {
		AdcScanChannel();
		call_sht11(&temp,&hum);
		light = pow(10,((1.78-log10(33/AdcReadCh0()-10))/0.6));
		int len = ReadRadioRxBuffer(recv);
		if(len>0)
		{
			air = recv[0]*10+recv[1];
		}
		char str[10];
		sprintf(str,"Lig:%5d",(int)light);
		OLED_ShowString(5,0,(uint8_t *)str);
		sprintf(str,"Tem:%5d",(int)temp);
		OLED_ShowString(5,2,(uint8_t *)str);
		sprintf(str,"Hum:%5d",(int)hum);
		OLED_ShowString(5,4,(uint8_t *)str);
		sprintf(str,"Air:%5d",(int)air);
		OLED_ShowString(5,6,(uint8_t *)str);
		
		light0 = (int)light/200;
		light1 = (int)light%200;
		Send[5] = light0;
		Send[6] = light1;
		Send[7] = (int)temp;
		Send[8] = (int)hum;
		Radio.Send(Send,9);
  }
}
