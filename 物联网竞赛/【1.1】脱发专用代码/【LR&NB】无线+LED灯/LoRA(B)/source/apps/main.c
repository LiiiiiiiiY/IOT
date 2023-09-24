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
extern unsigned char quanliang[],banliang[],quanmie[];
unsigned int count=1,len=0,led1=0,led2=0;
uint8_t Recv[4]={0xAB,1,1,0xBA};
void jiemian()
{
	OLED_ShowString(16,6,(uint8_t *)"LED1");
	OLED_ShowString(80,6,(uint8_t *)"LED2");
}



/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init();
	NS_RadioInit(433003000,10,1000,1000);
	while(1)
	{
		jiemian();
		len=ReadRadioRxBuffer(Recv);
		if(len>0)
		{
			if(Recv[0]==0xAB&&Recv[3]==0xBA)
			{
				led1 = Recv[1];
				led2 = Recv[2];
			}
		}
		if(led1==1&&led2==1)
		{
			OLED_DrawBMP(16,1,32,32,quanmie);
			OLED_DrawBMP(80,1,32,32,quanmie);
		}
		else if(led1==0&&led2==1)
		{
			count++;
			switch(count)
			{
				case 1:OLED_DrawBMP(16,1,32,32,quanmie);break;
				case 2:OLED_DrawBMP(16,1,32,32,banliang);break;
				case 3:OLED_DrawBMP(16,1,32,32,quanliang);break;
				case 4:OLED_DrawBMP(16,1,32,32,banliang);break;
			}
			OLED_DrawBMP(80,1,32,32,quanmie);
			if(count==4)count=0;
			HAL_Delay(1000);
			OLED_Clear_Page(1);OLED_Clear_Page(3);
		}
		else if(led1==1&&led2==0)
		{
			count++;
			switch(count)
			{
				case 1:OLED_DrawBMP(80,1,32,32,quanmie);break;
				case 2:OLED_DrawBMP(80,1,32,32,banliang);break;
				case 3:OLED_DrawBMP(80,1,32,32,quanliang);break;
				case 4:OLED_DrawBMP(80,1,32,32,banliang);break;
			}
			OLED_DrawBMP(16,1,32,32,quanmie);
			if(count==4)count=0;
			HAL_Delay(1000);
			OLED_Clear_Page(1);OLED_Clear_Page(3);
		}
		else if(led1==0&&led2==0)
		{
			count++;
			switch(count)
			{
				case 1:OLED_DrawBMP(80,1,32,32,quanmie);OLED_DrawBMP(16,1,32,32,quanmie);break;
				case 2:OLED_DrawBMP(80,1,32,32,banliang);OLED_DrawBMP(16,1,32,32,banliang);break;
				case 3:OLED_DrawBMP(80,1,32,32,quanliang);OLED_DrawBMP(16,1,32,32,quanliang);break;
				case 4:OLED_DrawBMP(80,1,32,32,banliang);OLED_DrawBMP(16,1,32,32,banliang);break;
			}
			if(count==4)count=0;
			HAL_Delay(1000);
			OLED_Clear_Page(1);OLED_Clear_Page(3);
		}
	}
}
