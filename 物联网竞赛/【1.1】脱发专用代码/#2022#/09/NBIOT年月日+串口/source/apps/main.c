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
#include "flash.h"
#include "usart1-board.h"
#include "uart_receiver.h"

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

unsigned int year=2022;//����һ����������ʾ���
unsigned int month=9;//����һ����������ʾ�·�
unsigned int day=28;//����һ����������ʾ����

unsigned int year1;
unsigned int year2;

uint8_t yes[3]={0XFB,0X00,0XFE};
uint8_t no[3]={0XFB,0X01,0XFE};

int flag=0;//��ʼ����������ý������ֵı�־λ�����ڳ�ʼ������ʱ������key4->flag=1->�������ý��棩
int count=2;
int Key4Flag=0;


/************************************�ж�����*********************************/
void day_count()
{
	if((year%4==0&&year%100!=0)||(year%400==0))
	{
		if(month==2)
		{
			if(day>29)day=1;
			else if(day==0)day=29;
		}
		else if(month==1||month==3||month==5||month==9||month==11)
		{
			if(day>30)day=1;
			else if(day==0)day=30;
		}
		else
		{
			if(day>31)day=1;
			else if(day==0)day=31;
		}
	}
	else
	{
	  if(month==2)
		{
			if(day>28)day=1;
			else if(day==0)day=28;
		}
		else if(month==1||month==3||month==5||month==9||month==11)
		{
			if(day>30)day=1;
			else if(day==0)day=30;
		}
		else
		{
			if(day>31)day=1;
			else if(day==0)day=31;
		}
	}
}

//

/************************************�������������ʾ*********************************/
void time()
{
	OLED_ShowCHinese(0 ,0,0);//��
	OLED_ShowCHinese(16,0,1);//ǰ
	OLED_ShowCHinese(32,0,2);//��
	OLED_ShowCHinese(48,0,3);//��
	OLED_ShowCHinese(64,0,7);//��
	OLED_ShowCHinese(32,3,4);//��
	OLED_ShowCHinese(16,6,5);//��
	OLED_ShowCHinese(48,6,6);//��
	OLED_ShowNum(0,3,year,4,16);
	OLED_ShowNum(0,6,month,2,16);
	OLED_ShowNum(32,6,day,2,16);
}
//


/******************************************���ý�����ʾ************************************/
void Set_up()
{
	OLED_ShowCHinese(0 ,0,4);//��
	OLED_ShowCHinese(16,0,7);//��
	OLED_ShowCHinese(0 ,3,5);//��
	OLED_ShowCHinese(16,3,7);//��
	OLED_ShowCHinese(0 ,6,6);//��
	OLED_ShowCHinese(16,6,7);//��
	OLED_ShowNum(48,0,year,4,16);
	OLED_ShowNum(48,3,month,4,16);
	OLED_ShowNum(48,6,day,4,16);
}


/*************************************��Key2��Key3�ı�������****************************/
void timeChange()
{
	if(count==2)
	{
		OLED_ShowString(96,0,(uint8_t *)"@");
		OLED_ShowString(96,3,(uint8_t *)" ");
		OLED_ShowString(96,6,(uint8_t *)" ");
		if(isKey2Pressed())
		{
			year++;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			year--;
			resetKey3();
		}
	}
	if(count==3)
	{
		OLED_ShowString(96,0,(uint8_t *)" ");
		OLED_ShowString(96,3,(uint8_t *)"@");
		OLED_ShowString(96,6,(uint8_t *)" ");
		if(isKey2Pressed())
		{
			month++;
			if(month>12)month=1;
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			month--;
			if(month==0)month=12;
			resetKey3();
		}
	}
	if(count==4)
	{
		OLED_ShowString(96,0,(uint8_t *)" ");
		OLED_ShowString(96,3,(uint8_t *)" ");
		OLED_ShowString(96,6,(uint8_t *)"@");
		if(isKey2Pressed())
		{
			day++;
			day_count();
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			day--;
			day_count();
			resetKey3();
		}
	}
}

