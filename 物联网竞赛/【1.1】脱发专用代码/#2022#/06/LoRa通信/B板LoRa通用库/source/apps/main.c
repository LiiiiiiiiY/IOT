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
#include "NS_Radio.h"
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
extern unsigned char Lamp1;
extern unsigned char Lamp2;
extern unsigned char Lamp3;


void LED_Light()
{
  OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
}


void LED_str2()
{
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp2);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp3);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
}

void LED_str3()
{
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp2);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp3);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
}

void LED_str23()
{
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp1);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp1);
  OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp2);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp2);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
	OLED_Clear();
	OLED_DrawBMP(16,2,32,32,&Lamp3);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_DrawBMP(80,2,32,32,&Lamp3);
	OLED_ShowString(80,6,(uint8_t *)"LED2");
	HAL_Delay(500);
}



/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	NS_RadioInit(433000000,10,1000,1000);
	uint8_t str[64]={00};
  while(1)
  {
		ReadRadioRxBuffer(str);
		if((str[0]==1)&&(str[1]==1))
		{
		  LED_str23();
		}
		else if(str[0]==1)
		{
			LED_str2();
		}
		else if(str[1]==1)
		{
			LED_str3();
		}
		else if((str[0]==0)&&(str[1])==0)
		{
		  LED_Light();
		}
  }
}
