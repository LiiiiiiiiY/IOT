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
#include <string.h>


#define SW1 P1_2
#define SW2 P0_1
#define JD1 P1_7
#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x3333     //发送地址
/********无线数据缓存********/

void Init()
{
  P1SEL &=~0x9B;
  P1DIR |= 0x9B;
  D3 = D4 =D5 = D6 = JD1=0;
  P1INP &=~0x04;
  P2INP &=~0x40;
  
  P0SEL &=~0x02;
  P0DIR &=~0x02;
  P0INP &=~0x02;
  P2INP &=~0x20;
}

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
unsigned char send[64],Urecv[255];

unsigned char Send_ledDisplay1[255]; 
unsigned char LedDisplay[25]={0xAA,0x01,0xBB,0x51,0x54,0,0x01,0X06,0X00,0X00,0x63,0,0,0,0,0,0,0,0,0,0,0,0,0xFF}; 

uint16 len=0;
unsigned char recv[255];
unsigned char LedDisplay1[]={0xAA,0x01,0xBB,0x51,0x54,0,0x01,0X06,0X00,0X00,0x63}; 

void KeyScan()
{
  if(SW1==0)
  {
    halMcuWaitUs(5);
    if(SW1==0)
    {
      while(!SW1);
      send[0]=0xCC;
      basicRfSendPacket(SEND_ADDR,send,1);
    }
  }
}

void LedDispaly_Format()
{
  len = len+11;
  for(int i=0;i<len;i++)
  {
    LedDisplay1[11+i] = recv[i];
  }
  for(int i=6;i<len;i++)
  {
    LedDisplay1[5] += LedDisplay1[i];
  }
  LedDisplay1[len]=0xff;
}


/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();
  halUartInit(9600);
  while(1)
  {
    /* user code start */
    halMcuWaitUs(10);
    len = halUartRead(recv,255);
    if(len>0)
    {
      if(recv[0]>=0xa0&&recv[0]<=0xaf)
      {
        //      
        switch(recv[0])
        {
        case 0xA0:D3=D4=D5=D6=0;break;  //0000
        case 0xA1:D4=1;D3=D5=D6=0;break;//0001
        case 0xA2:D3=1;D4=D5=D6=0;break;//0010
        case 0xA3:D3=D4=1;D5=D6=0;break;//0011
        case 0xA4:D6=1;D3=D4=D5=0;break;//0100
        case 0xA5:D6=D4=1;D5=D3=0;break;//0101
        case 0xA6:D3=D6=1;D4=D5=0;break;//0110
        case 0xA7:D3=D4=D6=1;D5=0;break;//0111
        case 0xA8:D5=1;D3=D4=D6=0;break;//1000
        case 0xA9:D5=D4=1;D3=D6=0;break;//1001
        case 0xAA:D5=D3=1;D4=D6=0;break;//1010
        case 0xAB:D5=D3=D4=1;D6=0;break;//1011
        case 0xAC:D5=D6=1;D3=D4=0;break;//1100
        case 0xAD:D5=D6=D4=1;D3=0;break;//1101
        case 0xAE:D5=D6=D3=1;D4=0;break;//1110
        case 0xAF:D5=D6=D4=D3=1;break;//1111
        case 0xCC:
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB7:
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:basicRfSendPacket(SEND_ADDR,recv,1);break;
        default:
          LedDispaly_Format();
          halUartWrite(LedDisplay1,len+1);
          basicRfSendPacket(SEND_ADDR,LedDisplay1,len+1);
          LedDisplay1[5]=0;
          break;
        }
      }
      else if(len>0)
      {
        LedDispaly_Format();
        halUartWrite(LedDisplay1,len+1);
        basicRfSendPacket(SEND_ADDR,LedDisplay1,len+1);
        LedDisplay1[5]=0;
      }
      /* user code end */
    }
  }
}