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
#define MY_ADDR                   0x1244     //����ģ���ַ
#define SEND_ADDR                 0x1255     //���͵�ַ
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
unsigned char h,a=1;
void init_DIng()
{
  P1SEL |=0x02;
  PERCFG = 0x40;
  T1CTL |=0x01;
  T1CCTL1 =0x64;
  T1CC1L =0xff;
  T1CC1H =h;
}

void breath()
{
  if((T1STAT&0x02)>0)
  {
    T1STAT=T1STAT&0xfd;
    if(a==1)h=h+1;
    else h=h-1;
    T1CC1L = 0xff;
    T1CC1H = h;
    if(h>=254)a=2;
    if(h==0)a=1;
  }
}


/********************MAIN************************/
void main(void)
{
  halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
  ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
  init_DIng();
  while(1)
  {
  /* user code start */
   breath();
  /* user code end */
  }
}