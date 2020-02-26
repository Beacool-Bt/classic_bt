#include "filter.h"


void LPF_2nd_Factor_Cal(float deltaT, float Fcut, LPF2ndData_t* lpf_data)
{
	float a = 1 / (2 * M_PI * Fcut * deltaT);
	lpf_data->b0 = 1 / (a*a + 3*a + 1);
	lpf_data->a1 = (2*a*a + 3*a) / (a*a + 3*a + 1);
	lpf_data->a2 = (a*a) / (a*a + 3*a + 1);
}

int32_t Baro_LPF_2nd(LPF2ndData_t* lpf_2nd, float newData)
{
	float baroAltTemp;
	static float baro_lastout, baro_preout;
	
	baroAltTemp = newData * lpf_2nd->b0 + baro_lastout * lpf_2nd->a1 - baro_preout * lpf_2nd->a2;
	baro_preout = baro_lastout;
	baro_lastout = baroAltTemp;
	
	return (int32_t)baroAltTemp;
}

Vector3f_t LowPassFilter_2nd(LPF2ndData_t* lpf_2nd, Vector3f_t newData)
{
	Vector3f_t lpf_2nd_data;
	
	lpf_2nd_data.x = (float)newData.x * lpf_2nd->b0 + lpf_2nd->lastout.x * lpf_2nd->a1 - lpf_2nd->preout.x * lpf_2nd->a2;
	lpf_2nd_data.y = (float)newData.y * lpf_2nd->b0 + lpf_2nd->lastout.y * lpf_2nd->a1 - lpf_2nd->preout.y * lpf_2nd->a2;
	lpf_2nd_data.z = (float)newData.z * lpf_2nd->b0 + lpf_2nd->lastout.z * lpf_2nd->a1 - lpf_2nd->preout.z * lpf_2nd->a2;
	
	lpf_2nd->preout.x = lpf_2nd->lastout.x;
	lpf_2nd->preout.y = lpf_2nd->lastout.y;
	lpf_2nd->preout.z = lpf_2nd->lastout.z;
	
	lpf_2nd->lastout.x = lpf_2nd_data.x;
	lpf_2nd->lastout.y = lpf_2nd_data.y;
	lpf_2nd->lastout.z = lpf_2nd_data.z;
	
	return lpf_2nd_data;
}

/*----------------------»¥²¹ÂË²¨Æ÷ÏµÊý¼ÆËã-------------------------*/
float Complementary_Factor_Cal(float deltaT, float tau)
{
	return tau / (deltaT + tau);
}

/*----------------------Ò»½×»¥²¹ÂË²¨Æ÷-----------------------------*/
float Complementary_Filter_1st(float AngleGyro, float AngleAcc, float cf_factor)
{
	return cf_factor * AngleGyro  + (1 - cf_factor) * AngleAcc;	
}





