#include "iocc2530.h"

#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4 
#define uint unsigned int
#define uchar unsigned char

uchar cmd = 0;//���ֽڱ��������ڽ�������
uchar F_recv = 0;//���ܵ�����ı�־����

void init_All()
{
  P1SEL &=~0x1b;
  P1DIR |= 0x1b;
  P1    &=~0x1b;
  
  P1SEL &=~0x04;
  P1DIR &=~0x04;
  P1INP &=~0x04;
  P2INP &=~0x40;
  
  //����IO����
  PERCFG |=~0X01;
  P0SEL  |= 0X0C;
  //���ò�����
  U0BAUD =59;
  U0GCR  =8;
  //����UART���ƼĴ���
  U0UCR |=0X80;
  //���ô��ڿ�����״̬�Ĵ���
  U0CSR |=0XC0;
  //ʹ���ж���ؿ���λ
  UTX0IF = 0;//U:���ڣ�TX:���ͣ�0:����0��IE:�жϱ�־λ
  URX0IF = 0;//U:���ڣ�RX:���գ�0:����0��IE:�жϱ�־λ
  URX0IE = 1;//U:���ڣ�RX:���գ�0:����0��IE:�жϿ���λ
  EA=1;
}

void Set_Clock_32M()
{
  CLKCONCMD &=~0x40;
  while(CLKCONSTA & 0x40);
  CLKCONCMD &=~0x07;
}

void UR0_sendByte(unsigned char dat)
{
  U0DBUF =dat;
  while(UTX0IF == 0);
  UTX0IF=0;
}

void UR0_sendString(unsigned char * str)
{
  while(*str != '\0')
  {
    UR0_sendByte(*str++);
  }
}

#pragma vector = URX0_VECTOR
__interrupt void Service_UR0_recv()
{
  cmd = U0DBUF;//���յ����������cmd����
  F_recv = 1;//�����������յ��µ�ָ����ڱ�־����
}

void Led_ctrl()
{
  switch(cmd)
  {
  case 0xD1:D4=1;UR0_sendString("�����ͬ��������\r\n");break;
  case 0xD2:D4=0;UR0_sendString("����ѽ��������\r\n");break;
  case 0xE1:D3=1;UR0_sendString("�̵���ͬ��������\r\n");break;
  case 0xE2:D3=0;UR0_sendString("�̵��ѽ��������\r\n");break;
  case 0xF1:D6=1;UR0_sendString("���˴���ƣ�\r\n");break;
  case 0xF2:D6=0;UR0_sendString("������������\r\n");break;
  case 0xAA:D4=D3=D6=1;UR0_sendString("��ơ��̵ơ���������ͬ��������\r\n");break;
  case 0x00:D4=D3=D6=0;UR0_sendString("��ơ��̵ơ���������ͬ���رգ�\r\n");break;
  default:UR0_sendString("����������������룡\r\n");break;
  }
}

void main()
{
  Set_Clock_32M();
  init_All();
  while(1)
  {
    if(F_recv == 1)
    {
      F_recv = 0;
      Led_ctrl();
    }
  }
}