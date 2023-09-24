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

#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4
void Init()
{
  P1SEL &=!0x1b;
  P1DIR |= 0x1b;
  P1&=~0x1b;
}
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x1244     //本机模块地址
#define SEND_ADDR                 0x1255     //发送地址

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
uint8 Urecv[3]={0,0,0},value[1],valueLow[2],RFrecv[1],RFsend[2]={0xff,0x00};
int RFLen=0,flag=1;
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  basicRfSendPacket(SEND_ADDR,RFsend,1);
  Init();
  while(1)
  {
    if(basicRfPacketIsReady())
    {
      basicRfReceive(RFrecv,1,NULL);
      if(RFrecv[0]==0xff)flag=0;
    }
    if(flag==0)
    {
      P1&=~0x1b;
      D6=1;
      halMcuWaitMs(2000);
      P1&=~0x1b;
      flag=1;
      RFrecv[0]=RFsend[0]=0x00;
    }
    if(flag==1)
    {
      int len = halUartRead(Urecv,3);
      if(len>0)
      {
        switch(len)
        {
        case 1:value[0]=(Urecv[0]-0x30);break;
        case 2:value[0]=((Urecv[0]-0x30)*10)+((Urecv[1]-0x30)*1);break;
        case 3:value[0]=((Urecv[0]-0x30)*100)+((Urecv[1]-0x30)*10)+((Urecv[2]-0x30)*1);break;
        }
        RFsend[1]=value[0]/0x10;
        valueLow[0]=value[0]%0x10;
        basicRfSendPacket(SEND_ADDR,RFsend,2);
        switch(valueLow[0])
        {                               //4365
        case 0x00:D3=D4=D5=D6=0;break;  //0000
        case 0x01:D5=1;D3=D4=D6=0;break;//0001
        case 0x02:D6=1;D3=D4=D5=0;break;//0010
        case 0x03:D5=D6=1;D3=D4=0;break;//0011
        case 0x04:D3=1;D4=D5=D6=0;break;//0100
        case 0x05:D3=D5=1;D4=D6=0;break;//0101
        case 0x06:D3=D6=1;D4=D5=0;break;//0110
        case 0x07:D3=D5=D6=1;D4=0;break;//0111
        case 0x08:D4=1;D3=D5=D6=0;break;//1000
        case 0x09:D4=D5=1;D3=D6=0;break;//1001
        case 0x0a:D4=D6=1;D3=D5=0;break;//1010
        case 0x0b:D4=D5=D6=1;D3=0;break;//1011
        case 0x0c:D4=D3=1;D5=D6=0;break;//1100
        case 0x0d:D4=D3=D5=1;D6=0;break;//1101
        case 0x0e:D4=D3=D6=1;D5=0;break;//1110
        case 0x0f:D3=D4=D5=D6=1;break;  //1111
        }
      }
      value[0]=valueLow[0]=0;
    }
  }
  /* user code end */
}