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
#include "hal_oled.h"//oled��
#include "adc_reader.h"//����
#include "sht3x.h"
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

int interface_flag=0;
extern unsigned char bitmap4oled[];


void select_menu()	
{
	OLED_ShowString(18,0,(uint8_t *)"select menu");
	OLED_DrawBMP(50,4,27,24,bitmap4oled);
}

void Auto_Lamp()		
{
	OLED_ShowString(20,0,(uint8_t *)"Auto Lamp");
	OLED_ShowString(1,2,(uint8_t *)"Light:");
	OLED_ShowString(1,4,(uint8_t *)" Temp:");
	OLED_ShowString(1,6,(uint8_t *)" Humi:");
}

void View_Freq()	
{
	OLED_ShowString(20,0,(uint8_t *)"View Freq");
	OLED_ShowString(1,2,(uint8_t *)"Freq:");
	OLED_ShowNum(48,2,433000000,9,16);
}

/**
 * Main application entry point.
 */
int main( void )
{
	uint8_t recv[64]={0,1};	
  int len;								
	int data_flag=0;				
	Init();									
	OLED_Init();					
	NS_RadioInit(433000000,20,1000,1000);		
  while(1)
  {
		//HAL_Delay(10);				
		len = ReadRadioRxBuffer(recv);		
		if(data_flag==1)			
		{
			char data[10];			
			float light_H,light_L,light;			
			light_H = (recv[2]*10)+recv[3];			
			light_L = recv[4];	
			light = light_H + (light_L / 100);
			sprintf(data,"%.2f  ",light);			
			OLED_ShowString(48,2,(uint8_t *)data);
		
			float temp_H,temp_L,temp;				
			temp_H = recv[5];	
			temp_L = recv[6];	
			temp = temp_H + (temp_L / 100);	
			sprintf(data,"%.2f  ",temp);		
			OLED_ShowString(48,4,(uint8_t *)data);	
			float hum_H,hum_L,hum;						
			hum_H = recv[7];		
			hum_L = recv[8];	
			hum = hum_H + (hum_L / 100);		
			sprintf(data,"%.2f  ",hum);			
			OLED_ShowString(48,6,(uint8_t *)data);
			
			if(light<60)			
			{
				GpioWrite(&Led1,0);
				GpioWrite(&Led2,0);
			}
			else					
			{
				GpioWrite(&Led1,1);
				GpioWrite(&Led2,1);
			}
			
			if(recv[0]==0)			
			{
				data_flag=0;
				OLED_Clear();
				select_menu();
			}
		}
		else if(len>0)			
		{
			OLED_Clear();
			if(recv[0]==0)		
			{
				select_menu();
			}
			else if(recv[0]==1)
			{
				Auto_Lamp();
				data_flag=1;		
			}
			else if(recv[0]==2)	
			{
				OLED_ShowString(20,0,(uint8_t *)"Mula Lamp");
				if(recv[1]==0)	
				{
					OLED_ShowString(1,2,(uint8_t *)"Lamp:Open ");
					GpioWrite(&Led1,0);
					GpioWrite(&Led2,0);
				}
				else if(recv[1]==1)
				{
					OLED_ShowString(1,2,(uint8_t *)"Lamp:Close");
					GpioWrite(&Led1,1);
					GpioWrite(&Led2,1);
				}
			}
			else if(recv[0]==3)
			{
				View_Freq();
			}
		}
		
  }
}
