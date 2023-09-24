/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"
int second,interface_flag=0;

void Time2Handler(){
	keyScanner();
}

void Time3Handler(){
	if(interface_flag==0)second++;
	
}
