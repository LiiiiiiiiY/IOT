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

/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x1255     //本机模块地址
#define SEND_ADDR                 0x1244     //发送地址

/********无线数据缓存********/
void Init()
{
  P1SEL &=~0x1f;
  P1DIR |= 0x1b;
  P1DIR &=~0x04;
  P1&=~0x1b;
  P1INP&=~0x04;
  P2INP &=~0x40;
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


/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();
  while(1)
  {
  /* user code start */
    if(P1_2==0)
    {
      halMcuWaitUs(500);
      if(P1_2==0)
      {
        while(!P1_2)
        {
          P1_0=1;P1_1=0;
        }
        P1_0=P1_1=1;
      }
    }
  /* user code end */
  }
}