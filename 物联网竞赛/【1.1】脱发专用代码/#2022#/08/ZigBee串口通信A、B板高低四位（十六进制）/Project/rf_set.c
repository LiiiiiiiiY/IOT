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

#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4
#define uchar unsigned char

void init_Port()
{
  MCU_IO_DIR_OUTPUT(1,0);
  MCU_IO_DIR_OUTPUT(1,1);
  MCU_IO_DIR_OUTPUT(1,3);
  MCU_IO_DIR_OUTPUT(1,4);
  D3=D4=D5=D6=0;  
}

uint8 len;
uint8 dat[2];
uchar Recv;
uchar flag;
uchar num;
uchar num_unit;
uchar num_ten;
uchar num_A;
uchar num_B;

/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  init_Port();
  D5=1;
  halMcuWaitMs(2000);
  D5=0;
  basicRfSendPacket(SEND_ADDR,"G",1);
  while(1)
  {
    /* user code start */
    len = halUartRxLen();
    while(!len)
    {
      len = halUartRxLen();
      if(basicRfPacketIsReady())
      {
        basicRfReceive(&Recv,1,NULL);
        if(Recv=='G')
        {
          basicRfSendPacket(SEND_ADDR,"H",1);
          D3=D4=D5=0;D6=1;
          halMcuWaitMs(2000);
          D3=D4=D5=D6=0;
        }
        else if(Recv=='H')
        {
          D3=D4=D5=0;D6=1;
          halMcuWaitMs(2000);
          D3=D4=D5=D6=0;
        }
      }
    }    
    if(len==2)
    {
      halUartRead(dat,2);
      D4=D5=D6=0;
      num_B=4;
      num_A=6;
    }
    if(len==1)//输入长度只有一位时（十六进制）
    {
      D3=D4=D5=D6=0;
      halUartRead(dat,2);//读取数据并存到dat中（自动将十六进制转成十进制）
      num=dat[0];//dat[0]（十进制）赋值给num（十进制）
      num_unit=num%16;//num->D2H(个位)
      num_ten=num/16;//num->D2H(十位)
      num=(num_ten*10)+num_unit;//最终转成十六进制
      num_B=num%16;//num转16进制取余数部分（低位->B板）
      num_A=num/16;//num转16进制取整数部分（高位->A板）
    }
    basicRfSendPacket(SEND_ADDR,&num_A,1);
    switch(num_B)
    {
    case 0:break;
    case 1:D5=1;break;
    case 2:D6=1;break;
    case 3:D5=D6=1;break;
    case 4:D3=1;break;
    case 5:D3=D5=1;break;
    case 6:D3=D6=1;break;
    case 7:D3=D5=D6=1;break;
    case 8:D4=1;break;
    case 9:D4=D5=1;break;
    case 10:D4=D6=1;break;
    case 11:D4=D5=D6=1;break;
    case 12:D4=D3=1;break;
    case 13:D4=D3=D5=1;break;
    case 14:D4=D3=D6=1;break;
    case 15:D4=D3=D5=D6=1;break;
    }
    /* user code end */
  }
}