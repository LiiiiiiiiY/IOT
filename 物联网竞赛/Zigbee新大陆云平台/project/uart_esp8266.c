#include "uart.h"
#include "string.h"

#define LED1 P1_0
#define LED2 P1_1
#define LED3 P0_4
uint8 USART1_RX_BUF[MAX_UART1_REC_LEN];//串口1接收缓存
uint8 USART1_RX_RECEIVING=0;//串口接收正在进行中标志
                              //0：暂停或者停止，1：正在进行
uint8 USART1_RX_TIMEOUT_COUNT=0;//串口接收超时数
uint8 USART1_RX_COUNT=0;	
uint8 UART1_RX_FINISH=0;//接收状态标志，1：接收完成，0：接收未完成
uint8 USART1_AT_CMD[MAX_UART1_REC_LEN];//串口1接收缓存

/*
    输入：AtRxBuf：原始AT数据缓存首地址
          GetIpData：截取出来的tcp/ip数据包
    输出：返回收到的数据包长度
         ESP8266接收到的数据，有用数据在+IPD后面
         AT+CIPSEND=76
	{"t":3,"datatype":2,"datas":{"alarm":{"2018-06-19 18:15:02":0}},"msgid":001}
	+IPD,29:{"msgid":1,"status":0,"t":4}
*/
uint8 ESP8266_GetIpData(uint8 *AtRxBuf, char *GetIpData)
{
	char *Point = NULL;
	uint8 len = 0;
	
	Point = strstr((const char *)AtRxBuf, (const char *)"+IPD,");
	if(Point != NULL)//
	{
		len=(Point[5]-'0')*10+Point[6]-'0';//数据长度
		Point = strstr((const char *)AtRxBuf, (const char *)":")+1;
		memcpy(GetIpData, Point, len);
		GetIpData[len]='\0';
		printf("收到IP数据：%s\r\n",GetIpData);
		printf("收到IP数据长度=%d\r\n",len);
	}
	return (len);
}

/******串口0打印**************/
void printf_uart0(char *data_tx,uint8 length)
{
  int i;
  for(i=0;i<length;i++)
  {
      U0DBUF=*(data_tx++);
      while(UTX0IF==0);
      UTX0IF=0;
  }
}
void CLR_UART1_RX_BUF()
{
    USART1_RX_COUNT=0;
    memset(USART1_RX_BUF,'\0',MAX_UART1_REC_LEN);
    UART1_RX_FINISH=0;
    
}
/******串口1(ESP8266)打印**************/
void printf_uart1(char *data_tx,uint8 length)
{
  int i;
  for(i=0;i<length;i++)
  {
      U1DBUF=*(data_tx++);
      while(UTX1IF==0);
      UTX1IF=0;
  }
}

#pragma vector=URX1_VECTOR
__interrupt void UART1_ISR(void)
{
  USART1_RX_RECEIVING=1;//标识当前串口1正在接收数据
  USART1_RX_BUF[USART1_RX_COUNT++]=U1DBUF;//将数据读出来，放在缓存数组中
  USART1_RX_TIMEOUT_COUNT=0;
  if(USART1_RX_COUNT>MAX_UART1_REC_LEN)
  {
    USART1_RX_COUNT=0;
  }
}
/*解析云服务器下发的数据*/
void USER_DataAnalysisProcess(char *RxBuf)
{
  if(strstr(RxBuf,"\"apitag\":\"light\"")!=NULL)//开灯1
    {
       if(strstr(RxBuf,"\"data\":0")!=NULL)
       {
           LED2=1;
       }
       else if(strstr(RxBuf,"\"data\":1")!=NULL)
       {
           LED2=0;
       }
    }
  if(strstr(RxBuf,"\"apitag\":\"light1\"")!=NULL)//开灯2
    {
       if(strstr(RxBuf,"\"data\":0")!=NULL)
       {
           LED1=1;
       }
       else if(strstr(RxBuf,"\"data\":1")!=NULL)
       {
           LED1=0;
       }
    }

}
/*******设置成STA模式
return = 0 ,sucess
return < 0 ,error
**********/
uint8 ESP8266_SetStation()
{
  
   CLR_UART1_RX_BUF();
   //1
   printf_uart1(AT_CWMODE,strlen(AT_CWMODE));//通过串口1发送"AT+CWMODE_CUR=1\r\n"
   //2延时200ms
   halMcuWaitMs(400);
   //判断接收缓存中是否有"OK"
   if(strstr(USART1_RX_BUF,"OK")==NULL)
   {
     printf_uart0(USART1_RX_BUF,strlen(USART1_RX_BUF));
     return -1;
   }
   else
   {
      printf_uart0(USART1_RX_BUF,strlen(USART1_RX_BUF));
      return 0;
   }
     
   
}

