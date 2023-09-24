#ifndef _UART_H_
#define  _UART_H_
#include "hal_cc8051.h"
#include "hal_defs.h"
#include "clound_connecting.h"
#define MAX_UART1_REC_LEN  255
extern uint8 USART1_RX_BUF[MAX_UART1_REC_LEN];//串口1接收缓存
extern uint8 USART1_RX_RECEIVING;//串口接收正在进行中标志
                              //0：暂停或者停止，1：正在进行
extern uint8 USART1_RX_TIMEOUT_COUNT;//串口接收超时数
extern uint8 USART1_RX_COUNT;	
extern uint8 UART1_RX_FINISH;//接收状态标志，1：接收完成，0：接收未完成

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