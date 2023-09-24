#ifndef _UART_H_
#define  _UART_H_
#include "hal_cc8051.h"
#include "hal_defs.h"
#include "clound_connecting.h"
#define MAX_UART1_REC_LEN  255
extern uint8 USART1_RX_BUF[MAX_UART1_REC_LEN];//����1���ջ���
extern uint8 USART1_RX_RECEIVING;//���ڽ������ڽ����б�־
                              //0����ͣ����ֹͣ��1�����ڽ���
extern uint8 USART1_RX_TIMEOUT_COUNT;//���ڽ��ճ�ʱ��
extern uint8 USART1_RX_COUNT;	
extern uint8 UART1_RX_FINISH;//����״̬��־��1��������ɣ�0������δ���

extern uint8 ConnectToServer(char *DeviceID, char *SecretKey);
extern void printf_uart1(char *data_tx,uint8 length);
extern void printf_uart0(char *data_tx,uint8 length);
extern void CLR_UART1_RX_BUF();
extern uint8 ESP8266_ConnectAP();
extern uint8 ESP8266_IpSend(char *IpBuf, uint8 len);
extern uint8 ESP8266_SendSensor(uint8 signal);
extern void CLR_UART1_RX_BUF();
extern uint8 ESP8266_GetIpData(uint8 *AtRxBuf, char *GetIpData);
#endif