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

/*****��Ե�ͨѶ��ַ����******/


#define RF_CHANNEL                11         // Ƶ�� 11~26
#define PAN_ID                    0x1212     //����id 
#define MY_ADDR                   0x2222     //����ģ���ַ
#define SEND_ADDR                 0x1111     //���͵�ַ

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

/*������ѯ����*/
uint8 DanCiLunXun[16]={0xff,0x55,0x00,0x00,0x03,0x0a,0x07,0xbb,0x00,0x22,0x00,0x00,0x22,0x7e,0xc7,0xc0};

/*���ڽ��ջ��棬���ڴ��UHF���ص�����*/
uint8 UartRecv[255];

/*������ȡEPC����*/
uint8 EPC[12];

/*�����ж�UHFʶ��ı�ǩ�Ƿ��ص�*/
uint8 EPC_FLAG=0x00;

/*������ģ��A���޷�������*/
uint8 RFSend[1]={0xab};
/********************MAIN************************/
void main(void)
{
  halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
  ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
  Init();//��ʼ���ĸ�LED�ƺ�SW1
  D5=1;
  halMcuWaitMs(2000);
  D5=0;//��ʼ��D5��������
  basicRfSendPacket(SEND_ADDR,RFSend,1);//��Э����ģ��A����һ����Ϣ����ʾ�����ɹ�
  halMcuWaitMs(5);
  D6=1;
  halMcuWaitMs(2000);
  D6=0;//�����ɹ���D6��������
  while(1)
  {
    /* user code start */
    halUartWrite(DanCiLunXun,16);//ÿһ���UHF���͵�����ѯ����
    halMcuWaitMs(1000);
    int len = halUartRead(UartRecv,255);//���ڽ���UHF���ݣ������UartRecv��
    if(len>0)//��UHF�����ݷ�����
    {
      /*������������һ�������γ��������*/
      
      /*���һ��û��ʶ�𵽳���Ƶ��*/
      if(UartRecv[8]==0x01)
      {
        EPC_FLAG = 0xFF;
        RFSend[0] = 0x01;
        basicRfSendPacket(SEND_ADDR,RFSend,1);
      }
      
      /*�������ʶ���п�Ƭ*/
      
          /*ʶ�𵽿�Ƭ���Էֳ��������*/
      else if(UartRecv[8]==0x02)
      {
        /*��ȡEPC��ֵ*/
        for(int i=0;i<13;i++)
        {
          EPC[i] = UartRecv[15+i];
        }
        /*����ȡ�Ŀ�Ƭ����һ�ſ�Ƭ����ͬһ��ʱ*/
        if(EPC_FLAG!=EPC[11])
        {
          /*���1����ȡ��Ա����A*/
          if(EPC[11]==0x4A)
          {
            D3=D4=1;
            RFSend[0] = 0x0A;
            basicRfSendPacket(SEND_ADDR,RFSend,1);
            halMcuWaitMs(2000);
            P1&=~0x1b;
          }
          
          /*���2����ȡ��Ա����A*/
          else if(EPC[11]==0x47)
          {
            D5=D6=1;
            RFSend[0] = 0x0B;
            basicRfSendPacket(SEND_ADDR,RFSend,1);
            halMcuWaitMs(2000);
            P1&=~0x1b;
          }
          
          /*���3��ʶ��δ¼��Ŀ�Ƭ*/
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
          /*���¶�ȡ�Ŀ�Ƭ�������ֵ�ֵ��Ž���־λ��*/
          EPC_FLAG = EPC[11];
        }
      }
      /*���EPC��ֵ*/
      for(int i=0;i<13;i++)
      {
        EPC[i] = 0x00;
      }
    }
    /* user code end */
  }
}