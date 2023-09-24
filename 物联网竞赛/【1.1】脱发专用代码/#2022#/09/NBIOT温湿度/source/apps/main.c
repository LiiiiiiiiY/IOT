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
#include "sht1x.h"


uint16_t tenMSecClick=0;																		// 10毫秒计时器



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

    setTimer2Callback(Time2Handler);
    Tim2McuInit(1);//定时器初始化，设置定时中断1ms中断一次
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



char data_temp[64]={0};
char data_hum[64]={0};
float temp;
float humidity;

void Data()
{
	//memset(data_temp,'\0',64);
//  SHT1x_get_temperature(&temp);
//  sprintf(data_temp,"%0.2f",temp);
	OLED_ShowString(20,4,(uint8_t *)data_temp);
	OLED_ShowCHinese(65,4,6);
	
	//memset(data_hum,'\0',64);
//  SHT1x_get_relative_humidity(&humidity);
//	sprintf(data_hum,"%0.2f",humidity);
	OLED_ShowString(81,4,(uint8_t *)data_hum);
	OLED_ShowChar(123,4,'%');
}

void Oled1()
{
  OLED_ShowCHinese(20,0,0);//当
	OLED_ShowCHinese(36,0,1);//前
	OLED_ShowCHinese(52,0,2);//温
	OLED_ShowCHinese(68,0,3);//湿
	OLED_ShowCHinese(84,0,4);//度
	OLED_ShowCHinese(100,0,5);//：
}

unsigned int flag=0;

/**
 * Main application entry point.
 */
int main( void )
{
  Init();
	OLED_Init();
  SHT1x_init();
	Oled1();
  while(1)
  {
		SHT1x_get_temperature(&temp);
    sprintf(data_temp,"%0.0f",temp);
		
		SHT1x_get_relative_humidity(&humidity);
	  sprintf(data_hum,"%0.0f",humidity);
		
		if(isKey2Pressed())
		{
			flag=2;
			OLED_Clear();
			resetKey2();
		}
		else if(isKey3Pressed())
		{
			flag=3;
			OLED_Clear();
			resetKey3();
		}
		else if(isKey4Pressed())
		{
			flag=0;
			resetKey4();
			OLED_Clear();
		}
		if(flag==0)
		{
			Oled1();
		  OLED_ShowString(20,4,(uint8_t *)data_temp);
	    OLED_ShowCHinese(40,4,6);
			OLED_ShowChar(56,4,',');
	    OLED_ShowString(72,4,(uint8_t *)data_hum);
	    OLED_ShowChar(92,4,'%');
		}
		else if(flag==2)
		{
		  OLED_ShowCHinese(20,0,0);//当
	    OLED_ShowCHinese(36,0,1);//前
	    OLED_ShowCHinese(52,0,2);//温
			OLED_ShowCHinese(68,0,4);//度
	    OLED_ShowCHinese(84,0,5);//：
	    OLED_ShowString(20,4,(uint8_t *)data_temp);
	    OLED_ShowCHinese(40,4,6);
		}
		else if(flag==3)
		{
		  OLED_ShowCHinese(20,0,0);//当
	    OLED_ShowCHinese(36,0,1);//前
	    OLED_ShowCHinese(52,0,3);//湿
			OLED_ShowCHinese(68,0,4);//度
	    OLED_ShowCHinese(84,0,5);//：
    	OLED_ShowString(20,4,(uint8_t *)data_hum);
	    OLED_ShowChar(40,4,'%');
		}
  }
}
