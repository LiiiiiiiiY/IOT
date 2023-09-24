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
#define SW1 P1_2

void Init()
{
  P1SEL &=~0x1b;
  P1DIR |= 0x1b;
  P1&=~0x1b;
  
  P1SEL &=~0x04;
  P1DIR &=~0x04;
  P1INP &=~0x04;
  P2INP &=~0x40;
}

/*****点对点通讯地址设置******/


#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1212     //网络id 
#define MY_ADDR                   0x1111     //本机模块地址
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


uint8 DanCiLunXun[16]={0xff,0x55,0x00,0x00,0x03,0x0a,0x07,0xbb,0x00,0x22,0x00,0x00,0x22,0x7e,0xc7,0xc0};
uint8 RFRecv[255],RFSend[1]={0xab};
uint8 staff_A_B[1];
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();//初始化四个LED灯和SW1
  D5=1;
  halMcuWaitMs(2000);
  D5=0;
  while(1)
  {
    /* user code start */
    if(basicRfPacketIsReady())
    {
      int len = basicRfReceive(RFRecv,255,NULL);
      if(len>0)
      {
        if(RFRecv[0]==0xab)//当有新的zigbee组网时
        {
          basicRfSendPacket(SEND_ADDR,RFSend,1);
          D6=1;
          halMcuWaitMs(2000);
          D6=0;
        }
        else if((RFRecv[0]==0x01)||(RFRecv[0]==0x0A)||(RFRecv[0]==0x0B)||(RFRecv[0]==0x0E))//当UHF获取到新数据
        {
          if((RFRecv[0]==0x0A)||(RFRecv[0]==0x0B))
          {
            staff_A_B[0] = RFRecv[0];
          }
          halUartWrite(RFRecv,1);
        }
      }
    }
    if(SW1==0)
    {
      halMcuWaitMs(5);
      if(SW1==0)
      {
        while(!SW1);
        halUartWrite(staff_A_B,1);
      }
    }
    
    
    /* user code end */
  }
}