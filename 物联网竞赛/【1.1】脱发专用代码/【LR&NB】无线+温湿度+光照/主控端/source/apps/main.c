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
#include "hal_oled.h"//oled屏
#include "adc_reader.h"//光照
#include "sht3x.h"//温湿度
#include "NS_Radio.h"
#include "usart1-board.h"
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

void menu()								//主界面
{
	OLED_ShowString(50,0,(uint8_t *)"Menu");
	OLED_ShowString(1,2,(uint8_t *)"1.Auto Lamp");
	OLED_ShowString(1,4,(uint8_t *)"2.Mula Lamp");
	OLED_ShowString(1,6,(uint8_t *)"3.View Freq");
}

void Auto_Lamp()					//光照温湿度界面
{
	OLED_ShowString(20,0,(uint8_t *)"Auto Lamp");
	OLED_ShowString(1,2,(uint8_t *)"Light:");
	OLED_ShowString(1,4,(uint8_t *)" Temp:");
	OLED_ShowString(1,6,(uint8_t *)" Humi:");
}

void Mula_Lamp()					//灯光控制界面
{
	OLED_ShowString(20,0,(uint8_t *)"Mula Lamp");
	OLED_ShowString(1,2,(uint8_t *)"Lamp:");
}
extern unsigned char bitmap4oled[];		//调用oledfont.h的变量（wifi图标）
void View_Freq()											//载波频率界面
{
	OLED_ShowString(20,0,(uint8_t *)"View Freq");
	OLED_ShowString(1,2,(uint8_t *)"Freq:");
	OLED_ShowNum(48,2,433000000,9,16);	//显示载波频率
	OLED_DrawBMP(1,4,27,24,bitmap4oled);//显示wifi图
}

uint8_t send[64]={0,1};	//用于存放无线发送内容的变量
													//第零位用于判断界面的标志位，如0代表界面为主界面，1代表界面为温湿度光照界面等...
													//第一位用于判断mula lamp界面的led灯状态，如果是0就代表open（即两个led开），如果是1就代表close（即两个led关）
													//第二位用于存放光照值整数部分除10的整数
													//第三位用于存放光照值整数部分除10的余数
													//第四位用于存放光照值的小数部分
													//第五位用于存放光照值的整数部分
													//第六位用于存放光照值的小数部分
													//第七位用于存放光照值的整数部分
													//第八位用于存放光照值的小数部分

int pointer_count=1;			//主界面的箭头
uint8_t interface_flag=0;	//判断界面的标志位

void menu_pointer()  			//判断主界面指针函数
{
	if(isKey2Pressed())			//如果Key2按下按下（代表指针向上移动）
	{
		OLED_Clear_Page(pointer_count*2);		//清除第pointer_count*2行（如：pointer_count=1，就代表仅清楚第二行）
		pointer_count--;
		if(pointer_count==0)pointer_count=3;//这个判断的作用是：如果指针在第一行时，按下了key2按键，就让按键置于第三行
		resetKey2();					//重置key2按键的状态
	}
	else if(isKey3Pressed())//如果Key3按键按下（代表指针向下移动）
	{
		OLED_Clear_Page(pointer_count*2);
		pointer_count++;
		if(pointer_count==4)pointer_count=1;//这个判断的作用是：如果指针在第三行时，按下了key3按键，就让按键置于第一行
		resetKey3();					//重置Key3按键的状态
	}
	
	OLED_ShowString(96,pointer_count*2,(uint8_t *)"<");//显示指针（按照pointer_count来进行改变、显示）
	
	if(isKey4Pressed())			//如果Key4按键按下（代表确认）
	{
		
		interface_flag = pointer_count;	//将指针在第几行赋值给判断界面的标志位
		send[0] = interface_flag;				//将判断界面的标志位赋值给发送变量的第零位
		Radio.Send(send,16);						//发送函数，将send发送给同频对象（可一对多）
		OLED_Clear();										//清除屏幕内容
		resetKey4();										//重置Key4按键的状态
	}
}

float light;				//用于存放光照值（用字符型是因为题目的温湿度光照保留了两位小数）
float hum;					//用于存放湿度值
float temp;					//用于存放温度值
int Led_state=1;		//用于记录led状态的变量（1代表led关闭，0代表led常量）

