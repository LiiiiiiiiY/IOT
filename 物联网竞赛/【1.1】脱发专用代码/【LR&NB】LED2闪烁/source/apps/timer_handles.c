/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"

int count=0;
void Time2Handler(){
	keyScanner();
}

void Time3Handler(){
	if(count%2!=0)GpioToggle(&Led2);
	
}
