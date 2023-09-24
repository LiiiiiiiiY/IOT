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

int light=0;//定义一个变量，让他等于光照值
char light_2[64];  //定义一个数组来存放light的光照值
uint16_t voltage;

void ADClight()
{
	AdcScanChannel();  //转换ADC双信道寄存器数据为电压值
	//voltage = (AdcReadCh0()/4096.0) * 3300 ;
	light = 250* AdcReadCh0();  //获取的电压值通过公式转化为光照值
  
	//1-把light的值赋给light_2
	//2-打印出——（光照值）Lx
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
	OLED_ShowCHinese(0,0,0);  //矩阵在  #include oledfont.h  中
	OLED_ShowCHinese(16,0,1);
	OLED_ShowCHinese(32,0,2);
	OLED_ShowCHinese(48,0,3);   
  while( 1 )
  {
		
    ADClight();
		if(light<50)GpioWrite( &Led2, 0 );// 1 灭  0 亮
		else GpioWrite( &Led2, 1 );
  }
}
