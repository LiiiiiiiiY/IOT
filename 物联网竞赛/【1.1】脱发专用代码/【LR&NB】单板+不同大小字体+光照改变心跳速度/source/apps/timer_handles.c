/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"
#include "hal_oled.h"
#include "adc_reader.h"
#include "math.h"
uint8_t hour=12,min=0,sec=0,Flag=0;
char str[20];

void Time2Handler(){
	keyScanner();
}

void Time3Handler()
{
	if(Flag==0)
	{
		sprintf(str,"%02d:%02d:%02d",hour,min,sec);
		OLED_ShowStringSize(78,0,12,(uint8_t*)str);
		sec++;
		if(sec==60)
		{
			sec=0;
			min++;
			if(min==60)
			{
				min=0;
				hour++;
				if(hour==24)
					hour=0;
			}
		}
	}
}
