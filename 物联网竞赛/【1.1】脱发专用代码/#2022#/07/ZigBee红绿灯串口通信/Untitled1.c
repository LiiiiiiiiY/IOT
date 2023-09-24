#include "iocc2530.h"

#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4 
#define uint unsigned int
#define uchar unsigned char

uchar cmd = 0;//单字节变量，用于接收命令
uchar F_recv = 0;//接受到命令的标志变量

void init_All()
{
  P1SEL &=~0x1b;
  P1DIR |= 0x1b;
  P1    &=~0x1b;
  
  P1SEL &=~0x04;
  P1DIR &=~0x04;
  P1INP &=~0x04;
  P2INP &=~0x40;
  
  //配置IO引脚
  PERCFG |=~0X01;
  P0SEL  |= 0X0C;
  //设置波特率
  U0BAUD =59;
  U0GCR  =8;
  //设置UART控制寄存器
  U0UCR |=0X80;
  //设置串口控制与状态寄存器
  U0CSR |=0XC0;
  //使能中断相关控制位
  UTX0IF = 0;//U:串口，TX:发送，0:串口0，IE:中断标志位
  URX0IF = 0;//U:串口，RX:接收，0:串口0，IE:中断标志位
  URX0IE = 1;//U:串口，RX:接收，0:串口0，IE:中断控制位
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
  cmd = U0DBUF;//接收到的命令放在cmd里面
  F_recv = 1;//告诉主函数收到新的指令，放在标志变量
}

void Led_ctrl()
{
  switch(cmd)
  {
  case 0xD1:D4=1;UR0_sendString("红灯已同步开启！\r\n");break;
  case 0xD2:D4=0;UR0_sendString("红灯已解除警报！\r\n");break;
  case 0xE1:D3=1;UR0_sendString("绿灯已同步开启！\r\n");break;
  case 0xE2:D3=0;UR0_sendString("绿灯已解除警报！\r\n");break;
  case 0xF1:D6=1;UR0_sendString("行人闯红灯！\r\n");break;
  case 0xF2:D6=0;UR0_sendString("行人已正常！\r\n");break;
  case 0xAA:D4=D3=D6=1;UR0_sendString("红灯、绿灯、报警灯已同步开启！\r\n");break;
  case 0x00:D4=D3=D6=0;UR0_sendString("红灯、绿灯、报警灯已同步关闭！\r\n");break;
  default:UR0_sendString("输入错误，请重新输入！\r\n");break;
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