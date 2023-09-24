
#include <stdio.h>
#include "sht.h"
#include "sht1x.h"
#include "sht3x.h"


#define SHT_TYPE_SHT1X 0x00
#define SHT_TYPE_SHT3X 0x01

static uint8 SHT_TypeFlag = 0x00;//Mean��0-SHT1X, 1-SHT3X

/*******************************************************************
*������void SHT_Init(void)
*���ܣ���ʪ�ȴ�������ʼ��
*���룺��
*�������
*���أ���
*����˵������
*******************************************************************/
void SHT_Init(void)
{
    etError   error;       // error code
    error = SHT3X_Init(DEFAULT_SHT3X_ADDR);
    if(error != NO_ERROR)
    {
        SHT1X_Init();
        SHT_TypeFlag = SHT_TYPE_SHT1X;
    }
    else
    {
        SHT_TypeFlag = SHT_TYPE_SHT3X;
    }
    
#if (ENGINEER_DEBUG)
    if(SHT_TypeFlag != SHT_TYPE_SHT1X)
    {
        printf("��ʪ�ȴ������ͺţ�SHT3X\r\n");
    }
    else
    {
        printf("��ʪ�ȴ������ͺţ�SHT1X\r\n");
    }
#endif
}

/*******************************************************************
*������void SHT_SmpSnValue(float *tem, float *hum)
*���ܣ��ɼ����������ݣ��¶ȡ�ʪ��
*���룺��
*�����
*       int8 *tem, �¶�
*       uint8 *hum, ʪ��
*���أ���
*����˵������
*******************************************************************/
void SHT_SmpSnValue(float *tem, float *hum)
{
    
    if(SHT_TypeFlag != SHT_TYPE_SHT1X)
    {
        etError error;       // error code
        float      temperature; // temperature [��]
        float      humidity;    // relative humidity [%RH]
      
        error = NO_ERROR;
        // demonstrate a single shot measurement with polling and 50ms timeout
        error = SHT3X_GetTempAndHumi(&temperature, &humidity, REPEATAB_HIGH, MODE_POLLING, 50);
        if(error != NO_ERROR) // do error handling here
        {
            error = SHT3X_SoftReset();
            // ... if the soft reset fails, do a hard reset
            if(error != NO_ERROR)
            {
                SHT3X_HardReset();
            }
        }
#if (ENGINEER_DEBUG)
        printf("SHT3X �¶�=%f��, ʪ��=%f%%\r\n", temperature, humidity);
#endif
        *tem=temperature;
        *hum=humidity;
    }
    else
    {
        int tem_val, hum_val;
        call_sht1x(&tem_val, &hum_val);
        *tem=tem_val;
        *hum=hum_val;
#if (ENGINEER_DEBUG)
        printf("SHT1X �¶�:%d�� ʪ��:%d%%\r\n", tem_val, hum_val);
#endif
    }
#if (ENGINEER_DEBUG)
    float dew_point;
    dew_point=calc_dewpoint((float)(*tem), (float)(*hum)); //calculate dew point
    printf("SHT �¶�:%d�� ʪ��:%d%%\r\n", *tem, *hum);
    printf("¶��Dew Point:%5.1f��\n", dew_point);
#endif
}

/*******************************************************************
*������void call_sht11(float *tem, float *hum)
*���ܣ��ɼ����������ݣ��¶ȡ�ʪ��
*���룺��
*�����
*       unsigned int *tem, �¶�
*       unsigned int *hum, ʪ��
*���أ���
*����˵������
*******************************************************************/
void call_sht11(float *tem, float *hum)
{
  float sensor_tem;
  float sensor_val;
  SHT_SmpSnValue(&sensor_tem, &sensor_val);
  *tem = sensor_tem;
  *hum = sensor_val;
}

