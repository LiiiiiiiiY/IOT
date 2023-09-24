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
/**********************************************************************************************
*������void Init( void )
*���ܣ�ƽ̨��ʼ��
*���룺��
*�������
*����˵������
**********************************************************************************************/
void Init() {
    // ������ƽ̨��ʼ��
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//������ʼ��
    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//��ʱ����ʼ�������ö�ʱ�ж�1ms�ж�һ��
	 
}

/**********************************************************************************************
*������void KeyDownHandler( void )
*���ܣ���ť�¼�����
*���룺��
*�������
*����˵������
**********************************************************************************************/
void KeyDownHandler(void) {
	
}

/**********************************************************************************************
*������void handlerPre10Ms( void )
*���ܣ�10����ѭ��
*���룺��
*�������
*����˵����ѭ��������ʱ��300ms
**********************************************************************************************/
void handlerPre10Ms(void) {
    for (int delay = 0; delay < 30; delay++) {
        HAL_Delay(10);
        
    }
}

float voltage;
int light;
/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);
	ADCS_Init();
	GpioWrite(&Led1,0);
  while(1)
  {
		AdcScanChannel();
		light = AdcReadCh0();
		//light = voltage;//(uint16_t)(5/2.0)*(voltage*100);
		char strBuf[64]={0};
		sprintf(strBuf ,"%d Lx  ",light);
		OLED_ShowString(64,0,(uint8_t *)strBuf);
		if(light<50)GpioWrite(&Led2,0);
		else GpioWrite(&Led2,1);
  }
}
