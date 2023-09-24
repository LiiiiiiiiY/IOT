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
#include "flash.h"
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
unsigned int N,Y;
void arithmetic()
{
	if(isKey2Pressed())
	{
		if(N==0);
		else N--;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		N++;
		resetKey3();
	}
	else if(isKey4Pressed())
	{
		Y = N;
		OLED_ShowNum(64,6,Y,3,16);
		resetKey4();
		WriteFlashTest(0x0800f300,N);
		GpioWrite(&Led2,0);
		HAL_Delay(1000);
		GpioWrite(&Led2,1);
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	GpioWrite(&Led2,1);
	OLED_ShowCHinese(40,0,0);
	OLED_ShowCHinese(56,0,1);
	OLED_ShowCHinese(72,0,2);
	OLED_ShowCHinese(16,3,3);
	OLED_ShowCHinese(32,3,4);
	OLED_ShowCHinese(48,3,5);
	OLED_ShowCHinese(16,6,6);
	OLED_ShowCHinese(32,6,7);
	OLED_ShowCHinese(48,6,8);
	N = PrintFlashTest(0x0800f300);
  while(1)
  {
		arithmetic();
		OLED_ShowNum(64,3,N,3,16);
		OLED_ShowNum(64,6,Y,3,16);
  }
}
