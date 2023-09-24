/**************************************************************************************************
  Filename:      sh10.c

  Description:    sh10 driver 
                  
**************************************************************************************************/

/******************************************************************************
 * INCLUDES
 */
#include "ioCC2530.h"
#include "hal_defs.h"
//#include "hal_types.h"
#include "get_adc.h"


/******************************************************************************
 * GLOBAL VARIABLES
 */

void hal_adc_Init(void)
{
	APCFG  |=1;   
        P0SEL  |= (1 << (0));	
        P0DIR  &= ~(1 << (0));	
    
}
/******************************************************************************
 * ����       get_adc
 * ����       ��ȡA/Dֵ
 * ��ڲ���   ��
 * ���ڲ���   16λ��ѹֵ���ֱ���Ϊ10mV, ��0x0102��ʾ2.58V
 *****************************************************************************/
float readAdc(void)
{
   int32 value;
   hal_adc_Init(); // ADC��ʼ��
   ADCIF = 0;   //��ADC �жϱ�־
   //���û�׼��ѹavdd5:3.3V��ͨ��0������ADת��
   ADCCON3 = (0x80 | 0x10 | 0x00);
   while ( !ADCIF )
   {
       ;  //�ȴ�ADת������
   }
   value = ADCH;
   value = value<< 8;
   value |= ADCL;
   if(value&0x8000)
       value=0;
  // ADֵת���ɵ�ѹֵ
  // 0 ��ʾ 0V ��32768 ��ʾ 3.3V
  // ��ѹֵ = (value*3.3)/32768 ��V)
  //value = (value * 330);
  //value = value >> 15;   // ����32768
  // ���طֱ���Ϊ0.01V�ĵ�ѹֵ
  return (value*3.3)/32768;
}



void hal_adc4CH_Init(void)
{
	    APCFG  |= (1 << ACH1) | (1 << ACH2)| (1 << ACH3)| (1 << ACH4);   
        P0SEL  |= (1 << ACH1) | (1 << ACH2)| (1 << ACH3)| (1 << ACH4);	
        P0DIR  &= ~((1 << ACH1) | (1 << ACH2)| (1 << ACH3)| (1 << ACH4));	
    
}
#define AD4MA 32768*150*4/3300
#define AD20MA 32768*150*20/3300
//return 10ADֵ��0---0v, 3ff---3.3v

float readAdc4CH(uint8 ch)
{
    int32 value;
    ADCIF = 0;   //��ADC �жϱ�־
   //���û�׼��ѹavdd5:3.3V��ͨ��ch������ADת��
   ADCCON3 = (0x80 | 0x10 | (ch&0x0f) );
   while ( !ADCIF )
   {
       ;  //�ȴ�ADת������
   }
   value = ADCH;
   value = value<< 8;
   value |= ADCL;
   if(value&0x8000)
       value=0;
   return (value*3.3)/32768;
   //value= value>>5;
   //return (uint16)value;
   /*
  // ADֵת���ɵ�ѹֵ
  // 0 ��ʾ 0V ��32768 ��ʾ 3.3V
  // ���� 0--4ma, 255--20ma, ����150r
  value = (value - AD4MA)*255/(AD20MA - AD4MA);
  if(value<0)
      return    0;
  if(value>255)
      return    255;
  return    (uint8)value&0xff;*/
   
}






