/*******设置连接的热点名称和热点密码
return = 0 ,sucess
return < 0 ,error
**********/
uint8 ESP8266_ConnectAP()
{
  
   CLR_UART1_RX_BUF();
   //1
   memset(USART1_AT_CMD,'\0',MAX_UART1_REC_LEN);
   sprintf(USART1_AT_CMD,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",WIFI_AP,WIFI_PWD);
   printf_uart1(USART1_AT_CMD,strlen(USART1_AT_CMD));//通过串口1发送"AT+CWMODE_CUR=1\r\n"
   //2延时200ms
   halMcuWaitMs(6000);
   //判断接收缓存中是否有"OK"
   if(strstr(USART1_RX_BUF,"OK")==NULL)
   {
      printf_uart0(USART1_RX_BUF,strlen(USART1_RX_BUF));
     return -1;
   }
     else
   {
      printf_uart0(USART1_RX_BUF,strlen(USART1_RX_BUF));
      return 0;
   }
   
}
/*******连接新大陆服务器
return = 0 ,sucess
return < 0 ,error
**********/

uint8 ESP8266_ConnectServer()
{
  
   CLR_UART1_RX_BUF();
   //1
   memset(USART1_AT_CMD,'\0',MAX_UART1_REC_LEN);
   sprintf(USART1_AT_CMD,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",SERVER_IP,SERVER_PORT);
   printf_uart1(USART1_AT_CMD,strlen(USART1_AT_CMD));//通过串口1发送"AT+CWMODE_CUR=1\r\n"
   //2延时200ms
   halMcuWaitMs(2000);
   //判断接收缓存中是否有"OK"
   if(strstr(USART1_RX_BUF,"OK")==NULL)
   {
     printf_uart0(USART1_RX_BUF,strlen(USART1_RX_BUF));
     return -1;
   }
      else
   {
      printf_uart0(USART1_RX_BUF,strlen(USART1_RX_BUF));
      return 0;
   }
   
}
/*******客户端与服务器建立连接请
return = 0 ,sucess
return < 0 ,error
**********/
uint8 ESP8266_IpSend(char *IpBuf, uint8 len)
{
  uint8 error;
   CLR_UART1_RX_BUF();
   uint8 send_temp[MAX_UART1_REC_LEN];
   sprintf(send_temp,"AT+CIPSEND=%d\r\n",len);
   printf_uart1(send_temp,strlen(send_temp));//通过串口1发送"AT+CWMODE_CUR=1\r\n"
   //2延时23ms
   halMcuWaitMs(3);
   //printf_uart0(IpBuf,len);
   //判断接收缓存中是否有"OK"
   if(strstr(USART1_RX_BUF,"OK")==NULL)
   {
     return -1;
   }
   CLR_UART1_RX_BUF();
   printf_uart1(IpBuf,len);
   int i;
   for(i=0;i<60;i++)
   {
     if(strstr(USART1_RX_BUF,"SEND OK")==NULL)
     {
        printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
        error=-1;
     }
     else
     {
       printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
       error=0;
       break;
     }
     
     halMcuWaitMs(100);
   }
   return error;
   
}
/*******ESP8266发送传感数据给服务器
return = 0 ,sucess
return < 0 ,error
**********/
uint8 ESP8266_SendSensor(uint8 signal)
{
  uint8 error;
  CLR_UART1_RX_BUF();
  sprintf(USART1_AT_CMD,"{\"t\":3,\"datatype\":1,\"datas\":{\"signal\":%d},\"msgid\":123}",signal);
  if(ESP8266_IpSend(USART1_AT_CMD,strlen(USART1_AT_CMD))==0)
   {
      //error=0;
     printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
     uint8 TryCount=0;
     for(TryCount=0;TryCount<50;TryCount++)
     {
        if(strstr(USART1_RX_BUF,"\"status\": 0")==NULL)
        {
             error=-1;
        }
        else
        {
            break;
            error=0;
        }
         halMcuWaitMs(10);
     }
   }
  else
  {
     printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
     error=-1;
  }
  return error;
}


/**********ESP8266配置以及建立与服务器的连接**********/
uint8 ConnectToServer(char *DeviceID, char *SecretKey)
{
  uint8 TryCount=0;
  uint8 error=0;
  //step1:设置成STA模式
  for (TryCount=0;TryCount<3;TryCount++)
  {
    
   if(ESP8266_SetStation()==0)
   {
     error=0;
     break;
   }
   else
   {
     error=-1;
   }
  }
  if(error<0)
    return error;
  
  //step2：设置连接的热点名称和热点密码
  for (TryCount=0;TryCount<3;TryCount++)
  {
    
   if(ESP8266_ConnectAP()==0)
   {
     error=0;
     break;
   }
   else
   {
     error=-1;
   }
  }
  if(error<0)
    return error;
  //step3：连接新大陆服务器
  for (TryCount=0;TryCount<3;TryCount++)
  {
    
   if(ESP8266_ConnectServer()==0)
   {
     error=0;
     break;
   }
   else
   {

     error=-1;
   }
  }
  if(error<0)
    return error;
  //4客户端请求与服务器建立tcp连接
   CLR_UART1_RX_BUF();
   sprintf(USART1_AT_CMD,"{\"t\":1,\"device\":\"%s\",\"key\":\"%s\",\"ver\":\"v0.0.0.0\"}",MY_DEVICE_ID,MA_SECRET_KEY); 
   printf_uart0(USART1_AT_CMD,strlen(USART1_AT_CMD));
   if(ESP8266_IpSend(USART1_AT_CMD,strlen(USART1_AT_CMD))==0)
   {
     printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
     for(TryCount=0;TryCount<50;TryCount++)
     {
        if(strstr(USART1_RX_BUF,"\"status\":0")==NULL)
        {
             printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
             error=-1;
        }
        else
        {
            printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
            error=0;
            break;
            
        }
         halMcuWaitMs(10);
     }
   }
   else
   {
     printf_uart0(USART1_RX_BUF,USART1_RX_COUNT);
     error=-1;
   }
    return error;
}

