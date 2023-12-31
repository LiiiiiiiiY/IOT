
#ifndef _SH10_T_
#define _SH10_T_

#include "hal_defs.h"
#include "hal_cc8051.h"

#define ENGINEER_DEBUG false //调试信息开关

/*******************************************************************
*函数：void SHT_Init(void)
*功能：温湿度传感器初始化
*输入：无
*输出：无
*返回：无
*特殊说明：无
*******************************************************************/
void SHT_Init(void);

/*******************************************************************
*函数：uint8 ChkSum(uint8 *buf, uint8 len)
*功能：采集传感器数据：温度、湿度
*输入：无
*输出：
*       int8 *tem, 温度
*       uint8 *hum, 湿度
*返回：无
*特殊说明：无
*******************************************************************/
void SHT_SmpSnValue(float *tem, float *hum);

/*******************************************************************
*函数：void call_sht11(unsigned int *tem, unsigned int *hum)
*功能：采集传感器数据：温度、湿度
*输入：无
*输出：
*       unsigned int *tem, 温度
*       unsigned int *hum, 湿度
*返回：无
*特殊说明：无
*******************************************************************/
void call_sht11(float *tem, float *hum);

#endif
