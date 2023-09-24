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
#include "adc_reader.h"
#include "sht3x.h"
#include "NS_Radio.h"
#include "math.h"
#include "usart1-board.h"
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

int interfaceFlag=1,count=0;

void interface1()
{
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(0,2,2);
	OLED_ShowCHinese(16,2,3);
	OLED_ShowCHinese(0,4,4);
	OLED_ShowCHinese(16,4,5);
	OLED_ShowString(0,6,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,6,6);
}

void interface2()
{
	OLED_ShowCHinese(0,0,2);
	OLED_ShowCHinese(16,0,3);
	OLED_ShowCHinese(0,2,4);
	OLED_ShowCHinese(16,2,5);
	OLED_ShowString(0,4,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,4,6);
	OLED_ShowString(0,6,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,6,7);
}

void interface3()
{
	OLED_ShowCHinese(0,0,4);
	OLED_ShowCHinese(16,0,5);
	OLED_ShowString(0,2,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,2,6);
	OLED_ShowString(0,4,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,4,7);
	OLED_ShowString(0,6,(uint8_t *)"LED2");
	OLED_ShowCHinese(32,6,8);
	OLED_ShowCHinese(48,6,9);
	OLED_ShowCHinese(64,6,10);
//	float light_light=(uint16_t)(5/2.0)*(100*AdcReadCh0());
//	float light = pow(10,((1.78-log10(33/AdcReadCh0()-10)/0.6)));
//	float MMP=pow(10,((1.395-log10((50/AdcReadCh0()-20)/10))/0.465));
//	float KQZ=pow(10,((0.2552-log10((5/AdcReadCh0()-1)/10))/0.5829));
}

void interface4()
{
	OLED_ShowString(0,0,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,0,6);
	OLED_ShowString(0,2,(uint8_t *)"LED1");
	OLED_ShowCHinese(32,2,7);
	OLED_ShowString(0,4,(uint8_t *)"LED2");
	OLED_ShowCHinese(32,4,8);
	OLED_ShowCHinese(48,4,9);
	OLED_ShowCHinese(64,4,10);
	OLED_ShowString(0,6,(uint8_t *)"LED2");
	OLED_ShowCHinese(32,6,11);
	OLED_ShowCHinese(48,6,12);
}

void key()
{
	if(interfaceFlag==1)
	{
		if(isKey2Pressed()&&count==0)
		{
//			OLED_Clear();
//			interfaceFlag=4;
//			count=6;
			resetKey2();
		}
		else if(isKey3Pressed()&&count==6)
		{
			OLED_Clear();
			interfaceFlag=2;
			count=6;
			resetKey3();
		}
		else if(isKey3Pressed())
		{
			OLED_Clear_Page(count);
			count+=2;
			resetKey3();
		}
		else if(isKey2Pressed())
		{
			OLED_Clear_Page(count);
			count-=2;
			resetKey2();
		}
	}
	if(interfaceFlag==2)
	{
		if(isKey2Pressed()&&count==0)
		{
			OLED_Clear();
			interfaceFlag=1;
			count=0;
			resetKey2();
		}
		else if(isKey3Pressed()&&count==6)
		{
			OLED_Clear();
			interfaceFlag=3;
			count=6;
			resetKey3();
		}
		else if(isKey3Pressed())
		{
			OLED_Clear_Page(count);
			count+=2;
			resetKey3();
		}
		else if(isKey2Pressed())
		{
			OLED_Clear_Page(count);
			count-=2;
			resetKey2();
		}
	}
	if(interfaceFlag==3)
	{
		if(isKey2Pressed()&&count==0)
		{
			OLED_Clear();
			interfaceFlag=2;
			count=0;
			resetKey2();
		}
		else if(isKey3Pressed()&&count==6)
		{
			OLED_Clear();
			interfaceFlag=4;
			count=6;
			resetKey3();
		}
		else if(isKey3Pressed())
		{
			OLED_Clear_Page(count);
			count+=2;
			resetKey3();
		}
		else if(isKey2Pressed())
		{
			OLED_Clear_Page(count);
			count-=2;
			resetKey2();
		}
	}
	if(interfaceFlag==4)
	{
		if(isKey2Pressed()&&count==0)
		{
			OLED_Clear();
			interfaceFlag=3;
			count=0;
			resetKey2();
		}
		else if(isKey3Pressed()&&count==6)
		{
//			OLED_Clear();
//			interfaceFlag=1;
//			count=0;
			resetKey3();
		}
		else if(isKey3Pressed())
		{
			OLED_Clear_Page(count);
			count+=2;
			resetKey3();
		}
		else if(isKey2Pressed())
		{
			OLED_Clear_Page(count);
			count-=2;
			resetKey2();
		}
	}
	OLED_ShowString(96,count,(uint8_t *)"<");
}






/**
 * Main application entry point.
 */
char str_light[255];
int main( void )
{
	Init();
	NS_RadioInit(433333000,20,1000,1000);
	OLED_Init();
	ADCS_Init();
	hal_temHumInit();
	unsigned int light;
	float temp[1],hum[1];
	USART1_Init(115200);
	uint8_t send[10];
//	char str[10];
	while(1)
	{
		key();
		if(interfaceFlag==1)interface1();
		else if(interfaceFlag==2)interface2();
		else if(interfaceFlag==3)interface3();
		else if(interfaceFlag==4)interface4();

		if(isKey4Pressed())
		{
			if(interfaceFlag==1&&count==0)send[0]=0x01;//����
			else if((interfaceFlag==1&&count==2)||(interfaceFlag==2&&count==0))send[0]=0x02;//�¶�
			else if((interfaceFlag==1&&count==4)||(interfaceFlag==2&&count==2)||(interfaceFlag==3&&count==0))send[0]=0x03;//ʪ��
			else if((interfaceFlag==1&&count==6)||(interfaceFlag==2&&count==4)||(interfaceFlag==3&&count==2)||(interfaceFlag==4&&count==0))send[0]=0x04;//LED1��
			else if((interfaceFlag==2&&count==6)||(interfaceFlag==3&&count==4)||(interfaceFlag==4&&count==2))send[0]=0x05;//LED1��
			else if((interfaceFlag==3&&count==6)||(interfaceFlag==4&&count==4))send[0]=0x06;//LED2������
			else if(interfaceFlag==4&&count==6)send[0]=0x07;//LED2��˸
			Radio.Send(send,1);
			resetKey4();
		}
		
		if(send[0]==0x01)
		{
			AdcScanChannel();
			light =  pow(10,((1.78-log10(33/AdcReadCh0()-10))/0.6));
			send[1]=light/256;//��
			send[2]=light%256;
			Radio.Send(send,3);
			HAL_Delay(5);
		}
		else if(send[0]==0x02)
		{
			call_sht11(temp,hum);
			send[1]=temp[0];
			Radio.Send(send,2);
		}
		else if(send[0]==0x03)
		{
			call_sht11(temp,hum);
			send[1]=hum[0];
			Radio.Send(send,2);
		}
	}
}
