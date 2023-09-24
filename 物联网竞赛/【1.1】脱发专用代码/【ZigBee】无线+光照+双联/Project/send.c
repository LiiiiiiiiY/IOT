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
#include "sensor_drv\get_adc.h"

/*****点对点通讯地址设置******/
#define RF_CHANNEL                11         // 频道 11~26
#define PAN_ID                    0x1111     //网络id 
#define MY_ADDR                   0x2222     //本机模块地址
#define SEND_ADDR                 0x3333     //发送地址

/********无线数据缓存********/

void init()
{
  P1SEL &=~0x9b;
  P1DIR |= 0x9b;
  P1&=~0x9b;
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
uint16 value;
float light;
int len;
/********************MAIN************************/
void main(void)
{
  halBoardInit();//选手不得在此函数内添加代码
  ConfigRf_Init();//选手不得在此函数内添加代码
  hal_adc_Init();
  init();
  while(1)
  {
    /* user code start */
    value = get_adc();//获取电压值
    light = 1.0*value;//转换为光照并且为浮点类型
    uint8 senddata[1];//senddata是存放无线发送内容
    
    uint8 str[10];//str存放格式化后的光照值（此题不用）
    len = sprintf(str,"%.2f",light);//光照值保留两位小数赋给str（此题不用）
    halUartWrite(str,len);//给串口发送最终的光照值（此题不用）
    halMcuWaitMs(1000);//加延时是为了方便从串口助手查看信息（此题不用）
    //上面四行是为了方便从串口助手查看光照值，从而通过光照平均值来进行下面的判断
    
    if(light<50)//如果被遮住
    {
      senddata[0]=1;
    }
    else //没有被遮住
    {
      senddata[0]=0;
    }
    basicRfSendPacket(SEND_ADDR,senddata,len);//无限发送信息
    
    
    /* user code end */
  }
}