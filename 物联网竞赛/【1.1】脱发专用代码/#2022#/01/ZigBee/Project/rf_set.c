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
uint flag=0;
uchar Send;
uchar recv;

void init_all()
{
  MCU_IO_DIR_OUTPUT(1,0);
  MCU_IO_DIR_OUTPUT(1,1);
  MCU_IO_DIR_OUTPUT(1,3);
  MCU_IO_DIR_OUTPUT(1,4);
  MCU_IO_DIR_INPUT(1,2);
  D3=D4=D5=D6=0;
}

void delay(uint t)
{
  while(t--);
}

/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x3333     //发送地址
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

uint count=0;
void Running()
{
  count++;
  if(count==5)count=1;
  switch(count)
  {
  case 1:D4=1;D3=D5=D6=0;break;
  case 2:D3=1;D4=D5=D6=0;break;
  case 3:D6=1;D3=D5=D4=0;break;
  case 4:D5=1;D3=D4=D6=0;break;
  }
  halMcuWaitMs(250);
}

void Key_Scan()
{
  if(SW1==0)
  {
    halMcuWaitMs(50);
    if(SW1==0)
    {
      while(!SW1)
      {
        Send=1;
        basicRfSendPacket(SEND_ADDR,&Send,1);
      }
      Send=0;
      basicRfSendPacket(SEND_ADDR,&Send,1);
    }
  }
}

/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  init_all();
  while(1)
  {
    /* user code start */
    Key_Scan();
    if(basicRfPacketIsReady())
    {
      if(basicRfReceive(&recv,1,NULL))
      {
        flag=recv;
      }
    }
    if(flag==1)
    {
      Running();
    }
    /* user code end */
  }
}