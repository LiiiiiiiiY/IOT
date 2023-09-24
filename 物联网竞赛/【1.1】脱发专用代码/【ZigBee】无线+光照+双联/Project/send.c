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

/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                11         // Ƶ�� 11~26
#define PAN_ID                    0x1111     //����id 
#define MY_ADDR                   0x2222     //����ģ���ַ
#define SEND_ADDR                 0x3333     //���͵�ַ

/********�������ݻ���********/

void init()
{
  P1SEL &=~0x9b;
  P1DIR |= 0x9b;
  P1&=~0x9b;
}
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
uint16 value;
float light;
int len;
/********************MAIN************************/
void main(void)
{
  halBoardInit();//ѡ�ֲ����ڴ˺�������Ӵ���
  ConfigRf_Init();//ѡ�ֲ����ڴ˺�������Ӵ���
  hal_adc_Init();
  init();
  while(1)
  {
    /* user code start */
    value = get_adc();//��ȡ��ѹֵ
    light = 1.0*value;//ת��Ϊ���ղ���Ϊ��������
    uint8 senddata[1];//senddata�Ǵ�����߷�������
    
    uint8 str[10];//str��Ÿ�ʽ����Ĺ���ֵ�����ⲻ�ã�
    len = sprintf(str,"%.2f",light);//����ֵ������λС������str�����ⲻ�ã�
    halUartWrite(str,len);//�����ڷ������յĹ���ֵ�����ⲻ�ã�
    halMcuWaitMs(1000);//����ʱ��Ϊ�˷���Ӵ������ֲ鿴��Ϣ�����ⲻ�ã�
    //����������Ϊ�˷���Ӵ������ֲ鿴����ֵ���Ӷ�ͨ������ƽ��ֵ������������ж�
    
    if(light<50)//�������ס
    {
      senddata[0]=1;
    }
    else //û�б���ס
    {
      senddata[0]=0;
    }
    basicRfSendPacket(SEND_ADDR,senddata,len);//���޷�����Ϣ
    
    
    /* user code end */
  }
}