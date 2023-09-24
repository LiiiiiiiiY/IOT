#include"iocc2530.h"
#define D6 P1_4
#define D5 P1_3

void delay(unsigned int t)
{
  while(t--);
}


void Init_IO()
{
  P1SEL &= ~0X1B;
  P1DIR |= 0X1B;
  P1 &= ~0X1B;
}
void huxideng(unsigned int on_delay,unsigned int off_delay)
{
  D6 = D5 = 1;
  delay(on_delay);
  D5 = D6 = 0;
  delay(off_delay);
}
unsigned char flag = 0;
void main()
{
  Init_IO();
  unsigned int sys = 750;
  unsigned int pwm = 0;
  while(1)
  {
    
    
    
    if(flag == 0)
    {
      sys--;
      pwm++;
      huxideng(sys,pwm);
      if(sys == 0)
        flag = 1;
    }
    if(flag == 1)
    {
      sys++;
      pwm--;
      huxideng(sys,pwm);
      if(sys == 750)
        flag = 0;
    }
    
  }
}