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
#define JD1 P1_7
#define SW1 P1_2
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x3333     //本机模块地址
#define SEND_ADDR                 0x2222     //发送地址

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
unsigned char RFrecv[255];
uint8 Send[]={0xaa,0x01,0xbb,0x51,0x54,0,0x08,0x00,0x00,0x02,0x63};
int len;

void init()
{
  P1SEL &=~0x9b;
  P1DIR |= 0x9b;
  P1&=~0x9b;
  
  P1SEL &=~0x04;
  P1DIR &=~0x04;
  P1INP &=~0x04;
  P2INP &=~0x04;
}

void Jisuan()
{
  len = len+11;
  for(int i=0;i<len;i++)
  {
    Send[i+11] = RFrecv[i];
  }
 // halUartWrite(Send,len);
  for(int i=6;i<len;i++)
  {
    Send[5] +=Send[i];
  }
  Send[len] = 0xff;
}
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  init();
  while(1)
  {
    /* user code start */
    if(basicRfPacketIsReady())
    {
      len = basicRfReceive(RFrecv,255,NULL);
      //halUartWrite(RFrecv,len);
      if(len>0)
      {
        if(len==1)
        {
          switch(RFrecv[0])
          {
          case 0xb0:D3=D4=D5=D6=0;break;
          case 0xb1:D4=1;D3=D5=D6=0;break;
          case 0xb2:D3=1;D4=D5=D6=0;break;
          case 0xb3:D3=D4=1;D5=D6=0;break;
          case 0xb4:D6=1;D3=D4=D5=0;break;
          case 0xb5:D6=D4=1;D3=D5=0;break;
          case 0xb6:D6=D3=1;D4=D5=0;break;
          case 0xb7:D3=D4=D6=1;D5=0;break;
          case 0xb8:D5=1;D3=D4=D6=0;break;
          case 0xb9:D5=D4=1;D3=D6=0;break;
          case 0xba:D5=D3=1;D4=D6=0;break;
          case 0xbb:D5=D3=D4=1;D6=0;break;
          case 0xbc:D5=D6=1;D3=D4=0;break;
          case 0xbd:D5=D6=D4=1;D3=0;break;
          case 0xbe:D5=D6=D3=1;D4=0;break;
          case 0xbf:D3=D4=D5=D6=1;break;
          case 0xcc:JD1^=1;break;
          default:
            Jisuan();
            halUartWrite(Send,len+1);
            Send[5]=0;
            break;
          }
        }
        else
        {
          halUartWrite(RFrecv,len);
        }
      }
    }
    /* user code end */
  }
}