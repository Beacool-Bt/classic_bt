#include "rc.h"

rc_data_t rc_data;



void RC_Init(void)
{
	
}

void RC_CalCommand(void)
{
	uint8_t i = 0;
	for (i = 0; i < 3; i++) 
	{	//处理ROLL,PITCH,YAW这三个轴的数据
		rc_data.Command[i] = (rc_data.rawData[i] - RC_MID) * 0.35;
	}	
	rc_data.Command[YAW] = -rc_data.Command[YAW];
	rc_data.Command[THROTTLE] = rc_data.rawData[THROTTLE];
		
		//-------------------航向锁定------------------
		if (abs(rc_data.Command[YAW]) < 70 && rc_data.rawData[THROTTLE] > RC_MINCHECK) 
		{
			int16_t dif = imu_data.angle.z - imu_data.magHold;
			if (dif <= -180)
				dif += 360;
			if (dif >= +180)
				dif -= 360;
			dif = -dif;
			
			rc_data.Command[YAW] -= dif * pid[PIDMAG].kP * 0.1;  	
		} 	
		else
			imu_data.magHold = imu_data.angle.z;	
		
}


const uint8_t stick_min_flag[4] = {1<<0,1<<2,1<<4,1<<6}; 
const uint8_t stick_max_flag[4] = {1<<1,1<<3,1<<5,1<<7};
#define ROL_L 0x01
#define ROL_H 0x02
#define PIT_L	0x04
#define PIT_H 0x08
#define YAW_L 0x10
#define YAW_H 0x20
#define THR_L 0x40
#define THR_H 0x80

void RC_CheckSticks(void)
{
	int i;
	static uint8_t rcDelayCommand;
	
	static uint8_t stick_flag = 0;

	for (i = 0; i < 4; i++) 
	{
			if(rc_data.rawData[i]<900||rc_data.rawData[i]>2000)	//如果摇杆值不在正常范围内，则退出检查
				break;
			
			if (rc_data.rawData[i] < RC_MINCHECK)
					stick_flag |= stick_min_flag[i];  // check for MIN
			else
					stick_flag &= ~stick_min_flag[i];
			
			if (rc_data.rawData[i] > RC_MAXCHECK)
					stick_flag |= stick_max_flag[i];  // check for MAX
			else
					stick_flag &= ~stick_max_flag[i];  // check for MAX
	}	
	
	if(stick_flag&0xff)	//如果任一摇杆在最大或最小位置
	{
		if(rcDelayCommand < 250)
			rcDelayCommand++;
	}
	else
	{
		rcDelayCommand = 0;
		stick_flag &= 0;
	}	
		
	if (rcDelayCommand == 100) //2s: 20ms * 100
	{
		if (config.f.ARMED) //如果已经处于解锁状态
		{ 
			if((stick_flag & YAW_L)&&(stick_flag & THR_L))
			{
				config.f.ARMED = 0;	//上锁
			}
		}
		else
		{
			if((stick_flag & YAW_H)&&(stick_flag & THR_L))
			{
				config.f.ARMED = 1;	//解锁
			}
		}
		stick_flag &= 0;
		rcDelayCommand = 0;
	}
}

