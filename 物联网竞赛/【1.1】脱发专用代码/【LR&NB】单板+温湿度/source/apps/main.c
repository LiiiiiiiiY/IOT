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
#include "sht3x.h"

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

int interface_flag=0;


void keyScan()
{
	if(isKey2Pressed())
	{
		interface_flag=1;
		OLED_Clear();
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		interface_flag=2;
		OLED_Clear();
		resetKey3();
	}
	else if(isKey4Pressed())
	{
		interface_flag=0;
		OLED_Clear();
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
	hal_temHumInit();
	float temp,hum;
  while(1)
  {
		char data_buf[16];
		call_sht11(&temp,&hum);
		
		keyScan();
		if(interface_flag==0)
		{
			OLED_ShowCHinese(0,0,0);
			OLED_ShowCHinese(16,0,1);
			OLED_ShowCHinese(32,0,2);
			OLED_ShowCHinese(48,0,3);
			OLED_ShowCHinese(64,0,4);
			OLED_ShowString(80,0,(uint8_t *)":");
			
			sprintf(data_buf,"%.0f",temp);
			OLED_ShowString(10,4,(uint8_t *)data_buf);
			OLED_ShowCHinese(32,4,5);
			sprintf(data_buf," , %.0f",hum);
			OLED_ShowString(48,4,(uint8_t *)data_buf);
			OLED_ShowCHinese(96,4,6);
		}
		else if(interface_flag==1)
		{
			OLED_ShowCHinese(0,0,0);
			OLED_ShowCHinese(16,0,1);
			OLED_ShowCHinese(32,0,2);
			OLED_ShowCHinese(48,0,4);
			OLED_ShowString(64,0,(uint8_t *)":");
			sprintf(data_buf,"%.0f",temp);
			OLED_ShowString(10,4,(uint8_t *)data_buf);
			OLED_ShowCHinese(32,4,5);
		}
		else if(interface_flag==2)
		{
			OLED_ShowCHinese(0,0,0);
			OLED_ShowCHinese(16,0,1);
			OLED_ShowCHinese(32,0,3);
			OLED_ShowCHinese(48,0,4);
			OLED_ShowString(64,0,(uint8_t *)":");
			sprintf(data_buf,"%.0f",hum);
			OLED_ShowString(10,4,(uint8_t *)data_buf);
			OLED_ShowCHinese(32,4,6);
		}
		HAL_Delay(50);
	}
}
