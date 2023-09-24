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


/**
 * Main application entry point.
 */

extern int count;
int main( void )
{
  Init();
	OLED_Init();
	Tim3McuInit(2000);
  while( 1 )
  {
		if(isKey2Pressed())
		{
			count++;
			OLED_Clear();
			resetKey2();
		}
		switch(count%2)
		{
			case 0:
				GpioWrite(&Led2,1);
				OLED_ShowString(0,0,(uint8_t *)"05");
				break;
			
			default:
				OLED_ShowString(0,0,(uint8_t *)"Led2");
				OLED_ShowCHinese(32,0,0);
				OLED_ShowCHinese(48,0,1);
				setTimer3Callback(Time3Handler);
				break;
		}
  }
}
