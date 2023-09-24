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

int count=0;

void mune()
{
	OLED_ShowString(16,0,(uint8_t *)"LED1:");
	OLED_ShowCHinese(64,0,1);
	OLED_ShowCHinese(96,0,2);
	OLED_ShowString(16,4,(uint8_t *)"LED2:");
	OLED_ShowCHinese(64,4,1);
	OLED_ShowCHinese(96,4,2);
}

void update()
{
	if(count==1||count==2)OLED_Clear_Page(2);
	else if(count==3||count==4)OLED_Clear_Page(6);
}

void pointer()
{
	if(isKey2Pressed())
	{
		if(count==0)count=1;
		else 
		{
			update();
			count--;
		}
		if(count==0)count=4;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		if(count==0)count=1;
		else 
		{
			update();
			count++;
		}
		if(count==5)count=1;
		resetKey3();
	}
	if(count==1)OLED_ShowCHinese(64,2,0);
	else if(count==2)OLED_ShowCHinese(96,2,0);
	else if(count==3)OLED_ShowCHinese(64,6,0);
	else if(count==4)OLED_ShowCHinese(96,6,0);
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	mune();
  while( 1 )
  {
		pointer();
		if(isKey4Pressed())
		{
			switch(count)
			{
				case 1:GpioWrite(&Led1,0);break;
				case 2:GpioWrite(&Led1,1);break;
				case 3:GpioWrite(&Led2,0);break;
				case 4:GpioWrite(&Led2,1);break;
			}
			resetKey4();
		}
  }
}
