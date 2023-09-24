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
//extern unsigned char bitmap4oled[];
int move_flag=0;
int x=112,y=2;

void key_Scan()
{
	if(move_flag!=0)
	{
		if(isKey2Pressed())
		{
			move_flag=1;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			move_flag=2;
			resetKey3();
		}
		else if(isKey4Pressed())
		{
			move_flag=3;
			resetKey4();
		}
	}
}
/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init();
	int count=1;
	int up_down=0;
	int flag=0;
	while( 1 )
	{
		key_Scan();
		if(move_flag==0)
		{
			OLED_Clear();
			switch(count)
			{
				case 1:OLED_ShowCHinese(x,y,0);x=x-16;break;
				case 2:OLED_ShowCHinese(x,y,0);OLED_ShowCHinese(x+16,y,1);x=x-16;break;
				case 3:OLED_ShowCHinese(x,y,0);OLED_ShowCHinese(x+16,y,1);OLED_ShowCHinese(x+32,y,2);x=x-16;break;
				case 4:OLED_ShowCHinese(x,y,0);OLED_ShowCHinese(x+16,y,1);OLED_ShowCHinese(x+32,y,2);OLED_ShowCHinese(x+48,y,3);move_flag=1;break;
			}
			count++;
		}
		else if(move_flag==1&&x!=0)
		{
			flag=1;
			OLED_Clear();
			x=x-16;
			OLED_ShowCHinese(x,y,0);
			OLED_ShowCHinese(x+16,y,1);
			OLED_ShowCHinese(x+32,y,2);
			OLED_ShowCHinese(x+48,y,3);
			
		}
		else if(move_flag==2&&x+48!=112)
		{
			flag=1;
			OLED_Clear();
			x=x+16;
			OLED_ShowCHinese(x,y,0);
			OLED_ShowCHinese(x+16,y,1);
			OLED_ShowCHinese(x+32,y,2);
			OLED_ShowCHinese(x+48,y,3);
		}
		else if(move_flag==3)
		{
			flag=1;
			OLED_Clear();
			if(y==0)up_down=0;
			else if(y==6)up_down=1;
			if(up_down==0)y=y+2;
			else if(up_down==1)y=y-2;
			OLED_ShowCHinese(x,y,0);
			OLED_ShowCHinese(x+16,y,1);
			OLED_ShowCHinese(x+32,y,2);
			OLED_ShowCHinese(x+48,y,3);
		}
		HAL_Delay(1000);
		if(flag==1)
		{
			if(x==0)move_flag=2;
			else if(x+48==112)move_flag=1;
		}
	}
}
