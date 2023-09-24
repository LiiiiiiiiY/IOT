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

#define D3 P1_0 //01
#define D4 P1_1 //02
#define D5 P1_3 //08
#define D6 P1_4 //10
#define JD1 P1_7//80
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x3333     //本机模块地址
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

void init_Port()
{
  P1SEL &=~0X9B;
  P1DIR |= 0X9B;
  P1    &=~0X9B;
}
unsigned char recv[1];
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  init_Port();
  while(1)
  {
    /* user code start */
    if(basicRfPacketIsReady())
    {
      basicRfReceive(recv,1,NULL);
      switch(recv[0])
      {
      case 0x00:JD1=0;break;
      case 0x01:JD1=1;break;
      case 0xB0:D3=D4=D5=D6=0;break;
      case 0xB1:D4=1;D3=D5=D6=0;break;
      case 0xB2:D3=1;D4=D5=D6=0;break;
      case 0xB3:D5=D6=0;D3=D4=1;break;
      case 0xB4:D6=1;D4=D5=D3=0;break;
      case 0xB5:D3=D5=0;D4=D6=1;break;
      case 0xB6:D3=D6=1;D4=D5=0;break;
      case 0xB7:D3=D4=D6=1;D5=0;break;
      case 0xB8:D5=1;D3=D4=D6=0;break;
      case 0xB9:D4=D5=1;D3=D6=0;break;
      case 0xBA:D4=D6=0;D3=D5=1;break;
      case 0xBB:D3=D4=D5=1;D6=0;break;
      case 0xBC:D3=D4=0;D5=D6=1;break;
      case 0xBD:D6=D4=D5=1;D3=0;break;
      case 0xBE:D3=D5=D6=1;D4=0;break;
      case 0xBF:D3=D4=D5=D6=1;break;
      default:halUartWrite(recv,1);break;
      }
    }
    /* user code end */
  }
}