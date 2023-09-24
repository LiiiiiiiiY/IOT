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
#define JD1 P1_7//������
#define JD2 P2_0//��ʾ��
#define SW1 P1_2
#define SW2 P0_1
/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                11         // Ƶ�� 11~26
#define PAN_ID                    0x1111     //����id 
#define MY_ADDR                   0x3333     //����ģ���ַ
#define SEND_ADDR                 0x2222     //���͵�ַ
/********�������ݻ���********/
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

void Init()
{
  P1SEL &=~0x9B;
  P1DIR |= 0x9B;
  P1&=~0x9B;
  
  P2SEL &=~0x01;
  P2DIR |= 0x01;
  P2&=~0x01;
  
  IEN2 |=0X10;
  IEN1 |=0X20;
  P1IEN |=0X04;
  P0IEN |=0X02;
  PICTL |=0X03;
  EA=1;
}

#pragma vector = P1INT_VECTOR
__interrupt void Key1Pressend(void)
{
  if(P1IFG &0x04)
  {
    JD1=1;
    halMcuWaitMs(5000);
    JD1=0;
    P1IFG&=~0x04;
  }
  P1IF = 0;
}

#pragma vector = P0INT_VECTOR
__interrupt void P0_INT()
{
  if(P0IFG &0x02)
  {
    JD2=1;
    halMcuWaitMs(5000);
    JD2=0;
    P0IFG&=~0x02;
  }
  P0IF = 0;
}


/*FF 55 00 00 03 0A 07 BB 00 22 00 00 22 7E C7 C0*///������ѯ����
/*E2 80 68 94 00 00 40 16 8D 42 48 55*///��Ȩ�ı�ǩ��ȫ������
uint8 LunXun[16]={0xff,0x55,0x00,0x00,0x03,0x0a,0x07,0xbb,0x00,0x22,0x00,0x00,0x22,0x7e,0xc7,0xc0};
uint8 RecvData[12],URecv[255];
int flag = 0;
/********************MAIN************************/
void main(void)
{
  halBoardInit();
  ConfigRf_Init();
  Init();
  while(1)
  {
    halUartWrite(LunXun,17);//���͵��ʲ�ѯ�����������UHF���ᷢ�ͱ�ǩ�����
    halMcuWaitMs(500);
    int Len = halUartRead(URecv,255);
    if(Len>1)
    {
      for(int i=0;i<13;i++)//��ȡ��ǩ������
      {
        RecvData[i] = URecv[15+i];
      }
      if(RecvData[0]!=0xFD)//��ȡ�����ĵ�һλ�����FD����û��ʶ�𵽱�ǩ��UHF��û�б�ǩ��
      {
        if(flag!=(RecvData[11]+RecvData[10]+RecvData[9]+RecvData[8]))//��λ����һ���ǿ��ǩ��ʶ��׼ȷ�ȣ���֤ͬһ�ű�ǩ�벻�����ڶ��γ���
        {
          if(RecvData[8]==0x8d&&RecvData[9]==0x42&&RecvData[10]==0x48&&RecvData[11]==0x54)//Ա��A����Ȩ��ǩ�벿������
          {
            D5=D6=JD2=0;
            D3=D4=JD1=1;
            halMcuWaitMs(5000);
            JD1=JD2=0;
          }
          else if(RecvData[8]==0x43&&RecvData[9]==0x55&&RecvData[10]==0x51&&RecvData[11]==0x2e)//Ա��B����Ȩ��ǩ�벿������
          {
            D3=D4=JD2=0;
            D5=D6=JD1=1;
            halMcuWaitMs(5000);
            JD1=JD2=0;
          }
          else//û����Ȩ�ı�ǩ��
          {
            D3=D4=D5=D6=JD1=0;
            JD2=1;
            halMcuWaitMs(5000);
            JD2=0;
          }
          flag = RecvData[11]+RecvData[10]+RecvData[9]+RecvData[8];
        }
      }
    }
  }
}