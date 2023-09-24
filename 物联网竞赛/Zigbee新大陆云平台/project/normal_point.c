#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_adc.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h"
#include "TIMER.h"
#include "get_adc.h"
#include "sh10.h"
#include "UART_PRINT.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
//LED
#define LED1 P1_0
#define LED2 P1_1
//KEY
#define KEY_UP P0_1
#define KEY_DOWN P2_0
/*BasicRF�������*/
#define PAN_ID  0x2007
#define CHANNEL  25
#define SEND_ADDR 0x1234
#define MY_ADDR 0xBDCC
/*�����С*/
#define MAX_SEND_BUF_LEN  128 //������������ͳ���
#define MAX_RECV_BUF_LEN  128 //�������������ճ���
/*����*/
static basicRfCfg_t basicRfCfg; 
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
static uint8 count=0;
//�����ʼ��

//BasicRF��ʼ��
void baiscrf_init()
{
    //BasicRFͨ����س�ʼ��
    basicRfCfg.panId=PAN_ID;
    basicRfCfg.channel=CHANNEL;
    basicRfCfg.myAddr=MY_ADDR;
    basicRfCfg.ackRequest=TRUE;
    while(basicRfInit(&basicRfCfg)==FAILED);
    
}
void led_init()
{
  //#define LED1 P1_0
  //#define LED2 P1_1
  P1SEL&=~0x03;
  P1DIR|=0x03;
  LED1=0;
  LED2=0;
}
void key_init()
{
   //#define KEY_UP P0_1
    P0SEL&=~0x02;//GPIO
    P0DIR&=~0x02;//����
    P0INP&=~0x02;//����/����
    IEN1|=0x20;//P0���ж�ʹ��
    P0IEN|=0x02;//�ж�ʹ��P0_1
    PICTL|=0x01;//�½��ش����ж�
   //#define KEY_DOWN P2_0
    P2SEL&=~0x01;//GPIO
    P2DIR&=~0x01;//����
    P2INP&=~0x01;//����/����
    IEN2|=0x02;//P2���ж�ʹ��
    P2IEN|=0x01;//�ж�ʹ��P2_0
    PICTL|=0x08;//�½��ش����ж�
    EA=1;//�������ж�
}
void main()
{
    //ģ����Դ��ʼ��
    halBoardInit();
    //BasicRF��ʼ��
    baiscrf_init();
    led_init();
    key_init();
    while(1);
}
// KEY_UP P0_1
#pragma vector=P0INT_VECTOR
__interrupt void P0_ISR()
{
  if(P0IFG&0x02)
  {
    //������ز���
    LED1=~LED1;
    count++;
    pTxData[0]=count;
    basicRfSendPacket(SEND_ADDR,pTxData,1);
  }
  P0IFG=0x00;
  P0IF=0x00;
  
    
}
// KEY_DOWN P2_0
#pragma vector=P2INT_VECTOR
__interrupt void P2_ISR()
{
  if(P2IFG&0x01)
  {
    //������ز���
    LED2=~LED2;
    count--;
    if(count>=0)
    {
      pTxData[0]=count;
      basicRfSendPacket(SEND_ADDR,pTxData,1);
    }
  }
  P2IF=0;
  P2IFG=0;
}