#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
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

void init_Port()
{
  P1SEL &=~0X1B;
  P1DIR |= 0X1B;
  P1    &=~0X1B;
  
  P1SEL &=~0X04;
  P1DIR &=~0X04;
  P1INP &=~0x04;
  P2INP &=~0x40;
}
int len;//用于存放安卓端发送的数据长度

/*存放安卓端发来的数据*/
unsigned char recv[1];

/*存放发给节点端的数据*/
unsigned char send[1]={0x00};

void Led_main()//主控端LED灯状态
{
  switch(recv[0])
  {
  case 0xA0:D3=D4=D5=D6=0;break;
  case 0xA1:D4=1;D3=D5=D6=0;break;
  case 0xA2:D3=1;D4=D5=D6=0;break;
  case 0xA3:D5=D6=0;D3=D4=1;break;
  case 0xA4:D6=1;D4=D5=D3=0;break;
  case 0xA5:D3=D5=0;D4=D6=1;break;
  case 0xA6:D3=D6=1;D4=D5=0;break;
  case 0xA7:D3=D4=D6=1;D5=0;break;
  case 0xA8:D5=1;D3=D4=D6=0;break;
  case 0xA9:D4=D5=1;D3=D6=0;break;
  case 0xAA:D4=D6=0;D3=D5=1;break;
  case 0xAB:D3=D4=D5=1;D6=0;break;
  case 0xAC:D3=D4=0;D5=D6=1;break;
  case 0xAD:D6=D4=D5=1;D3=0;break;
  case 0xAE:D3=D5=D6=1;D4=0;break;
  case 0xAF:D3=D4=D5=D6=1;break;
  default:basicRfSendPacket(SEND_ADDR,recv,1);break;
  }
}

void Key_Scan()
{
  if(SW1==0)
  {
    halMcuWaitUs(10);
    if(SW1==0)
    {
      while(!SW1);
      if(send[0]==0x01)send[0]=0x00;
      else if(send[0]==0x00)send[0]=0x01;
      basicRfSendPacket(SEND_ADDR,send,1);
    }
  }
}
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  init_Port();
  while(1)
  {
    /* user code start */
    Key_Scan();
    len = halUartRxLen();
    if(len>0)
    {
      halUartRead(recv,1);
      Led_main();
    }
    /* user code end */
  }
}