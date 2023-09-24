#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "hal_pwm.h" 
#include "UART_PRINT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
//LED
#define LED1 P1_0
#define LED2 P1_1
#define LED3 P0_4
/*****点对点通讯地址设置******/
#define RF_CHANNEL                25         // 频道 11~26
#define PAN_ID                    0x2007     //网络id
#define MY_ADDR                   0x1234    //本机模块地址
/********无线数据缓存********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
uint8 signal;
/***********接收TCP/IP数据的缓存区************/
static uint8 TCP_DATA_BUF[255];
/**************************************************/
static basicRfCfg_t basicRfConfig;

// 无线RF初始化
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;        //zigbee的ID号设置
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee的频道设置
    basicRfConfig.myAddr      =   MY_ADDR;   //设置本机地址
    basicRfConfig.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                // 使能Basic_RF接收
}
void led_init()
{
  //#define LED1 P1_0
  //#define LED2 P1_1
  //#define LED3  P0_4
  P1SEL&=~0x03;
  P1DIR|=0x03;
  P0DIR&=~0x10;
  P0DIR|=0x10;
  LED1=0;
  LED2=0;
  LED3=0;
}
void t1_init()
{
  T1IE=1;
  T1CTL=0x0A;//101032分频，模计数模式，从0x0000~T1CCO(1000计数，即1模式)
  T1CCTL0 |=0X04;
  T1CC0H=0x27;
  T1CC0L=0x0f;
  TIMIF|=0x40;
  EA=1;
  T1IF=0;

}
int count=0;
#pragma vector=T1_VECTOR
__interrupt void T1_ISR()
{
    IRCON=0x00;//清除中断标志位
    //10ms定时到
    if(count++>=100)
    {
    count=0;
  //  LED1=!LED1;
    }
    if(USART1_RX_RECEIVING)
    {
        USART1_RX_TIMEOUT_COUNT++;
        if(USART1_RX_TIMEOUT_COUNT>=3)//超过30ms串口没有接收到数据
        {
          UART1_RX_FINISH=1;
          USART1_RX_TIMEOUT_COUNT=0;
          USART1_RX_RECEIVING=0;
        }
    }
}
/********************MAIN START************************/
void main(void)
{
    uint16 len = 0;   
    halBoardInit();  //模块相关资源的初始化
    ConfigRf_Init(); //无线收发参数的配置初始化 
    led_init();
    halUartInit1(115200); //初始化串口1的波特率为115200
    halUartInit0();//初始化串口0，波特率为115200
    t1_init();
    char data[]="uart0 start working!";
    printf_uart0(data,strlen(data));
    uint8 TryCount=0;
    //开始ESP8266连接服务器
    uint8 temp;
    for(TryCount=0;TryCount<3;TryCount++)
    {
        temp=ConnectToServer(MY_DEVICE_ID,MA_SECRET_KEY);
        if(temp != 0)
	{
	     //printf("Connect To Server ERROR=%d\r\n",temp);
	}
	else
	{
	    //printf("connect to servet success DEVICE_ID: temperature1 SECRET_KEY 27c4af8cd8d748dca888f809e9309184 \r\n");
	    break;
	}
    
    }
    while(1)
    {
       
        /*************************无线数据接收处理进程******************/
        if(basicRfPacketIsReady())   //查询有没收到无线信号
        {
            //接收无线数据
            len = basicRfReceive(pRxData, MAX_RECV_BUF_LEN, NULL);
          //  LED1=~LED1;
            signal=pRxData[0];
            ESP8266_SendSensor(signal);
            CLR_UART1_RX_BUF();
        }
        if(UART1_RX_FINISH)
        {
            uint8 len;
            memset(TCP_DATA_BUF,'\0',255);
            len=ESP8266_GetIpData(USART1_RX_BUF, TCP_DATA_BUF);
            printf_uart0(TCP_DATA_BUF,len);
            USER_DataAnalysisProcess(TCP_DATA_BUF);
            CLR_UART1_RX_BUF();
        }
    }
}
/************************MAIN END ****************************/