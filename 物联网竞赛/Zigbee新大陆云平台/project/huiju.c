#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "hal_pwm.h" 
#include "UART_PRINT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
//LED
#define LED1 P1_0
#define LED2 P1_1
#define LED3 P0_4
/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                25         // Ƶ�� 11~26
#define PAN_ID                    0x2007     //����id
#define MY_ADDR                   0x1234    //����ģ���ַ
/********�������ݻ���********/
#define MAX_SEND_BUF_LEN  128
#define MAX_RECV_BUF_LEN  128
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
uint8 signal;
/***********����TCP/IP���ݵĻ�����************/
static uint8 TCP_DATA_BUF[255];
/**************************************************/
static basicRfCfg_t basicRfConfig;

// ����RF��ʼ��
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;        //zigbee��ID������
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee��Ƶ������
    basicRfConfig.myAddr      =   MY_ADDR;   //���ñ�����ַ
    basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                // ʹ��Basic_RF����
}
void led_init()
{
  //#define LED1 P1_0
  //#define LED2 P1_1
  //#define LED3  P0_4
  P1SEL&=~0x03;
  P1DIR|=0x03;
  P0DIR&=~0x10;
  P0DIR|=0x10;
  LED1=0;
  LED2=0;
  LED3=0;
}
void t1_init()
{
  T1IE=1;
  T1CTL=0x0A;//101032��Ƶ��ģ����ģʽ����0x0000~T1CCO(1000��������1ģʽ)
  T1CCTL0 |=0X04;
  T1CC0H=0x27;
  T1CC0L=0x0f;
  TIMIF|=0x40;
  EA=1;
  T1IF=0;

}
int count=0;
#pragma vector=T1_VECTOR
__interrupt void T1_ISR()
{
    IRCON=0x00;//����жϱ�־λ
    //10ms��ʱ��
    if(count++>=100)
    {
    count=0;
  //  LED1=!LED1;
    }
    if(USART1_RX_RECEIVING)
    {
        USART1_RX_TIMEOUT_COUNT++;
        if(USART1_RX_TIMEOUT_COUNT>=3)//����30ms����û�н��յ�����
        {
          UART1_RX_FINISH=1;
          USART1_RX_TIMEOUT_COUNT=0;
          USART1_RX_RECEIVING=0;
        }
    }
}
/********************MAIN START************************/
void main(void)
{
    uint16 len = 0;   
    halBoardInit();  //ģ�������Դ�ĳ�ʼ��
    ConfigRf_Init(); //�����շ����������ó�ʼ�� 
    led_init();
    halUartInit1(115200); //��ʼ������1�Ĳ�����Ϊ115200
    halUartInit0();//��ʼ������0��������Ϊ115200
    t1_init();
    char data[]="uart0 start working!";
    printf_uart0(data,strlen(data));
    uint8 TryCount=0;
    //��ʼESP8266���ӷ�����
    uint8 temp;
    for(TryCount=0;TryCount<3;TryCount++)
    {
        temp=ConnectToServer(MY_DEVICE_ID,MA_SECRET_KEY);
        if(temp != 0)
	{
	     //printf("Connect To Server ERROR=%d\r\n",temp);
	}
	else
	{
	    //printf("connect to servet success DEVICE_ID: temperature1 SECRET_KEY 27c4af8cd8d748dca888f809e9309184 \r\n");
	    break;
	}
    
    }
    while(1)
    {
       
        /*************************�������ݽ��մ������******************/
        if(basicRfPacketIsReady())   //��ѯ��û�յ������ź�
        {
            //������������
            len = basicRfReceive(pRxData, MAX_RECV_BUF_LEN, NULL);
          //  LED1=~LED1;
            signal=pRxData[0];
            ESP8266_SendSensor(signal);
            CLR_UART1_RX_BUF();
        }
        if(UART1_RX_FINISH)
        {
            uint8 len;
            memset(TCP_DATA_BUF,'\0',255);
            len=ESP8266_GetIpData(USART1_RX_BUF, TCP_DATA_BUF);
            printf_uart0(TCP_DATA_BUF,len);
            USER_DataAnalysisProcess(TCP_DATA_BUF);
            CLR_UART1_RX_BUF();
        }
    }
}
/************************MAIN END ****************************/