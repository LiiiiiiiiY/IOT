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

#define JD1 P1_7
#define JD2 P2_0

/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x3333     //发送地址
/********无线数据缓存********/
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

void Init()
{
  P1SEL &=~0x9B;
  P1DIR |= 0x9B;
  P1&=~0x9B;
  
  P2SEL &=~0x01;
  P2DIR |= 0x01;
  P2&=~0x01;
}
/*FF 55 00 00 03 0A 07 BB 00 22 00 00 22 7E C7 C0*///单次轮询
/*FF 55 00 00 03 0A 0A BB 00 27 00 03 22 00 05 51 7E 35 1C*///多次轮询
/*E2 80 68 94 00 00 40 16 8D 42 48 55*/
uint8 RFRecv[255];
int count=0;
/********************MAIN************************/
void main(void)
{
  halBoardInit();
  ConfigRf_Init();
  Init();
  while(1)
  {
    if(basicRfPacketIsReady())
    {
      int len = basicRfReceive(RFRecv,255,NULL);
      halUartWrite(RFRecv,len);
    }
  }
}