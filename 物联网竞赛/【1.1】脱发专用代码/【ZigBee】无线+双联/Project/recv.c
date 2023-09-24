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

/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                11         // Ƶ�� 11~26
#define PAN_ID                    0x1111     //����id 
#define MY_ADDR                   0x3333     //����ģ���ַ
#define SEND_ADDR                 0x2222     //���͵�ַ

/********�������ݻ���********/


/**************************************************/
static basicRfCfg_t basicRfConfig;
// ����RF��ʼ��
void ConfigRf_Init(void)
{
  
    basicRfConfig.panId       =   PAN_ID;
    basicRfConfig.channel     =   RF_CHANNEL;
    basicRfConfig.myAddr      =   MY_ADDR;
    basicRfConfig.ackRequest  =   TRUE;
    while(basicRfInit(&basicRfConfig) == FAILED);
    basicRfReceiveOn();
 
}

void inti()
{
  P1SEL &=~0x9f;
  P1DIR |= 0x9b;
  P1&=~0x9b;
  P1DIR &=~0x04;
  P1INP &=~0x04;
  P2INP &=~0x40;
}

/********************MAIN************************/
void main(void)
{
  halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
  ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
  inti();
  uint8 recv[1];
  while(1)
  {
  /* user code start */
    if(basicRfPacketIsReady())
    {
      basicRfReceive(recv,1,NULL);
    }
    if(recv[0]==1)
    {
      P1_7=1;
    }
    else 
    {
      P1_7=0;
    }
      
  /* user code end */
  }
}