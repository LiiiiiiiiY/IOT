#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#define D3 P1_0//01
#define D4 P1_1//02
#define D5 P1_3
#define D6 P1_4//10

void Init()
{
  P1SEL &=~0x1b;
  P1DIR |= 0x1b;
  P1&=~0x1b;
}

/*****点对点通讯地址设置******/
#define RF_CHANNEL                25         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x3333     //发送地址

/********无线数据缓存********/


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
uint8 Flag=0,RFSend[10]={0xff},RFRecv[1],Urecv[3],str[1],value[3];
void RFInit()
{
  int count=0;
  if(!basicRfPacketIsReady())
  {
    halMcuWaitMs(50);
    basicRfSendPacket(SEND_ADDR,RFSend,1);
  }
  else if(basicRfPacketIsReady())
  {
    basicRfReceive(RFRecv,1,NULL); 
    count = RFRecv[0];
    if(count==0xff)
    {
      Flag=1;
      D6=1;
      halMcuWaitMs(2000);
      D6=0;
    }
  }
  
}

void Udata2Led()
{
  int len =halUartRead(Urecv,255); 
  if(len>0)
  {
    if(Urecv[0]==0x0a)
    {
      value[1] = Urecv[1]/0x10;
      value[0] = Urecv[1]%0x10;
    }
    else
    {
      switch(len)
      {
      case 0x01:str[0]=(Urecv[0]-0x30);break;
      case 0x02:str[0]=((Urecv[0]-0x30)*10)+(Urecv[1]-0x30);break;
      case 0x03:str[0]=((Urecv[0]-0x30)*100)+((Urecv[1]-0x30)*10)+(Urecv[2]-0x30);break;
      }
      value[1] = str[0]/0x10;
      value[0] = str[0]%0x10;
    }
    halUartWrite(str,1);
    basicRfSendPacket(SEND_ADDR,value,2);
    switch(value[0])
    {
    case 0x00:D3=D4=D5=D6=0;break;
    case 0x01:D3=D4=D5=D6=0;D5=1;break;
    case 0x02:D3=D4=D5=D6=0;D6=1;break;
    case 0x03:D3=D4=D5=D6=0;D5=D6=1;break;
    case 0x04:D3=D4=D5=D6=0;D3=1;break;
    case 0x05:D3=D4=D5=D6=0;D3=D5=1;break;
    case 0x06:D3=D4=D5=D6=0;D3=D6=1;break;
    case 0x07:D3=D4=D5=D6=0;D3=D5=D6=1;break;
    case 0x08:D3=D4=D5=D6=0;D4=1;break;
    case 0x09:D3=D4=D5=D6=0;D4=D5=1;break;
    case 0x0a:D3=D4=D5=D6=0;D4=D6=1;break;
    case 0x0b:D3=D4=D5=D6=0;D4=D5=D6=1;break;
    case 0x0c:D3=D4=D5=D6=0;D4=D3=1;break;
    case 0x0d:D3=D4=D5=D6=0;D4=D3=D5=1;break;
    case 0x0e:D3=D4=D5=D6=0;D4=D3=D6=1;break;
    case 0x0f:D4=D3=D5=D6=1;break;
    }
  }
}


/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  halUartInit(9600);
  Init();
  D5=1;
  halMcuWaitMs(2000);
  D5=0;
  while(1)
  {
    if(Flag==0)
    {
      RFInit();
    }
    else if(Flag==1)
    {
      Udata2Led();
    }
  }
}