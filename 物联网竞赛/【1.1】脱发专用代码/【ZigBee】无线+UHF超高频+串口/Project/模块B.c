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
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x1111     //发送地址

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

/*单次轮询报文*/
uint8 DanCiLunXun[16]={0xff,0x55,0x00,0x00,0x03,0x0a,0x07,0xbb,0x00,0x22,0x00,0x00,0x22,0x7e,0xc7,0xc0};

/*串口接收缓存，用于存放UHF返回的数据*/
uint8 UartRecv[255];

/*用于提取EPC数据*/
uint8 EPC[12];

/*用于判断UHF识别的标签是否重叠*/
uint8 EPC_FLAG=0x00;

/*用于向模块A无限发送数据*/
uint8 RFSend[1]={0xab};
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();//初始化四个LED灯和SW1
  D5=1;
  halMcuWaitMs(2000);
  D5=0;//初始化D5亮两秒灭
  basicRfSendPacket(SEND_ADDR,RFSend,1);//给协调器模块A发送一个信息，表示组网成功
  halMcuWaitMs(5);
  D6=1;
  halMcuWaitMs(2000);
  D6=0;//组网成功后D6亮两秒灭
  while(1)
  {
    /* user code start */
    halUartWrite(DanCiLunXun,16);//每一秒给UHF发送单次轮询报文
    halMcuWaitMs(1000);
    int len = halUartRead(UartRecv,255);//串口接收UHF数据，存放在UartRecv中
    if(len>0)//当UHF有数据发过来
    {
      /*发过来的数据一共可以形成两种情况*/
      
      /*情况一：没有识别到超高频卡*/
      if(UartRecv[8]==0x01)
      {
        EPC_FLAG = 0xFF;
        RFSend[0] = 0x01;
        basicRfSendPacket(SEND_ADDR,RFSend,1);
      }
      
      /*情况二：识别有卡片*/
      
          /*识别到卡片可以分成三种情况*/
      else if(UartRecv[8]==0x02)
      {
        /*提取EPC的值*/
        for(int i=0;i<13;i++)
        {
          EPC[i] = UartRecv[15+i];
        }
        /*当读取的卡片与上一张卡片不是同一张时*/
        if(EPC_FLAG!=EPC[11])
        {
          /*情况1：读取到员工卡A*/
          if(EPC[11]==0x4A)
          {
            D3=D4=1;
            RFSend[0] = 0x0A;
            basicRfSendPacket(SEND_ADDR,RFSend,1);
            halMcuWaitMs(2000);
            P1&=~0x1b;
          }
          
          /*情况2：读取到员工卡A*/
          else if(EPC[11]==0x47)
          {
            D5=D6=1;
            RFSend[0] = 0x0B;
            basicRfSendPacket(SEND_ADDR,RFSend,1);
            halMcuWaitMs(2000);
            P1&=~0x1b;
          }
          
          /*情况3：识别到未录入的卡片*/
          else
          {
            RFSend[0] = 0x0E;
            basicRfSendPacket(SEND_ADDR,RFSend,1);
            for(int i=0;i<5;i++)
            {
              P1|= 0x1b;
              halMcuWaitMs(500);
              P1&=~0x1b;
              halMcuWaitMs(500);
            }
          }
          /*将新读取的卡片可以区分的值存放进标志位中*/
          EPC_FLAG = EPC[11];
        }
      }
      /*清楚EPC的值*/
      for(int i=0;i<13;i++)
      {
        EPC[i] = 0x00;
      }
    }
    /* user code end */
  }
}