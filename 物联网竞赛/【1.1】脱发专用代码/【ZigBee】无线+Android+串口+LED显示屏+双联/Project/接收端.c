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
#define MY_ADDR                   0x3333     //本机模块地址
#define SEND_ADDR                 0x2222     //发送地址
/********无线数据缓存********/
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
unsigned char recv[255];
char hz[]="AB欢迎光临12";
int a=0;
uint8 len;
unsigned char HU[]={0xAA,0x01,0xBB,0x51,0x54,0,0x01,0X06,0X00,0X00,0x63}; 
void key_welcome()
{
  if(SW2==0)
  {
    while(!SW2);
    len = 11+12;    
    for(int i=0;i<23;i++)
    {
      HU[i+11] = hz[i];
    }
    for(int i=6;i<23;i++)
    {
      HU[5]+=HU[i];
    }
    HU[len]=0xff;
    halUartWrite(HU,24);
    HU[5] = 0;
  }
}

/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  Init();
  halUartInit(9600);
  while(1)
  {
    /* user code start */
    key_welcome();
    if(basicRfPacketIsReady())
    {
      halMcuWaitUs(10);
      len=basicRfReceive(recv,255,NULL);
      if(len>0)
      {
        switch(recv[0])
        {
        case 0xB0:D3=D4=D5=D6=0;break;  //0000
        case 0xB1:D4=1;D3=D5=D6=0;break;//0001
        case 0xB2:D3=1;D4=D5=D6=0;break;//0010
        case 0xB3:D3=D4=1;D5=D6=0;break;//0011
        case 0xB4:D6=1;D3=D4=D5=0;break;//0100
        case 0xB5:D6=D4=1;D5=D3=0;break;//0101
        case 0xB6:D3=D6=1;D4=D5=0;break;//0110
        case 0xB7:D3=D4=D6=1;D5=0;break;//0111
        case 0xB8:D5=1;D3=D4=D6=0;break;//1000
        case 0xB9:D5=D4=1;D3=D6=0;break;//1001
        case 0xBA:D5=D3=1;D4=D6=0;break;//1010
        case 0xBB:D5=D3=D4=1;D6=0;break;//1011
        case 0xBC:D5=D6=1;D3=D4=0;break;//1100
        case 0xBD:D5=D6=D4=1;D3=0;break;//1101
        case 0xBE:D5=D6=D3=1;D4=0;break;//1110
        case 0xBF:D5=D6=D4=D3=1;break;//1111
        case 0xCC:JD1^=1;;break;
        default:
          halUartWrite(recv,1);
          break;
        }
      }
    }
    /* user code end */
  }
}