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
#include "oledfont.h"
#include "hal_oled.h"

uint16_t tenMSecClick=0;																		// 10�����ʱ��
int i=1;
int j;
char buffer[2]={0};
char buffer2[2]={0};


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
    for (int delay = 0; delay < 100; delay++) {
        HAL_Delay(10);  
    }
}

void OLED(){
  OLED_ShowCHinese(40,0,0);
	OLED_ShowCHinese(56,0,1);
	OLED_ShowCHinese(72,0,2);
	
	OLED_ShowCHinese(30,3,3);
	OLED_ShowCHinese(46,3,4);
	OLED_ShowString (62,3,(uint8_t *)":");	
	
	OLED_ShowCHinese(30,6,5);
	OLED_ShowCHinese(46,6,6);
	OLED_ShowString (62,6,(uint8_t *)":");
}

void KeyScan(){
  if(isKey2Pressed()){
  	j--;resetKey2();
	}
	if(isKey3Pressed()){
		j++;resetKey3();
	}
  if(isKey4Pressed()){
	  GpioWrite(&Led2,0);
	 	HAL_Delay(1000);
	  GpioWrite(&Led2,1);
	 	resetKey4();
  }
}


int main( void )
{
  Init();
	OLED_Init();
	OLED();
	//memset(buffer,'0',64);//�����ڴ�
	sprintf(buffer,"%d",i);
	OLED_ShowString (70,3,(uint8_t *)buffer);
	j=i;
  while(1)
  {
		KeyScan();
	//	memset(buffer,'0',64);//�����ڴ�
		sprintf(buffer2,"%d",j);
		OLED_ShowString(70,6,(uint8_t *)buffer2);
  }
}
