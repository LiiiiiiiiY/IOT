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

/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                11         // Ƶ�� 11~26
#define PAN_ID                    0x1111     //����id 
#define MY_ADDR                   0x2222     //����ģ���ַ
#define SEND_ADDR                 0x3333     //���͵�ַ

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
  halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
  ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
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
    if(len==1)//���볤��ֻ��һλʱ��ʮ�����ƣ�
    {
      D3=D4=D5=D6=0;
      halUartRead(dat,2);//��ȡ���ݲ��浽dat�У��Զ���ʮ������ת��ʮ���ƣ�
      num=dat[0];//dat[0]��ʮ���ƣ���ֵ��num��ʮ���ƣ�
      num_unit=num%16;//num->D2H(��λ)
      num_ten=num/16;//num->D2H(ʮλ)
      num=(num_ten*10)+num_unit;//����ת��ʮ������
      num_B=num%16;//numת16����ȡ�������֣���λ->B�壩
      num_A=num/16;//numת16����ȡ�������֣���λ->A�壩
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