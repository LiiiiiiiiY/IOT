#include "iocc2530.h"

#define LED1 P1_0
#define LED2 P1_1

unsigned int adc_value = 0;
float adc_volt=0;

void Init_Led()
{
  P1SEL &=~0x1b;
  P1DIR |= 0x1b;
  P1    &=~0x1b;
}

void init_ADC()
{
  APCFG |= 0x01;
}

void Start_ADC_Control_Led()
{
  ADCCON3 = (0x80|0x30|0x00);
  
  while((ADCCON1 & 0x80) != 0x80);
  
  adc_value = ADCH;
  adc_value = (adc_value << 8)|ADCL;
  adc_value = adc_value >>5;
  
  adc_volt = (3.3/1023)*adc_value;
  
  if(adc_volt<0.5)
  {
    LED1=1;LED2=1;
  }
  else if(adc_volt<1.0)
  {
    LED1=1;LED2=0;
  }
  else
  {
    LED1=LED2=0;
  }
}

void main()
{
  Init_Led();
  init_ADC();
  while(1)
  {
    Start_ADC_Control_Led();
  }
}