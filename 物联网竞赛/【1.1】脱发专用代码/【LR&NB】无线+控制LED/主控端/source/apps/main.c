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
uint8_t send[2]={0,4};

void mune()
{
	OLED_ShowString(16,0,(uint8_t *)"LED1:");
	OLED_ShowString(16,4,(uint8_t *)"LED2:");
	OLED_ShowCHinese(64,0,0);
	OLED_ShowCHinese(96,0,1);
	OLED_ShowCHinese(64,4,0);
	OLED_ShowCHinese(96,4,1);
}

void pointer()
{
	if(count==1||count==2)OLED_Clear_Page(2);
	else if(count==3||count==4)OLED_Clear_Page(6);
	
}

void light_up()
{
	if(isKey4Pressed())
	{
		switch(count)
		{
			case 1:GpioWrite(&Led1,0);send[0]=count;break;
			case 2:GpioWrite(&Led1,1);send[0]=count;break;
			case 3:GpioWrite(&Led2,0);send[1]=count;break;
			case 4:GpioWrite(&Led2,1);send[1]=count;break;
		}
		Radio.Send(send,2);
		resetKey4();
	}
}

void KeyScan()
{
	if(isKey2Pressed())
	{
		pointer();
		if(count==0)count=1;
		else count--;
		if(count==0)count=4;
		resetKey2();
	}
	else if(isKey3Pressed())
	{
		pointer();
		if(count==0)count=1;
		else count++;
		if(count==5)count=1;
		resetKey3();
	}
	switch(count)
	{
		case 1:OLED_ShowCHinese(64,2,2);break;
		case 2:OLED_ShowCHinese(96,2,2);break;
		case 3:OLED_ShowCHinese(64,6,2);break;
		case 4:OLED_ShowCHinese(96,6,2);break;
	}
	
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
	mune();
	NS_RadioInit(433000000,20,1000,1111);
	Radio.Send(send,2);
  while(1)
  {
		KeyScan();
		light_up();
  }
}
