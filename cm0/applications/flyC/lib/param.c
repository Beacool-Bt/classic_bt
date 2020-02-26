
#include "param.h"
#include "board.h"
#include "sensor.h"
//#include "eeprom.h"



#define EE_FirstInitFlag 0
#define EE_ACC_X_OFFSET_ADDR	1
#define EE_ACC_Y_OFFSET_ADDR	2
#define EE_ACC_Z_OFFSET_ADDR	3
#define EE_GYRO_X_OFFSET_ADDR	4
#define EE_GYRO_Y_OFFSET_ADDR	5
#define EE_GYRO_Z_OFFSET_ADDR	6
#define EE_PID_ROL_P	7
#define EE_PID_ROL_I	8
#define EE_PID_ROL_D	9
#define EE_PID_PIT_P	10
#define EE_PID_PIT_I	11
#define EE_PID_PIT_D	12
#define EE_PID_YAW_P	13
#define EE_PID_YAW_I	14
#define EE_PID_YAW_D	15
#define EE_PID_ALT_P	16
#define EE_PID_ALT_I	17
#define EE_PID_ALT_D	18
#define EE_PID_LEVEL_P	19
#define EE_PID_LEVEL_I	20
#define EE_PID_LEVEL_D	21
#define EE_PID_MAG_P	22
#define EE_PID_MAG_I	23
#define EE_PID_MAG_D	24

uint16_t FirstInitFlag = 0x44;

static	void Param_SaveFirstInitFlag(void);
static	uint16_t Param_ReadFirstInitFlag(void);
static	void Param_ReadAccelOffset(void);
static	void Param_ReadGyroOffset(void);
static	void Param_ReadPID(void);
	
void Param_Init(void)
{

	if(Param_ReadFirstInitFlag()!= FirstInitFlag)	//板子从未初始化
	{
		Param_SavePID();
	}
	
  Param_ReadPID();
	Param_ReadAccelOffset();
	Param_ReadGyroOffset();
	Param_SaveFirstInitFlag();
	
}

void Param_SaveFirstInitFlag(void)
{
	//EE_WriteVariable(VirtAddVarTab[EE_FirstInitFlag], FirstInitFlag);
}

uint16_t Param_ReadFirstInitFlag(void)
{
	uint16_t temp;
	//EE_ReadVariable(VirtAddVarTab[EE_FirstInitFlag], &temp);
	return temp;
}