void Show_data()		//查看温湿度光照数据界面的内核
{
	char data[10];		//用于存放格式化后的数据值
	AdcScanChannel();	//转换ADC双信道寄存器数据为电压值
	light = (5/2.0)*(AdcReadCh0()*100);			//光照值的固定公式，AdcReadCh0()返回电压值【光照=（5/20）*（电压值*100）】
	sprintf(data,"%.2f  ",light);						//将附加参数light通过保留两位小数的格式写入data中存放
	OLED_ShowString(48,2,(uint8_t *)data);	//向oled屏输出data中的数据
	send[2]	=	(int)light / 10;							//取光照值的整数部分，并赋值给发送变量的第二位
	send[3] = (int)light % 10;							//除10并取余是因为char(uint8_t)最高只能存到255,光照很容易超过255Lx
	send[4] = (light-(int)light)*100;				//取光照值的小数部分，并赋值给发送变量的第三位
										//小数部分=（光照值-光照值的整数部分）*100————>乘100是因为无线传输时【char(uint8_t)】只能传输整数
	if(light<60)						//如果光照值小于60
	{
		GpioWrite(&Led1,0);
		GpioWrite(&Led2,0);		//开启Led1、Led2
		Led_state=0;					//记录Led1、Led2当前状态（常量）
		send[1]= Led_state;		//将Led当前状态赋值给发送变量的第一位
	}
	else 										//如果光照值大于等于60
	{
		GpioWrite(&Led1,1);
		GpioWrite(&Led2,1);		//关闭Led1、Led2
		Led_state=1;					//记录Led1、Led2当前状态(关闭)
		send[1]= Led_state;		//将Led当前状态存放在发送变量的第一位
	}	
	
	call_sht11(&temp,&hum);	//获取温湿度光照值并存放在变量temp、hum中
	sprintf(data,"%.2f  ",temp);					//将温度值通过保留两位小数的格式写入data中
	OLED_ShowString(48,4,(uint8_t *)data);//向oled屏输出data中的数据
	send[5]=(int)temp;									//保留温度值的整数部分，并赋值给发送变量的第四位
	send[6]=(temp-(int)temp)*100;						//保留温度值的小数部分，并赋值给发送变量的第五位
	USART1_SendStr(send,4);
	sprintf(data,"%.2f  ",hum);						//将温度值通过保留两位小数的格式写入data中
	OLED_ShowString(48,6,(uint8_t *)data);//向oled屏输出data中的数据
	send[7]=(int)hum;											//保留湿度值的整数部分，并赋值给发送变量的第六位
	send[8]=(hum-(int)hum)*100;						//保留湿度值的小数部分，并赋值给发送变量的第七位
	
	Radio.Send(send,16);									//发送函数，将send发送给同频对象（可一对多）
}

void Show_mula()					//控制Led状态的内核
{
	if(isKey2Pressed())			//如果Key2按键按下（Led1、Led2开）
	{
		GpioWrite(&Led1,0);
		GpioWrite(&Led2,0);
		Led_state=0;					//记录Led1、Led2当前状态（常量）
		send[1]= Led_state;		//将Led当前状态赋值给发送变量的第一位
		Radio.Send(send,16);	//发送函数，将send发送给同频对象（可一对多）
		resetKey2();					//重置Key2按键的状态
	}
	if(isKey3Pressed())			//如果Key3按键按下（Led1、Led2关）
	{
		GpioWrite(&Led1,1);
		GpioWrite(&Led2,1);
		Led_state=1;					//记录Led1、Led2当前状态（关闭）
		send[1]= Led_state;		//将Led当前状态赋值给发送变量的第一位
		Radio.Send(send,16);	//发送函数，将send发送给同频对象（可一对多）
		resetKey3();					//重置Key3按键的状态
	}
	if(Led_state==0)				//如果Led状态为常量
	{
		OLED_ShowString(48,2,(uint8_t *)"Open ");	//在Oled屏上显示Open（多一个空格是因为colse多一个字符，会和open冲突）
	}
	else if(Led_state==1)		//如果Led状态为关闭
	{
		OLED_ShowString(48,2,(uint8_t *)"Close");	//在Oled屏上显示Close
	}
}

/**
 * Main application entry point.
 */
int main( void )
{
  Init();							//开发板平台初始化
	OLED_Init();				//Oled屏初始化
	ADCS_Init();				//ADC通道0、通道2初始化
	hal_temHumInit();		//温湿度模块初始化
	NS_RadioInit(433000000,20,1000,1000);	//无线初始化
	send[0]=0;					//开发板复位后将send第零位置0（同频开发板将回到主界面）
	Radio.Send(send,16);//发送函数，将send发送给同频对象（可一对多）
	USART1_Init(115200);
  while(1)
  {
		if(interface_flag==0)					//当界面标志位为0时，显示主界面内容及指针
		{
			menu();					//界面
			menu_pointer();	//指针
		}
		else if(interface_flag==1)		//当界面标志位为1时，显示温湿度光照界面和数据
		{
			Auto_Lamp();		//温湿度界面
			Show_data();		//温湿度光照数据
			if(isKey4Pressed())					//如果按键4按下，清楚界面信息，返回主界面
			{
				interface_flag=0;					//界面标志位-置0
				send[0] = interface_flag;	//界面标志位赋值给发送变量的第零位
				Radio.Send(send,16);			//发送函数，将send发送给同频对象（可一对多）
				OLED_Clear();	//清屏
				resetKey4();	//重置Key4状态
			}
		}
		else if(interface_flag==2)		//当界面标志位为2时，显示led状态界面及操作内核
		{
			Mula_Lamp();		//状态界面
			Show_mula();		//状态内核
			if(isKey4Pressed())					//如果按键4按下，清楚界面信息，返回主界面
			{
				interface_flag=0;
				send[0] = interface_flag;
				Radio.Send(send,16);
				OLED_Clear();
				resetKey4();
			}
		}
		else if(interface_flag==3)		//当界面标志位为3时，显示载波频率及WiFi图标
		{
			View_Freq();		//载波频率及图标
			if(isKey4Pressed())					//如果按键4按下，清楚界面信息，返回主界面
			{
				interface_flag=0;
				send[0] = interface_flag;
				Radio.Send(send,16);
				OLED_Clear();
				resetKey4();
			}
		}
  }
}
