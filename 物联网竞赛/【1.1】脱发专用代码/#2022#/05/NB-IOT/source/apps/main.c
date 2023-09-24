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
    for (int delay = 0; delay < 1000; delay++) {
        HAL_Delay(10);
       
    }
}
unsigned int count=0;
/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	OLED_ShowString(55,3,(uint8_t *)"12");
  while( 1 )
  {
     if(isKey2Pressed())
		 {
		   count++;
			 OLED_Clear();
			 if(count==3)count=1;
			 resetKey2();
		 }
		 if(count==1)
		 {
			 OLED_ShowString(32,3,(uint8_t *)"LED2");
		   OLED_ShowCHinese(64,3,0);
			 OLED_ShowCHinese(80,3,1);
		   GpioWrite(&Led2,0);
			 HAL_Delay(1000);
			 GpioWrite(&Led2,1);
			 HAL_Delay(1000);
		 }
		 else if(count==2)
		 {
		   GpioWrite(&Led2,1);
			 OLED_ShowString(55,3,(uint8_t *)"12");
		 }
  }
}
