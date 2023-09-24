/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"
#include "usart1-board.h"
//uint8_t send_failyre[3]={0xFB,0x01,0xFE};

void Time2Handler(){
	keyScanner();
}

void Time3Handler()
{
	USART1_RX_COUNT=0;
	USART1_ReceiveClr();
}
