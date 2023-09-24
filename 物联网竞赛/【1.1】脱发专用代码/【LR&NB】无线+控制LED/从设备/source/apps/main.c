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
extern unsigned char light1[];
extern unsigned char light2[];
extern unsigned char light3[];

void light_Led1()
{
	OLED_Clear_Page(2);
	OLED_Clear_Page(4);
	switch(count)
	{
		case 1:OLED_DrawBMP(16,2,32,32,light2);OLED_DrawBMP(80,2,32,32,light1);break;
		case 2:OLED_DrawBMP(16,2,32,32,light3);OLED_DrawBMP(80,2,32,32,light1);break;
		case 3:OLED_DrawBMP(16,2,32,32,light2);OLED_DrawBMP(80,2,32,32,light1);break;
		case 4:OLED_DrawBMP(16,2,32,32,light1);OLED_DrawBMP(80,2,32,32,light1);break;
	}
	count++;
	if(count==5)count=1;
}

void light_Led2()
{
	OLED_Clear_Page(2);
	OLED_Clear_Page(4);
	switch(count)
	{
		case 1:OLED_DrawBMP(80,2,32,32,light2);OLED_DrawBMP(16,2,32,32,light1);break;
		case 2:OLED_DrawBMP(80,2,32,32,light3);OLED_DrawBMP(16,2,32,32,light1);break;
		case 3:OLED_DrawBMP(80,2,32,32,light2);OLED_DrawBMP(16,2,32,32,light1);break;
		case 4:OLED_DrawBMP(80,2,32,32,light1);OLED_DrawBMP(16,2,32,32,light1);break;
	}
	count++;
	if(count==5)count=1;
}

void light_both()
{
	OLED_Clear_Page(2);
	OLED_Clear_Page(4);
	switch(count)
	{
		case 1:OLED_DrawBMP(16,2,32,32,light2);OLED_DrawBMP(80,2,32,32,light2);break;
		case 2:OLED_DrawBMP(16,2,32,32,light3);OLED_DrawBMP(80,2,32,32,light3);break;
		case 3:OLED_DrawBMP(16,2,32,32,light2);OLED_DrawBMP(80,2,32,32,light2);break;
		case 4:OLED_DrawBMP(16,2,32,32,light1);OLED_DrawBMP(80,2,32,32,light1);break;
	}
	count++;
	if(count==5)count=1;
}

/**
 * Main application entry point.
 */
int main( void )
{
	uint8_t recv[2]={0,4};
	int recvrecv[2]={0,4};
	int len;
  Init();
	OLED_Init();
	NS_RadioInit(433000000,20,1000,1111);
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_ShowString(80,6,(uint8_t *)"LED2");
  while(1)
  {
		len = ReadRadioRxBuffer(recv);
		if(len>0)
		{
			recvrecv[0]=recv[0];
			recvrecv[1]=recv[1];
		}
		if(recvrecv[0]==1&&recvrecv[1]==3)
		{
			light_both();
			HAL_Delay(1000);
		}
		else if(recvrecv[0]==1)
		{
			light_Led1();
			HAL_Delay(1000);
		}
		else if(recvrecv[1]==3)
		{
			light_Led2();
			HAL_Delay(1000);
		}
		else
		{
			OLED_DrawBMP(16,2,32,32,light1);
			OLED_DrawBMP(80,2,32,32,light1);
		}
		
  }
}
