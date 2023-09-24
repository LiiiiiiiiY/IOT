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
#include "sht1x.h"
#include "adc_reader.h"
#include "NS_Radio.h"
#include "usart1-board.h"

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
unsigned int count=1;
unsigned int flag=0;
extern unsigned char WIFI[];

uint8_t send[4];


float light;   //��Ź���ֵ
float temp;    //����¶�ֵ
float humi;		 //���ʪ��ֵ
char data_light[64];
char data_temp[64];
char data_hum[64];
unsigned int Mala_Lamp_Key;


void Menu(){//������

  OLED_ShowString(48,0,(uint8_t *)"Menu");//��������⣺Menu
	OLED_ShowString(2,2,(uint8_t *)"1.Auto Lamp");//��һ��ѡ��
	OLED_ShowString(2,4,(uint8_t *)"2.Mala Lamp");//�ڶ���ѡ��
	OLED_ShowString(2,6,(uint8_t *)"3.View Freq");//������ѡ��
	
	if(isKey2Pressed())//�������2����
	{
	  count--;//��ͷ�����ƶ�1��
		OLED_Clear();//�����Ļ
		resetKey2();//���ð���2
	}
	
	if(isKey3Pressed())//�������3����
	{
	  count++;//��ͷ�����ƶ�1��
		OLED_Clear();//�����Ļ
		resetKey3();//���ð���3
	}
	if(count==4)count=1;//�����ͷ���³�����Ļ���ü�ͷ�ڵ�һ�ţ�������һ�ţ�
	else if(count==0)count=3;//�����ͷ���ϳ�����Ļ���ü�ͷ�ڵ����ţ�������һ�ţ�
	OLED_ShowChar(96,count*2,'<');//��ͷ��ֱλ���ƶ�*2
}
/**/



void Auto_Lamp(){//��һ������(��ʪ�ȹ���ֵ)
	
	OLED_ShowString(28,0,(uint8_t *)"Auto Lamp");//��һ���������
	OLED_ShowString(2,2,(uint8_t *)"Light:");//��һ��
	OLED_ShowString(3,4,(uint8_t *)"Temp: ");//�ڶ���
	OLED_ShowString(3,6,(uint8_t *)"Humi: ");//������
	
	//��������ת������ʾ
	AdcScanChannel();//ת��ADC˫�ŵ��Ĵ�������Ϊ��ѹֵ
	light = 250 * AdcReadCh0();//���չ�ʽ������ֵ = 250 * ��ѹֵ��
	sprintf(data_light,"%0.2f ",light);//
	//USART1_SendStr((uint8_t *)data_light,3);
	OLED_ShowString(54,2,(uint8_t *)data_light);//��OLED������ʾ����ֵ�������ꡪ����54�п�ʼ��ʾ�������ꡪ����2�ſ�ʼ��ʾ��
	
	//SX1276Send((uint8_t *)data_light,6);
	
	if(light<80)
	{
	  GpioWrite( &Led1, 0 );
		GpioWrite( &Led2, 0 );
	}
	else 
	{
		GpioWrite( &Led1, 1 );
		GpioWrite( &Led2, 1 );
	}
	
	//�¶�������ʾ
	SHT1x_get_temperature(&temp);//��ȡ�¶����ݲ������ڱ���temp��
	sprintf(data_temp,"%0.2f ",temp);//
	OLED_ShowString(54,4,(uint8_t *)data_temp);//��OLED������ʾ�¶����ݣ������ꡪ����54�п�ʼ��ʾ�������ꡪ����4�ſ�ʼ��ʾ��
	
	//ʪ��������ʾ
	SHT1x_get_relative_humidity(&humi);//��ȡʪ�����ݲ������ڱ���humidity��
	sprintf(data_hum,"%0.2f ",humi);//
	OLED_ShowString(54,6,(uint8_t *)data_hum);//��OLED������ʾʪ�����ݣ������ꡪ����54�п�ʼ��ʾ�������ꡪ����6�ſ�ʼ��ʾ��
	
	send[1] = data_light[0];
	send[2] = data_temp[0];
	send[3] = data_hum[0];
}
/**/



