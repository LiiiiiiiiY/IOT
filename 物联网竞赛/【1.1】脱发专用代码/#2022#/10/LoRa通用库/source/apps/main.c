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

unsigned int flag=0;
extern unsigned char WIFI[];
extern unsigned char RQCode[];

void menu()
{
  OLED_ShowString(2,0,(uint8_t *)"K2.Formula");
	OLED_ShowString(2,3,(uint8_t *)"K3.WIFI");
	OLED_ShowString(2,6,(uint8_t *)"K4.RQCode");
}

void else_menu()
{
	if(isKey2Pressed())
	{
		OLED_Clear();
		flag=2;
		resetKey2();
  }
	if(isKey3Pressed())
	{
		OLED_Clear();
    flag=3;
		resetKey3();
  }
	if(isKey4Pressed())
	{
		OLED_Clear();
    flag=4;
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
	menu();
  while( 1 )
  {
    else_menu();
		if(flag==2)
		{
			GpioWrite( &Led1, 0 );GpioWrite( &Led2, 1 );
		  OLED_ShowString(38,0,(uint8_t *)"Formula");
			OLED_ShowString(2,4,(uint8_t *)"F: ************");
		}
		else if(flag==3)
		{
			GpioWrite( &Led1, 1 );GpioWrite( &Led2, 0 );
		  OLED_ShowString(52,0,(uint8_t *)"WIFI");
			OLED_DrawBMP(42,2,48,48,WIFI);
		}
		else if(flag==4)
		{
			GpioWrite( &Led1, 0 );GpioWrite( &Led2, 0 );
		  OLED_ShowString(40,0,(uint8_t *)"RQCode");
			OLED_DrawBMP(40,2,48,48,RQCode);
		}
  }
}
