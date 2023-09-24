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
    for (int delay = 0; delay < 100; delay++) {
        HAL_Delay(10);
    }
}
double count_X=112;
double count_Y=2;
unsigned int flag_X=0;
unsigned int flag_Y=0;
unsigned int level_vertical=0;

void Running_level()
{
  OLED_Clear();
  OLED_ShowCHinese(count_X,count_Y,0);
  if(count_X<112)OLED_ShowCHinese(count_X+16,count_Y,1);
	if(count_X<94)OLED_ShowCHinese(count_X+32,count_Y,2);
  if(count_X<80)OLED_ShowCHinese(count_X+48,count_Y,3);
	HAL_Delay(1000);
	if(count_X==0)flag_X=1;
  else if(count_X==112)flag_X=0;
	if(flag_X==1)count_X=count_X+16;
	else if(flag_X==0)count_X=count_X-16;
	if(count_X>112)count_X=112;
	if(count_X<0)count_X=0;
}

void Running_vertical()
{
	if(flag_Y==1)count_Y=count_Y-2;
	else if(flag_Y==0)count_Y=count_Y+2;
	if(count_Y>6)count_Y=6;
	if(count_Y<0)count_Y=0;
  OLED_Clear();
	OLED_ShowCHinese(count_X,count_Y,0);
  if(count_X<112)OLED_ShowCHinese(count_X+16,count_Y,1);
	if(count_X<94)OLED_ShowCHinese(count_X+32,count_Y,2);
  if(count_X<80)OLED_ShowCHinese(count_X+48,count_Y,3);
	HAL_Delay(1000);
	if(count_Y==6)flag_Y=1;
  else if(count_Y==0)flag_Y=0;
	
}
/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
  while( 1 )
  {
		if(isKey2Pressed())
		{
			flag_X=1;
			level_vertical=0;
			resetKey2();
		}
		if(isKey3Pressed())
		{
		  flag_X=0;
			level_vertical=0;
			resetKey3();
		}
		if(isKey4Pressed())
		{
			flag_Y=0;
		  level_vertical=1;
			resetKey4();
		}
		if(level_vertical==0)Running_level();
		if(level_vertical==1)Running_vertical();
  }
}