void Mala_Lamp_Key2(){//�ڶ������棨�ƹ�ת����
	
	if(Mala_Lamp_Key==2)//����ڵڶ�������ʱ��key2����
	{
    OLED_ShowString(2,2,(uint8_t *)"Lamp:Open ");//��oled������ʾopen
	  GpioWrite( &Led1, 0 );
	  GpioWrite( &Led2, 0 );//Led1  Led2  �͵�ƽϨ��
	}
	
	if(Mala_Lamp_Key==3)//����ڵڶ�������ʱ��key3����
	{
	  OLED_ShowString(2,2,(uint8_t *)"Lamp:Close");
		GpioWrite( &Led1, 1 );
	  GpioWrite( &Led2, 1 );//Led1  Led2  �ߵ�ƽϨ��
	}
}
/**/



void Mala_Lamp(){//�ڶ�������
	if(isKey2Pressed())//����ڵڶ�������ʱ������Key2
	{
		Mala_Lamp_Key=2;//��lamp����ʾopen������Led1  Led2
		resetKey2();   //����Key2״̬
	}
	if(isKey3Pressed())
	{
		Mala_Lamp_Key=3;//��lamp������ʾclose��Ϩ��led1 led2
		resetKey3();//����Key3״̬
	}
	Mala_Lamp_Key2();
  OLED_ShowString(28,0,(uint8_t *)"Mula Lamp");//�ڶ����������
}
/**/



void View_Freq(){//���������棨LoRaƵ�ʣ�
  OLED_ShowString(28,0,(uint8_t *)"View Freq");//����������ı���
	OLED_ShowString(2,2,(uint8_t *)"Freq:433000000");//��һ����ʾ����
	OLED_DrawBMP(2,4,16,16,WIFI);//wifiͼ��
}
/**/



int main( void )
{
  Init();
  OLED_Init();
	ADCS_Init();
	SHT1x_init();
	NS_RadioInit(433000000,10,1000,1000);
  while( 1 )
  {
		//flag=1;
		if(flag == 0)Menu();
		else if(flag==1)Auto_Lamp();  //�ڶ�������
		else if(flag==2)Mala_Lamp();  //����������
		else if(flag==3)View_Freq();  //���ĸ�����
		
		if(isKey4Pressed())                   //���Key4����
	  {
		  OLED_Clear();                       //�����Ļ
			
		  if(count==1)                        //���Key2����ʱ����ͷ�ڵ�һ��
		  {
		    if(flag==0)                       //�������֮ǰ��������
				{
					flag=1;                         //�����һ������
					send[0]=1;                       //��1��ֵ�����ͱ����ĵ�һλ������B������һ�����棩
				}
				else if(flag==1)                  //�������������
				{
					flag=0;                         //����������
					send[0]=0;                       //��0��ֵ�����ͱ����ĵ�һλ������B����������棩
				}
		  }
			
			
		  if(count==2)                   //���Key4����ʱ����ͷ�ڵڶ���
		  {

				if(flag==0)                       //�������֮ǰ��������
				{
					int t = GpioRead(&Led1);        //��ȡLED1״̬����ֵ������t
					if(t==0)Mala_Lamp_Key=2;        //���led1����0������led1��ǰΪ��״̬����lamp������ʾopen
					else Mala_Lamp_Key=3;           //����led1���ǵ���1������led1��ǰΪ�ر�״̬����lamp������ʾclose
					flag=2;                         //����ڶ�������
					send[0]=2;                       //��2��ֵ�����ͱ����ĵ�һλ������B�����ڶ������棩
				}
			  else if(flag==2)                  //�������֮ǰ����������
			  { 
			  	flag=0;                         //����������
					send[0]=0;                       //��0��ֵ�����ͱ����ĵ�һλ������B����������棩
			  }
		  }
			
			
		  if(count==3)                   //���Key4����ʱ����ͷ�ڵ�����
		  {
			  if(flag==0)                       //�������֮ǰ��������
				{
					flag=3;                         //�������������
					send[0]=3;                       //��2��ֵ�����ͱ����ĵ�һλ������B�����ڶ������棩
				}
			  else if(flag==3)                  //�������֮ǰ����������
			  {
				  flag=0;                         //����������
					send[0]=0;                       //��0��ֵ�����ͱ����ĵ�һλ������B����������棩
		  	}
	  	}
		 	resetKey4();                        //����Key4״̬
  	}
		Radio.Send(send,4);                    //����str����
	}
}
