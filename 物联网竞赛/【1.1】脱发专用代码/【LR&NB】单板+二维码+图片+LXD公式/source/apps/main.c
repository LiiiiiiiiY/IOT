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
extern unsigned char bitmap5oled[],bitmap4oled[];
/**
 * Main application entry point.
 */

void zhu()
{
	OLED_ShowCHinese(42,0,0);
	OLED_ShowCHinese(58,0,1);
	OLED_ShowCHinese(74,0,2);
	OLED_ShowString(0,2,(uint8_t *)"K2.Formula");
	OLED_ShowString(0,4,(uint8_t *)"K3.WIFI");
	OLED_ShowString(0,6,(uint8_t *)"K4.QRCode");
}

void k2()
{
	OLED_ShowString(32,0,(uint8_t *)"Formula");
	AdcScanChannel();
	char str[10],value;
	value = AdcReadCh0();
	sprintf(str,"L %d D",value);
	OLED_ShowString(32,3,(uint8_t *)str);
}

void k3()
{
	OLED_ShowString(32,0,(uint8_t *)"WIFI");
	OLED_DrawBMP(32,2,55,40,bitmap4oled);
}

void k4()
{
	OLED_ShowString(32,0,(uint8_t *)"QRCode");
	OLED_DrawBMP(32,2,32,32,bitmap5oled);
}
int main( void )
{
	Init();
	OLED_Init();
	ADCS_Init();
	zhu();
	while(1)
	{
		if(isKey2Pressed())
		{
			OLED_Clear();
			GpioWrite(&Led1,0);
			GpioWrite(&Led2,1);
			k2();
			resetKey2();
		}
		if(isKey3Pressed())
		{
			OLED_Clear();
			GpioWrite(&Led1,1);
			GpioWrite(&Led2,0);
			k3();
			resetKey3();
		}
		if(isKey4Pressed())
		{
			OLED_Clear();
			GpioWrite(&Led1,0);
			GpioWrite(&Led2,0);
			k4();
			resetKey4();
		}
		
	}
}
