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
}
int len;
unsigned char recv[1]={0x00};
/********************MAIN************************/
void main(void)
{
    halBoardInit();//选手不得在此函数内添加代码
    ConfigRf_Init();//选手不得在此函数内添加代码
    init_Port();
    while(1)
    {
    /* user code start */
      len = halUartRxLen();
      if(len>0)
      {
        halUartRead(recv,1);
        switch(recv[0])
        {
        case 0x00:D3=D4=D5=D6=0;halUartWrite(recv,1);break;
        case 0x01:D5=1;D3=D4=D6=0;break;
        case 0x02:D6=1;D4=D5=D3=0;break;
        case 0x03:D5=D6=1;D3=D4=0;break;
        case 0x04:D3=1;D4=D5=D6=0;break;
        case 0x05:D3=D5=1;D4=D6=0;break;
        case 0x06:D3=D6=1;D4=D5=0;break;
        case 0x07:D3=D5=D6=1;D4=0;break;
        case 0x08:D4=1;D3=D5=D6=0;break;
        case 0x09:D4=D5=1;D3=D6=0;break;
        case 0x0A:D4=D6=1;D3=D5=0;break;
        case 0x0B:D4=D6=D5=1;D3=0;break;
        case 0x0C:D3=D4=1;D5=D6=0;break;
        case 0x0D:D3=D4=D5=1;D6=0;break;
        case 0x0E:D3=D4=D6=1;D5=0;break;
        case 0x0F:D3=D4=D5=D6=1;break;
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x40:
        case 0x50:
        case 0x60:
        case 0x70:
        case 0x80:
        case 0x90:
        case 0xA0:
        case 0xB0:
        case 0xC0:
        case 0xD0:
        case 0xE0:
        case 0xF0:basicRfSendPacket(SEND_ADDR,recv,1);break;
        }
      }
    /* user code end */
    }
}