#include "hal_defs.h"
#include "hal_cc8051.h"//按键和LED初始化
#include "hal_int.h"
#include "hal_mcu.h"//延时
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "sensor_drv/sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4
#define SW1 P1_2

#define uint unsigned int
#define uchar unsigned char
uint flag=0,KeyFlag=0,count=0;
uchar Send,recv,h,a=1;
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x3333     //本机模块地址
#define SEND_ADDR                 0x2222     //发送地址
/**************************************************/
static basicRfCfg_t basicRfConfig;
// 无线RF初始化
void ConfigRf_Init(void)
{
  basicRfConfig.panId       =   PAN_ID;
  basicRfConfig.channel     =   RF_CHANNEL;
  basicRfConfig.myAddr      =   MY_ADDR;
  basicRfConfig.ackRequest  =   TRUE;
  while(basicRfInit(&basicRfConfig) == FAILED);
  basicRfReceiveOn();
}

void init_all()
{
  MCU_IO_DIR_OUTPUT(1,0);
  MCU_IO_DIR_OUTPUT(1,1);
  MCU_IO_DIR_OUTPUT(1,3);
  MCU_IO_DIR_OUTPUT(1,4);
  MCU_IO_DIR_INPUT(1,2);
  D3=D4=D5=D6=0;
}

void KeyScan()
{
  if(SW1==0)
  {
    halMcuWaitMs(5);
    if(SW1==0)
    {
      while(!SW1);
      count=0;
      KeyFlag=0;
      KeyFlag=1;
    }
  }
  if(KeyFlag==1)
  {
    halMcuWaitMs(5);
    count++;
    if(count<100)
    {
      if(SW1==0)
      {
        Send^=1;
        basicRfSendPacket(SEND_ADDR,&Send,1);
        KeyFlag=count=0;
        D3^=1;
      }
    }
    else if(count>99)
    {
      KeyFlag=count=0;
      D5^=1;
    }
  }
}

void InitT1()
{  
  T1CTL  |=0x01;      //定时器1时钟频率1分频，自动重装0x0000-0xFFFF
  PERCFG  =0x40;      //定时器1选择外设位置2
  P1SEL  |=0x02;      //P1_0选择外设功能0x01
  T1CCTL1 =0x64;      //定时器1通道2向上比较，比较模式
  T1CC1L  =0xff;        
  T1CC1H  =h;
}

void breath()
{
  if((T1STAT&0x02)>0)  
  {
    
    T1STAT=T1STAT&0xfd; //清除中断标志      
    if(a==1)h=h+1;      //渐亮
    else h=h-1;         //渐灭
    T1CC1L=0xff;        //重装比较值
    T1CC1H=h;
    if(h>=254)a=2;      //最大亮度，设为渐灭       
    if(h==0)a=1;        //最小亮度，设为渐亮         
  }
}


/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  init_all();
  InitT1();
  
  while(1)
  {
    /* user code start */
    KeyScan();
    
    if(basicRfPacketIsReady())
    {
      if(basicRfReceive(&recv,1,NULL))
      {
        flag=recv;
      }
    }
    if(flag==1)
    {
      breath();
    }
    /* user code end */
  }
}