void Param_SaveAccelOffset(void)
{
	//EE_WriteVariable(VirtAddVarTab[EE_ACC_X_OFFSET_ADDR], sensor.Acc_Offset.x);
	//EE_WriteVariable(VirtAddVarTab[EE_ACC_Y_OFFSET_ADDR], sensor.Acc_Offset.y);
	//EE_WriteVariable(VirtAddVarTab[EE_ACC_Z_OFFSET_ADDR], sensor.Acc_Offset.z);
}
void Param_ReadAccelOffset(void)
{
	//uint16_t temp[3];
	//EE_ReadVariable(VirtAddVarTab[EE_ACC_X_OFFSET_ADDR], &temp[0]);
	//EE_ReadVariable(VirtAddVarTab[EE_ACC_Y_OFFSET_ADDR], &temp[1]);
	//EE_ReadVariable(VirtAddVarTab[EE_ACC_Z_OFFSET_ADDR], &temp[2]);
	//sensor.Acc_Offset.x = temp[0];
	//sensor.Acc_Offset.y = temp[1];
	//sensor.Acc_Offset.z = temp[2];
}
void Param_SaveGyroOffset(void)
{
	//EE_WriteVariable(VirtAddVarTab[EE_GYRO_X_OFFSET_ADDR], sensor.Gyro_Offset.x);
	//EE_WriteVariable(VirtAddVarTab[EE_GYRO_Y_OFFSET_ADDR], sensor.Gyro_Offset.y);
	//EE_WriteVariable(VirtAddVarTab[EE_GYRO_Z_OFFSET_ADDR], sensor.Gyro_Offset.z);
}
void Param_ReadGyroOffset(void)
{
	//uint16_t temp[3];
	//EE_ReadVariable(VirtAddVarTab[EE_GYRO_X_OFFSET_ADDR], &temp[0]);
	//EE_ReadVariable(VirtAddVarTab[EE_GYRO_Y_OFFSET_ADDR], &temp[1]);
	//EE_ReadVariable(VirtAddVarTab[EE_GYRO_Z_OFFSET_ADDR], &temp[2]);
	//sensor.Gyro_Offset.x = temp[0];
	//sensor.Gyro_Offset.y = temp[1];
	//sensor.Gyro_Offset.z = temp[2];
}
void Param_SavePID(void)
{
	//uint16_t _temp;
	//_temp = pid[PIDROLL].kP * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_ROL_P],_temp);
	//_temp = pid[PIDROLL].kI * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_ROL_I],_temp);
	//_temp = pid[PIDROLL].kD * 1000;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_ROL_D],_temp);
	//_temp = pid[PIDPITCH].kP * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_PIT_P],_temp);
	//_temp = pid[PIDPITCH].kI * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_PIT_I],_temp);
	//_temp = pid[PIDPITCH].kD * 1000;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_PIT_D],_temp);
	//_temp = pid[PIDYAW].kP * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_YAW_P],_temp);
	//_temp = pid[PIDYAW].kI * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_YAW_I],_temp);
	//_temp = pid[PIDYAW].kD * 1000;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_YAW_D],_temp);
	//_temp = pid[PIDALT].kP * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_ALT_P],_temp);
	//_temp = pid[PIDALT].kI * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_ALT_I],_temp);
	//_temp = pid[PIDALT].kD * 1000;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_ALT_D],_temp);
	//_temp = pid[PIDLEVEL].kP * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_LEVEL_P],_temp);
	//_temp = pid[PIDLEVEL].kI * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_LEVEL_I],_temp);
	//_temp = pid[PIDLEVEL].kD * 1000;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_LEVEL_D],_temp);
	//_temp = pid[PIDMAG].kP * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_MAG_P],_temp);
	//_temp = pid[PIDMAG].kI * 100;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_MAG_I],_temp);
	//_temp = pid[PIDMAG].kD * 1000;
	//EE_WriteVariable(VirtAddVarTab[EE_PID_MAG_D],_temp);
}

void Param_ReadPID(void)
{
	//uint16_t _temp;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_ROL_P],&_temp);
	//pid[PIDROLL].kP = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_ROL_I],&_temp);
	//pid[PIDROLL].kI = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_ROL_D],&_temp);
	//pid[PIDROLL].kD = (float)_temp / 1000;
	
	//EE_ReadVariable(VirtAddVarTab[EE_PID_PIT_P],&_temp);
	//pid[PIDPITCH].kP = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_PIT_I],&_temp);
	//pid[PIDPITCH].kI = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_PIT_D],&_temp);
	//pid[PIDPITCH].kD = (float)_temp / 1000;
	
	//EE_ReadVariable(VirtAddVarTab[EE_PID_YAW_P],&_temp);
	//pid[PIDYAW].kP = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_YAW_I],&_temp);
	//pid[PIDYAW].kI = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_YAW_D],&_temp);
	//pid[PIDYAW].kD = (float)_temp / 1000;
	
	//EE_ReadVariable(VirtAddVarTab[EE_PID_ALT_P],&_temp);
	//pid[PIDALT].kP = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_ALT_I],&_temp);
	//pid[PIDALT].kI = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_ALT_D],&_temp);
	//pid[PIDALT].kD = (float)_temp / 1000;
	
	//EE_ReadVariable(VirtAddVarTab[EE_PID_LEVEL_P],&_temp);
	//pid[PIDLEVEL].kP = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_LEVEL_I],&_temp);
	//pid[PIDLEVEL].kI = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_LEVEL_D],&_temp);
	//pid[PIDLEVEL].kD = (float)_temp / 1000;
	
	//EE_ReadVariable(VirtAddVarTab[EE_PID_MAG_P],&_temp);
	//pid[PIDMAG].kP = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_MAG_I],&_temp);
	//pid[PIDMAG].kI = (float)_temp / 100;
	//EE_ReadVariable(VirtAddVarTab[EE_PID_MAG_D],&_temp);
	//pid[PIDMAG].kD = (float)_temp / 1000;
}