/******************************�����ý��水��Key4�������ĺ�ȷ�ϸ���*******************/
void Key4_Scan()
{
	if(count!=1)
	{
		if(isKey4Pressed())
		{
			if(Key4Flag==0)
			{
				Key4Flag=1;
				
			}
		else if(Key4Flag==1)
		{
			Key4Flag=0;
			GpioWrite(&Led2,1);
			WriteFlashTest(0x0800f500,year);
			WriteFlashTest(0x0800f600,month);
			WriteFlashTest(0x0800f700,day);
			OLED_ShowString(96,0,(uint8_t *)" ");
			OLED_ShowString(96,3,(uint8_t *)" ");
			OLED_ShowString(96,6,(uint8_t *)" ");
		}
		resetKey4();
	}
	if(Key4Flag==1)timeChange();
	}
}
/**************�����ý����ƶ���ͷѡ�����������*********************/
void Pointer()
{
		if(isKey2Pressed())
		{
			count--;
			if(count==1)count=4;
			resetKey2();
		}
		if(isKey3Pressed())
		{
			count++;
			if(count==5)count=2;
			resetKey3();
		}
		switch(count)
		{
		case 2:OLED_ShowCHinese(80,0,8);OLED_ShowString(80,3,(uint8_t *)"  ");OLED_ShowString(80,6,(uint8_t *)"  ");break;
		case 3:OLED_ShowCHinese(80,3,8);OLED_ShowString(80,6,(uint8_t *)"  ");OLED_ShowString(80,0,(uint8_t *)"  ");break;
		case 4:OLED_ShowCHinese(80,6,8);OLED_ShowString(80,3,(uint8_t *)"  ");OLED_ShowString(80,0,(uint8_t *)"  ");break;
		}
}

/**
 * Main application entry point.
 */

//uint8_t send[3]={0xFB,0x00,0xFE};
//uint8_t recv_send[3]={0XFE,0X01,0XFE};
int main( void )
{
  Init();
	USART1_Init(115200);
	OLED_Init();
	year = PrintFlashTest(0x0800f500);
	month = PrintFlashTest(0x0800f600);
	day = PrintFlashTest(0x0800f700);
  while( 1 )
  {
		if(USART1_RX_COUNT>0)
		{
			HAL_Delay(10);
			if((USART1_RX_BUF[0]==0xFB) && (USART1_RX_BUF[1]==0x01) && (USART1_RX_BUF[4]==0xFE))//������ܵ�������ݵ�����
			{
				year1 = USART1_RX_BUF[2];//���������λ�����ݸ�ֵ����1
				year2 = USART1_RX_BUF[3];//���������λ�����ݸ�ֵ����2
				year1 = (year1/16)*10+(year1%16);//��1����>����16����ת10����
				year2 = (year2/16)*10+(year2%16);//��2����>����16����ת10����
				year  = (year1*100) + year2;//�������=��1��ʮ����*100+��2��ʮ����
				WriteFlashTest(0x0800f500,year);//�����յõ���������ݴ����flash��0x0800f500��
				USART1_SendStr((uint8_t *)yes,3);//����λ�������ɹ����ź�
				USART1_ReceiveClr();
			}
			else if((USART1_RX_BUF[0]==0xFB) && (USART1_RX_BUF[1]==0x02) && (USART1_RX_BUF[3]==0xFE))//������ܵ������·ݵ�����
			{
				month = USART1_RX_BUF[2];//���������λ�����ݸ�ֵ���·�ֵ
				month = (month/16)*10 + (month%16);//�·�ֵ����>����16����ת10����
				WriteFlashTest(0x0800f600,month);//�����յõ����·����ݴ����flash��0x0800f600��
				USART1_SendStr((uint8_t *)yes,3);//����λ�������ɹ����ź�
				USART1_ReceiveClr();
			}
			else if( (USART1_RX_BUF[0]==0xFB) && (USART1_RX_BUF[1]==0x03) && (USART1_RX_BUF[3]==0xFE) )//������ܵ��������ڵ�����
			{
				day = USART1_RX_BUF[2];		//���������λ�����ݸ�ֵ������ֵ
				day = (day/16)*10 + (day%16);//����ֵ����>����16����ת10����
				WriteFlashTest(0x0800f700,day);//�����յõ����������ݴ����flash��0x0800f700��
				USART1_SendStr((uint8_t *)yes,3);//����λ�������ɹ����ź�
				USART1_ReceiveClr();
			}
			else//������������������㣬�Ǳ�ʾû�н��յ���ȷ����Ϣ������ʧ�ܣ�
			{
				USART1_SendStr((uint8_t *)no,3);//����λ������ʧ�ܵ��ź�
				USART1_ReceiveClr();
			}
		}
		if(flag==0)
		{
			time();
			if(isKey4Pressed())
			{
				OLED_Clear();
				flag=1;
				resetKey4();
			}
		}
		else if(flag==1)
		{
			Set_up();
			if(Key4Flag==0)Pointer();
			Key4_Scan();
		}
  }
}
