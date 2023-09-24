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
#include "adc_reader.h"
#include "oledfont.h"

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

int light=0;//����һ���������������ڹ���ֵ
char light_2[64];  //����һ�����������light�Ĺ���ֵ
uint16_t voltage;

void ADClight()
{
	AdcScanChannel();  //ת��ADC˫�ŵ��Ĵ�������Ϊ��ѹֵ
	//voltage = (AdcReadCh0()/4096.0) * 3300 ;
	light = 250* AdcReadCh0();  //��ȡ�ĵ�ѹֵͨ����ʽת��Ϊ����ֵ
  
	//1-��light��ֵ����light_2
	//2-��ӡ������������ֵ��Lx
  sprintf(light_2,"%dLx ",light);
	
	//memset(light_2,'\0',64);  
	
	OLED_ShowString(64,0,(uint8_t *)light_2);
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
  OLED_Init();
	ADCS_Init();
	OLED_ShowCHinese(0,0,0);  //������  #include oledfont.h  ��
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);   
  while( 1 )
  {
		
    ADClight();
		if(light<50)GpioWrite( &Led2, 0 );// 1 ��  0 ��
		else GpioWrite( &Led2, 1 );
  }
}
