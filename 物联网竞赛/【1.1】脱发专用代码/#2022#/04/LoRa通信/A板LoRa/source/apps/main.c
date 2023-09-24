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
unsigned int count=1;
unsigned int flag=0;
extern unsigned char WIFI[];

uint8_t send[4];


float light;   //存放光照值
float temp;    //存放温度值
float humi;		 //存放湿度值
char data_light[64];
char data_temp[64];
char data_hum[64];
unsigned int Mala_Lamp_Key;


void Menu(){//主界面

  OLED_ShowString(48,0,(uint8_t *)"Menu");//主界面标题：Menu
	OLED_ShowString(2,2,(uint8_t *)"1.Auto Lamp");//第一个选项
	OLED_ShowString(2,4,(uint8_t *)"2.Mala Lamp");//第二个选项
	OLED_ShowString(2,6,(uint8_t *)"3.View Freq");//第三个选项
	
	if(isKey2Pressed())//如果按键2按下
	{
	  count--;//箭头往下移动1格
		OLED_Clear();//清除屏幕
		resetKey2();//重置按键2
	}
	
	if(isKey3Pressed())//如果按键3按下
	{
	  count++;//箭头向上移动1格
		OLED_Clear();//清除屏幕
		resetKey3();//重置按键3
	}
	if(count==4)count=1;//如果箭头向下超出屏幕，让箭头在第一排（最上面一排）
	else if(count==0)count=3;//如果箭头向上超出屏幕，让箭头在第四排（最下面一排）
	OLED_ShowChar(96,count*2,'<');//箭头垂直位置移动*2
}
/**/



void Auto_Lamp(){//第一个界面(温湿度光照值)
	
	OLED_ShowString(28,0,(uint8_t *)"Auto Lamp");//第一个界面标题
	OLED_ShowString(2,2,(uint8_t *)"Light:");//第一排
	OLED_ShowString(3,4,(uint8_t *)"Temp: ");//第二排
	OLED_ShowString(3,6,(uint8_t *)"Humi: ");//第三排
	
	//光照数据转换并显示
	AdcScanChannel();//转换ADC双信道寄存器数据为电压值
	light = 250 * AdcReadCh0();//光照公式（光照值 = 250 * 电压值）
	sprintf(data_light,"%0.2f ",light);//
	//USART1_SendStr((uint8_t *)data_light,3);
	OLED_ShowString(54,2,(uint8_t *)data_light);//在OLED屏上显示光照值（横坐标——第54列开始显示，纵坐标——第2排开始显示）
	
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
	
	//温度数据显示
	SHT1x_get_temperature(&temp);//获取温度数据并保存在变量temp中
	sprintf(data_temp,"%0.2f ",temp);//
	OLED_ShowString(54,4,(uint8_t *)data_temp);//在OLED屏上显示温度数据（横坐标——第54列开始显示，纵坐标——第4排开始显示）
	
	//湿度数据显示
	SHT1x_get_relative_humidity(&humi);//获取湿度数据并保存在变量humidity中
	sprintf(data_hum,"%0.2f ",humi);//
	OLED_ShowString(54,6,(uint8_t *)data_hum);//在OLED屏上显示湿度数据（横坐标——第54列开始显示，纵坐标——第6排开始显示）
	
	send[1] = data_light[0];
	send[2] = data_temp[0];
	send[3] = data_hum[0];
}
/**/



void Mala_Lamp_Key2(){//第二个界面（灯光转换）
	
	if(Mala_Lamp_Key==2)//如果在第二个界面时，key2按下
	{
    OLED_ShowString(2,2,(uint8_t *)"Lamp:Open ");//在oled屏上显示open
	  GpioWrite( &Led1, 0 );
	  GpioWrite( &Led2, 0 );//Led1  Led2  低电平熄灭
	}
	
	if(Mala_Lamp_Key==3)//如果在第二个界面时，key3按下
	{
	  OLED_ShowString(2,2,(uint8_t *)"Lamp:Close");
		GpioWrite( &Led1, 1 );
	  GpioWrite( &Led2, 1 );//Led1  Led2  高电平熄灭
	}
}
/**/



void Mala_Lamp(){//第二个界面
	if(isKey2Pressed())//如果在第二个界面时，按下Key2
	{
		Mala_Lamp_Key=2;//让lamp后显示open并点亮Led1  Led2
		resetKey2();   //重置Key2状态
	}
	if(isKey3Pressed())
	{
		Mala_Lamp_Key=3;//让lamp后面显示close并熄灭led1 led2
		resetKey3();//重置Key3状态
	}
	Mala_Lamp_Key2();
  OLED_ShowString(28,0,(uint8_t *)"Mula Lamp");//第二个界面标题
}
/**/



void View_Freq(){//第三个界面（LoRa频率）
  OLED_ShowString(28,0,(uint8_t *)"View Freq");//第三个界面的标题
	OLED_ShowString(2,2,(uint8_t *)"Freq:433000000");//第一排显示内容
	OLED_DrawBMP(2,4,16,16,WIFI);//wifi图标
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
		else if(flag==1)Auto_Lamp();  //第二个界面
		else if(flag==2)Mala_Lamp();  //第三个界面
		else if(flag==3)View_Freq();  //第四个界面
		
		if(isKey4Pressed())                   //如果Key4按下
	  {
		  OLED_Clear();                       //清除屏幕
			
		  if(count==1)                        //如果Key2按下时，箭头在第一排
		  {
		    if(flag==0)                       //如果按下之前在主界面
				{
					flag=1;                         //进入第一个界面
					send[0]=1;                       //将1赋值给发送变量的第一位（代表B板进入第一个界面）
				}
				else if(flag==1)                  //如果不在主界面
				{
					flag=0;                         //进入主界面
					send[0]=0;                       //将0赋值给发送变量的第一位（代表B板进入主界面）
				}
		  }
			
			
		  if(count==2)                   //如果Key4按下时，箭头在第二排
		  {

				if(flag==0)                       //如果按下之前在主界面
				{
					int t = GpioRead(&Led1);        //读取LED1状态并赋值给变量t
					if(t==0)Mala_Lamp_Key=2;        //如果led1等于0，代表led1当前为打开状态，让lamp后面显示open
					else Mala_Lamp_Key=3;           //否则led1就是等于1，代表led1当前为关闭状态，让lamp后面显示close
					flag=2;                         //进入第二个界面
					send[0]=2;                       //将2赋值给发送变量的第一位（代表B板进入第二个界面）
				}
			  else if(flag==2)                  //如果按下之前不在主界面
			  { 
			  	flag=0;                         //进入主界面
					send[0]=0;                       //将0赋值给发送变量的第一位（代表B板进入主界面）
			  }
		  }
			
			
		  if(count==3)                   //如果Key4按下时，箭头在第三排
		  {
			  if(flag==0)                       //如果按下之前在主界面
				{
					flag=3;                         //进入第三个界面
					send[0]=3;                       //将2赋值给发送变量的第一位（代表B板进入第二个界面）
				}
			  else if(flag==3)                  //如果按下之前不在主界面
			  {
				  flag=0;                         //进入主界面
					send[0]=0;                       //将0赋值给发送变量的第一位（代表B板进入主界面）
		  	}
	  	}
		 	resetKey4();                        //重置Key4状态
  	}
		Radio.Send(send,4);                    //发送str内容
	}
}
