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
//#include "hal_oled.h"
#include "adc_reader.h"
#include "math.h"
#include "sht3x.h"
#include "usart1-board.h"
#include "string.h"
#define  key2 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)

void interface()
{
	OLED_ShowCHinese(32,0,0);
	OLED_ShowCHinese(48,0,1);
	OLED_ShowCHinese(64,0,2);
	OLED_ShowCHinese(80,0,3);
	OLED_ShowCHinese(0,2,4);
	OLED_ShowCHinese(16,2,6);
	OLED_ShowCHinese(0,4,5);
	OLED_ShowCHinese(16,4,6);
	OLED_ShowCHinese(0,6,7);
	OLED_ShowCHinese(16,6,8);
}
int light;
float temp,hum;
uint8_t Usend[255];
char str[255];
void value()
{
	call_sht11(&temp,&hum);
	sprintf(str,"%d",(int)temp);
	OLED_ShowString(40,2,(uint8_t *)str);
	OLED_ShowCHinese(64,2,9);
	
	sprintf(str,"%d %%",(int)hum);
	OLED_ShowString(40,4,(uint8_t *)str);
	
	AdcScanChannel();
	light = pow(10,((1.78-log10(33/AdcReadCh0()-10))/0.6));	
	sprintf(str,"%-5d",light);
	OLED_ShowString(40,6,(uint8_t *)str);
	OLED_ShowString(80,6,(uint8_t *)"Lux");
	
	
}



void key()
{
	char strr[255],temp1,temp2,hum1,hum2;
	if(key2==0)
	{
		while(!key2)
		{
			value();HAL_Delay(500);
			temp1 = ((int)temp/10)+30;
			temp2 = ((int)temp%10)+30;
			hum1 = ((int)hum/10)+30;
			hum2 = ((int)hum%10)+30;
			sprintf(strr,"74 65 6D 70 65 72 61 74 75 72 65 28 A1 E6 29 3A %d %d 7C 68 75 6D 69 64 69 74 79 28 A3 A5 29 3A %d %d",temp1,temp2,hum1,hum2);
			USART1_SendStr((uint8_t *)strr,101);
		}
		USART1_SendStr((uint8_t *)" ",1);
	}
	sprintf(str,"temperature(℃):%d|humidity(％):%d",(int)temp,(int)hum);
	USART1_SendStr((uint8_t *)str,34);HAL_Delay(500);
	for(int i=0;i<102;i++)
	{
		strr[i]=str[i]=0;
	}
}

/**********************************************************************************************
*函数：void Init( void )
*功能：平台初始化
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void Init() {
    // 开发板平台初始化
    BoardInitMcu();
    BoardInitPeriph();
    keys_init();//按键初始化
//    setTimer2Callback(Time2Handler);
//    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
	 
}

/**********************************************************************************************
*函数：void KeyDownHandler( void )
*功能：按钮事件监听
*输入：无
*输出：无
*特殊说明：无
**********************************************************************************************/
void KeyDownHandler(void) {
	
}

/**********************************************************************************************
*函数：void handlerPre10Ms( void )
*功能：10毫秒循环
*输入：无
*输出：无
*特殊说明：循环处理总时长300ms
**********************************************************************************************/
void handlerPre10Ms(void) {
    for (int delay = 0; delay < 30; delay++) {
        HAL_Delay(10);
        
    }
}


/**
 * Main application entry point.
 */
int main( void )
{
	Init();
	ADCS_Init();
	OLED_Init_Inverse();
	hal_temHumInit();
	interface();
	USART1_Init(115200);
	while(1)
	{
		value();
		key();
	}
}
