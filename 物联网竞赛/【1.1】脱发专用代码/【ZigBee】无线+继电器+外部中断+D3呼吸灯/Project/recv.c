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
#define JD2 P2_0
#define SW1 P1_2

/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x1255     //本机模块地址
#define SEND_ADDR                 0x1244     //发送地址

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
uint8 len=0,h=0,a=1,flag[4],RFrecv[255];


void Init()
{
  P1SEL &=~0x9b;
  P1DIR |= 0x9b;
  P1&=~0x9b;
  
  P2SEL &=~0x01;
  P2DIR |= 0x01;
  P2&=~0x01;
  
  P1SEL &=~0x04;
  P1DIR &=~0x04;
  P1INP &=~0x04;
  P2INP &=~0x40;
  
  IEN2|=0x10;
  P1IEN|=0x04;
  PICTL|=0x02;
  EA=1;
}
void T1Init()
{
  //P1SEL|=0x01;
  PERCFG =0x40;
  T1CTL |=0x01;
  T1CCTL2=0x64;
  T1CC2L=0xff;
  T1CC2H =h;
}

void breath()
{
  if((T1STAT&0x04)>0)
  {
    T1STAT=T1STAT&0xfb;
    
    if(a==1)h++;
    else h--;
    
    T1CC2L =0xff;
    T1CC2H = h;
    
    if(h>=254)a=2;
    if(h==0)a=1;
  }
}

#pragma vector=P1INT_VECTOR
__interrupt void KEY()
{
  if(P1IFG&0x04)
  {
    flag[3]^=1;
    P1IFG&=~0x04;
  }
  P1IF=0;
}

/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();
  T1Init();
  while(1)
  {
    /* user code start */
    if(basicRfPacketIsReady())
    {
      len = basicRfReceive(RFrecv,255,NULL);
    }
    if(len>0)
    {
      if(RFrecv[3]==0)JD1=0;
      else if(RFrecv[3]==1)JD1=1;
    }
    
    if(flag[3]==0)
    {
      P1SEL &=~0x01;
      JD2=D3=0;
    }
    else if(flag[3]==1)
    {
      P1SEL |=0x01;
      breath();
      JD2=1;
    }
    /* user code end */
  }
}