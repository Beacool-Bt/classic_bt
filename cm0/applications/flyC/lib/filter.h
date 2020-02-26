#ifndef _FILTER_H_
#define _FILTER_H_

#include "config.h"

void LPF_2nd_Factor_Cal(float deltaT, float Fcut, LPF2ndData_t* lpf_data);
Vector3f_t LowPassFilter_2nd(LPF2ndData_t* lpf_2nd, Vector3f_t newData);

float Complementary_Factor_Cal(float deltaT, float tau);
float Complementary_Filter_1st(float AngleGyro, float AngleAcc, float cf_factor);
	
#endif


