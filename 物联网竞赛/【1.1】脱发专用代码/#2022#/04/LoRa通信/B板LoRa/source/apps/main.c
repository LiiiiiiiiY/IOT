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

/********************************************************
*������uint8 CheckSum
*********************************************************/
uint8_t ChekSum(uint8_t *buf,uint8_t len)
{
  uint8_t temp=0;
  while(len--)
	{
	  temp += *buf;
		buf++;
	}
	return (uint8_t)temp;
}
int flag=0;
uint8_t recv[4];
extern unsigned char WIFI[];
float light[1]={0};
float temp[1]={0};
float hum[1]={0};

void Celect_Menu()
{
	OLED_ShowString(48,0,(uint8_t *)"Celect Menu");
	OLED_DrawBMP(80,4,16,16,WIFI);
}

void Auto_Lamp()
{
	light[0] = recv[1];
	temp[0]  = recv[2];
	hum[0]   = recv[3];
	OLED_ShowString(48,0,(uint8_t *)"Auto Lamp");
	OLED_ShowString(0,2,(uint8_t *)"Light:");
	OLED_ShowString(0,4,(uint8_t *)"Temp: ");
	OLED_ShowString(0,6,(uint8_t *)"Humi: ");
	OLED_ShowString(48,2,(uint8_t *)light);
	OLED_ShowString(48,2,(uint8_t *)temp);
	OLED_ShowString(48,2,(uint8_t *)hum);
}

void Mala_Lamp()
{
  OLED_ShowString(48,0,(uint8_t *)"Mala Lamp");
	OLED_ShowString(48,0,(uint8_t *)"Lamp:");
}

void View_Freq()
{
  OLED_ShowString(48,0,(uint8_t *)"View Freq");
	OLED_ShowString(48,0,(uint8_t *)"Freq:433000000");
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	NS_RadioInit(433000000,10,1000,1000);
  while(1)
  {
    ReadRadioRxBuffer(recv);
		switch(recv[0])
		{
			case 0:OLED_Clear();flag=0;break;
			case 1:OLED_Clear();flag=1;break;
			case 2:OLED_Clear();flag=2;break;
			case 3:OLED_Clear();flag=3;break;
		}
		switch(flag)
		{
			case 0:Celect_Menu();break;
			case 1:Auto_Lamp();break;
			case 2:Mala_Lamp();break;
			case 3:View_Freq();break;
		}
  }
}
