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

uint16_t tenMSecClick=0;																		// 10�����ʱ��

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

void Oled_init()
{
  OLED_ShowString(16,0,(uint8_t *)"LED1:");
	OLED_ShowCHinese(60,0,0);//LED1  ��
	OLED_ShowCHinese(92,0,1);//LED1  ��
	
	
	OLED_ShowString(16,4,(uint8_t *)"LED2:");
	OLED_ShowCHinese(60,4,0);//LED2  ��
	OLED_ShowCHinese(92,4,1);//LED2  ��
}

unsigned int count;
unsigned int flag;

void Hand23()
{
  if(isKey2Pressed())
	{
		count++;OLED_Clear();
		if(count==5)count=1;
	  resetKey2();
	}
	if(isKey3Pressed())
	{
		if(count==0)
		{
			count=1;
			OLED_Clear();
		}
		else
		{
	    count--;
			OLED_Clear();
		  if(count==0)count=4;
		}
	  resetKey3();
	}
}

void Hand4()
{
  if(isKey4Pressed())
	{
	  flag=count;
	  resetKey4();
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	Oled_init();
  while( 1 )
  {
    Hand23();
		switch(count)
	  {
		case 1:
			Oled_init();
			OLED_ShowCHinese(60,2,2);
		break;
		
		case 2:
			Oled_init();
		  OLED_ShowCHinese(92,6,2);
			break;
		
		case 3:
			Oled_init();
		  OLED_ShowCHinese(60,6,2);
			break;
		
		case 4:
			Oled_init();
		  OLED_ShowCHinese(92,2,2);
			break;
	}
		Hand4();
	switch(flag)
		{
			case 1:GpioWrite( &Led1, 0 );break;
			case 2:GpioWrite( &Led2, 1 );break;
			case 3:GpioWrite( &Led2, 0 );break;
			case 4:GpioWrite( &Led1, 1 );break;
		}
  }
}
