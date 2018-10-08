/*
 * FILE: adc_ts.h
 * ADC和触摸屏的测试函数接口
 */

#ifndef __ADC_TS_H__
#define __ADC_TS_H__

/* 
 * 测试ADC
 * 通过A/D转换，测量可变电阻器的电压值
 */       
void Test_Adc(void);

/* 
 * 测试触摸屏，打印触点坐标
 */       
void Test_Ts(void);

#endif /*__ADC_TS_H__*/
