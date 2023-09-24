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
#include "math.h"
#include "usart1-board.h"

//#include "sx1276.h"
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

#define KEY2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)
#define KEY3 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KEY4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)

/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	OLED_Init_Inverse();
	while(1)
	{
		if(KEY2==0)
		{
			DelayMs(50);
			if(KEY2==0)
			{
				while(!KEY2)
				{
					GpioWrite(&Led1,0);
				}
				GpioWrite(&Led1,1);
			}
		}
	}
}
