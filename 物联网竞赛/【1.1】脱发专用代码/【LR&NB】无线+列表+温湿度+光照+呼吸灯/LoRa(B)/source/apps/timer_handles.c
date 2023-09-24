/* Includes ------------------------------------------------------------------*/
#include "timer_handles.h"
#include "key_scaner.h"
#include "hal_oled.h"
#include "led_light.h"
int interfaceFlag=0;
int Tcount=1;
extern unsigned char light1[],light2[],light3[];
void Time2Handler(){
	keyScanner();
}

void Time3Handler(){
	if(interfaceFlag==6)
	{
		
		switch(Tcount)
			{
				case 1:OLED_Clear();OLED_DrawBMP(32,4,32,32,light1);break;
				case 2:OLED_Clear();OLED_DrawBMP(32,4,32,32,light2);break;
				case 3:OLED_Clear();OLED_DrawBMP(32,4,32,32,light3);break;
				case 4:OLED_Clear();OLED_DrawBMP(32,4,32,32,light2);break;
			}
			Tcount++;
			if(Tcount==5)Tcount=1;
	}
	else if(interfaceFlag==7)
	{
		switch(Tcount)
		{
			case 1:OLED_Clear();OLED_DrawBMP(32,4,32,32,light1);break;
			case 2:OLED_Clear();OLED_DrawBMP(32,4,32,32,light2);break;
		}
		GpioToggle(&Led2);
		Tcount++;
		if(Tcount>=3)Tcount=1;
	}
}
