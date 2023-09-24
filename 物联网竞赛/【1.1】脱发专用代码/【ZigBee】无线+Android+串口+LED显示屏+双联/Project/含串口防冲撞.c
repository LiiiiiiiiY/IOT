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
#include <string.h>


#define SW1 P1_2
#define SW2 P0_1
#define JD1 P1_7
#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4
/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x3333     //发送地址
/********无线数据缓存********/

#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小

#define MAX_UART_SEND_BUF_LEN  128
#define MAX_UART_RECV_BUF_LEN  128
uint8 uTxData[MAX_UART_SEND_BUF_LEN]; //定义串口发送缓冲区的大小
uint8 uRxData[MAX_UART_RECV_BUF_LEN]; //定义串口接收缓冲区的大小
uint16 uTxlen = 0;
uint16 uRxlen = 0;
void Init()
{
  P1SEL &=~0x9B;
  P1DIR |= 0x9B;
  D3 = D4 =D5 = D6 = JD1=0;
  P1INP &=~0x04;
  P2INP &=~0x40;
  
  P0SEL &=~0x02;
  P0DIR &=~0x02;
  P0INP &=~0x02;
  P2INP &=~0x20;
}

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
void MyByteCopy(uint8 *dst, int dststart, uint8 *src, int srcstart, int len)
{
    int i;
    for(i=0; i<len; i++)
    {
        *(dst+dststart+i)=*(src+srcstart+i);
    }
}
/****************************************************/
uint16 RecvUartData(void)
{   
    uint16 r_UartLen = 0;
    uint8 r_UartBuf[128]; 
    uRxlen=0; 
    r_UartLen = halUartRxLen();
    while(r_UartLen > 0)
    {
        r_UartLen = halUartRead(r_UartBuf, sizeof(r_UartBuf));
        MyByteCopy(uRxData, uRxlen, r_UartBuf, 0, r_UartLen);
        uRxlen += r_UartLen;
        halMcuWaitMs(5);   //这里的延迟非常重要，因为这是串口连续读取数据时候需要有一定的时间间隔
        r_UartLen = halUartRxLen();       
    }   
    return uRxlen;
}
unsigned char send[64],Urecv[255];
//unsigned char   HY[23]={0xAA,0X01,0XBB,0X51,0X54,0,0x08,0X06,0X99,0X00,0x63,0,0,0,0,0X3A,0,0,0,0,0, 0XFF};
unsigned char HU[23]={0xAA,0x01,0xBB,0x51,0x44,0xA1,0x08,0x00,0x99,0x00,0x41,0x42,0xBB,0xB6,0xD3,0xAD,0xB9,0xE2,0xC1,0xD9,0x31,0x32,0xFF};

uint8 sendDisplay_len;
unsigned char ledDisplay1[12]={0xAA,0x01,0xBB,0x51,0x54,0,0x01,0X06,0X00,0X00,0x63,0xff}; 
unsigned char Send_ledDisplay1[255]; 
unsigned char ledDisplay[25]={0xAA,0x01,0xBB,0x51,0x54,0,0x01,0X06,0X00,0X00,0x63,0,0,0,0,0,0,0,0,0,0,0,0,0xFF}; 

void KeyScan()
{
  if(SW1==0)
  {
    halMcuWaitUs(5);
    if(SW1==0)
    {
      while(!SW1);
      send[0]=0xcc;
      basicRfSendPacket(SEND_ADDR,send,1);
    }
  }
  if(SW2==0)
  {
    halMcuWaitUs(5);
    if(SW2==0)
    {
      while(!SW2);
      P1_0^=1;
      halUartWrite(HU,sizeof(HU));
      basicRfSendPacket(SEND_ADDR,HU,23);
    }
  }
}


/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();
  halUartInit(9600);
  uint16 len=0;
  while(1)
  {
    /* user code start */
    KeyScan();
   // len = halUartRxLen();
     halMcuWaitUs(10);
    len=RecvUartData();
    if(len>0)
    {
   //   halMcuWaitUs(10);
     // halUartRead(Urecv,255);
      ledDisplay[0]=len;
      halUartWrite(ledDisplay,1);
      
      halMcuWaitUs(5); 
      sendDisplay_len=12+len;
      Send_ledDisplay1[sendDisplay_len-1]=ledDisplay1[11];
      for(int i=0;i<11;i++)
      {
         Send_ledDisplay1[i]=ledDisplay1[i];
      }
    //  halUartWrite(Urecv,len);
    //  halUartWrite("/r/n",2);
      for(int i=0;i<len;i++)
      {
        Send_ledDisplay1[11+i]=uRxData[i];
      }
    for(int i=6;i<sendDisplay_len-1;i++)
    {
      Send_ledDisplay1[5]+=Send_ledDisplay1[i];
    } 
    halUartWrite(Send_ledDisplay1,sendDisplay_len);
    basicRfSendPacket(SEND_ADDR,Send_ledDisplay1,sendDisplay_len);
    //  halUartWrite(Urecv,len);
      
      
//      switch(Urecv[0])
//      {
//      case 0xA0:D3=D4=D5=D6=0;break;  //0000
//      case 0xA1:D4=1;D3=D5=D6=0;break;//0001
//      case 0xA2:D3=1;D4=D5=D6=0;break;//0010
//      case 0xA3:D3=D4=1;D5=D6=0;break;//0011
//      case 0xA4:D6=1;D3=D4=D5=0;break;//0100
//      case 0xA5:D6=D4=1;D5=D3=0;break;//0101
//      case 0xA6:D3=D6=1;D4=D5=0;break;//0110
//      case 0xA7:D3=D4=D6=1;D5=0;break;//0111
//      case 0xA8:D5=1;D3=D4=D6=0;break;//1000
//      case 0xA9:D5=D4=1;D3=D6=0;break;//1001
//      case 0xAA:D5=D3=1;D4=D6=0;break;//1010
//      case 0xAB:D5=D3=D4=1;D6=0;break;//1011
//      case 0xAC:D5=D6=1;D3=D4=0;break;//1100
//      case 0xAD:D5=D6=D4=1;D3=0;break;//1101
//      case 0xAE:D5=D6=D3=1;D4=0;break;//1110
//      case 0xAF:D5=D6=D4=D3=1;break;  //1111
      //case 0xCC:break;
//      default:basicRfSendPacket(SEND_ADDR,Send_ledDisplay1,sendDisplay_len);break;
//      }
      //basicRfSendPacket(SEND_ADDR,send,len);
    }
   // memset(send,0,sizeof(send));
    /* user code end */
  }
}