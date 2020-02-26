#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>

typedef struct{
	float x;
	float y;
	float z;
}Vector3f_t;

typedef struct{
	double x;
	double y;
	double z;
}Vector3d_t;

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}Vector3i_t;

typedef struct 
{
	float b0;
	float a1;
	float a2;
	Vector3f_t preout;
	Vector3f_t lastout;
}LPF2ndData_t;

#ifndef M_PI
#define M_PI 3.141592653f
#endif
#define DEG_TO_RAD 0.01745329f
#define RAD_TO_DEG 57.29577951f

#define EARTH_RADIUS  6371.004 //km

#define HALF_SQRT_2 0.70710678118654757f	//根号2的值

float safe_asin(float v);

//浮点数限幅
float constrain_float(float amt, float low, float high);

//16位整型数限幅
int16_t constrain_int16(int16_t amt, int16_t low, int16_t high);

//16位无符号整型数限幅
uint16_t constrain_uint16(uint16_t amt, uint16_t low, uint16_t high);

//32位整型数限幅
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high);

int32_t applyDeadband(int32_t value, int32_t deadband);
float applyDeadband_float(float value, float deadband);

//角度转弧度
float radians(float deg);

//弧度转角度
float degrees(float rad);

//求平方
float sq(float v);

//2维向量长度
float pythagorous2(float a, float b);

//3维向量长度
float pythagorous3(float a, float b, float c); 

//4维向量长度
float pythagorous4(float a, float b, float c, float d);

Vector3f_t Vector3iTo3f(Vector3i_t vector);
float Wrap_360_Deg(float angle);

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
//#define abs(x)    ( x > 0 ? (x) : -(x))

#endif







