#include "imu.h"
#include "sensor.h"
#include "mpu6050.h"
imu_data_t imu_data;

int32_t accRatio;
	

	//滤波器参数初始化
static	void Filter_Init(void);

//IMU初始化
void IMU_Init()
{

    imu_data.initAlignment = 0;	
	//滤波器参数初始化
	Filter_Init();
	//传感器初始化
	Sensor_Init();	

}

//更新传感器数据
void IMU_UpdateSensor()
{
   Vector3i_t temp;
	//读取加速度
    Sensor_ReadData();
	 if(imu_data.initAlignment==0)
	 	{
         imu_data.initAlignment = 1;
		 sensor.Gyro_CALIBRATED = 1;
		 return;
	 }
	 if(sensor.Gyro_CALIBRATED ==1)
	 	{
      return;
	 }
	//获取角速度，单位为度每秒
	imu_data.Gyro = Sensor_GetGyro();
	 temp= imu_data.Gyro;
	imu_data.Gyro.x = temp.y;
	imu_data.Gyro.y = -temp.x;
	imu_data.Gyrof.x = imu_data.Gyro.x*MPU6050G_S2000DPS;
	imu_data.Gyrof.y = imu_data.Gyro.y*MPU6050G_S2000DPS;
	imu_data.Gyrof.z = imu_data.Gyro.z*MPU6050G_S2000DPS;
	//获取加速度采样值
	imu_data.Acc = Sensor_GetAcc();
	 temp= imu_data.Acc;
    imu_data.Acc.x = temp.y;
	imu_data.Acc.y = -temp.x;
	imu_data.Accf.x = imu_data.Acc.x*MPU6050A_8mg;
	imu_data.Accf.y = imu_data.Acc.y*MPU6050A_8mg;
	imu_data.Accf.z = imu_data.Acc.z*MPU6050A_8mg;
	 	
}


//计算飞行器姿态
void IMU_GetAttitude()
{
	float deltaT;
	static uint32_t previousT=0;
	uint32_t tnow = 0;
	
	//角速度二阶低通滤波	
	imu_data.gyro_Lpf = LowPassFilter_2nd(&imu_data.Gyro_lpf_2nd, imu_data.Gyrof);
	//加速度二阶低通滤波
	imu_data.acc_Lpf = LowPassFilter_2nd(&imu_data.Acc_lpf_2nd, imu_data.Accf);


	
	tnow = systick_get_us();
	deltaT = (tnow-previousT)*1e-6;
	previousT = tnow;

	//IMU_DCM(imu_data.Gyro,imu_data.acc_Lpf);
	IMU_Update(imu_data.acc_Lpf, imu_data.gyro_Lpf,deltaT);

}



#if 1
void Filter_Init() 
{
	
	
	//互补滤波器系数计算
	imu_data.gyro_cf= Complementary_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);
	//加速度低通滤波器参数计算
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT, &imu_data.Acc_lpf_2nd);
	//角速度二阶低通滤波器系数计算
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_LPF_CUT, &imu_data.Gyro_lpf_2nd);		
}



float Kp = 0.4f;		/*比例增益*/
float Ki = 0.001f;		/*积分增益*/
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;		/*积分误差累计*/

static float q0 = 1.0f;	/*四元数*/
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	

//static float baseZacc = 1.0;		/*静态Z轴加速度*/

static float invSqrt(float x);	/*快速开平方求倒*/

void IMU_Update(Vector3f_t acc, Vector3f_t gyro, float dt)	/*数据融合 互补滤波*/
{
	float normalise;
	float ex, ey, ez;
	float q0s, q1s, q2s, q3s;	/*四元数的平方*/
	static float R11,R21;		/*矩阵(1,1),(2,1)项*/
	static float vecxZ, vecyZ, veczZ;	/*机体坐标系下的Z方向向量*/
	float halfT =0.5f * dt;
//	Vector3f_t tempacc =acc;

	gyro.x = gyro.x * DEG_TO_RAD;	/* 度转弧度 */
	gyro.y = gyro.y * DEG_TO_RAD;
	gyro.z = gyro.z * DEG_TO_RAD;

	/* 某一个方向加速度不为0 */
	if((acc.x != 0.0f) || (acc.y != 0.0f) || (acc.z != 0.0f))
	{
		/*单位化加速计测量值*/
		normalise = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
		acc.x *= normalise;
		acc.y *= normalise;
		acc.z *= normalise;

		/*加速计读取的方向与重力加速计方向的差值，用向量叉乘计算*/
		ex = (acc.y * veczZ - acc.z * vecyZ);
		ey = (acc.z * vecxZ - acc.x * veczZ);
		ez = (acc.x * vecyZ - acc.y * vecxZ);
		
		/*误差累计，与积分常数相乘*/
		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;
		
		/*用叉积误差来做PI修正陀螺零偏，即抵消陀螺读数中的偏移量*/
		gyro.x += Kp * ex + exInt;
		gyro.y += Kp * ey + eyInt;
		gyro.z += Kp * ez + ezInt;
	}
	/* 一阶近似算法，四元数运动学方程的离散化形式和积分 */
	float q0Last = q0;
	float q1Last = q1;
	float q2Last = q2;
	float q3Last = q3;
	q0 += (-q1Last * gyro.x - q2Last * gyro.y - q3Last * gyro.z) * halfT;
	q1 += ( q0Last * gyro.x + q2Last * gyro.z - q3Last * gyro.y) * halfT;
	q2 += ( q0Last * gyro.y - q1Last * gyro.z + q3Last * gyro.x) * halfT;
	q3 += ( q0Last * gyro.z + q1Last * gyro.y - q2Last * gyro.x) * halfT;
	
	/*单位化四元数*/
	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	/*四元数的平方*/
	q0s = q0 * q0;
	q1s = q1 * q1;
	q2s = q2 * q2;
	q3s = q3 * q3;
	
	R11 = q0s + q1s - q2s - q3s;	/*矩阵(1,1)项*/
	R21 = 2 * (q1 * q2 + q0 * q3);	/*矩阵(2,1)项*/

	/*机体坐标系下的Z方向向量*/
	vecxZ = 2 * (q1 * q3 - q0 * q2);/*矩阵(3,1)项*/
	vecyZ = 2 * (q0 * q1 + q2 * q3);/*矩阵(3,2)项*/
	veczZ = q0s - q1s - q2s + q3s;	/*矩阵(3,3)项*/
	
	if (vecxZ>1) vecxZ=1;
	if (vecxZ<-1) vecxZ=-1;
	
	/*计算roll pitch yaw 欧拉角*/
	imu_data.angle.y = asinf(vecxZ) * RAD_TO_DEG; 
	imu_data.angle.x = atan2f(vecyZ, veczZ) * RAD_TO_DEG;
	imu_data.angle.z = -atan2f(R21, R11) * RAD_TO_DEG;
	
	//state->acc.z= tempacc.x* vecxZ + tempacc.y * vecyZ + tempacc.z * veczZ - baseZacc;	/*Z轴加速度(去除重力加速度)*/
}

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x)	/*快速开平方求倒*/
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
#endif
#if 0
void AHRS_InitAlignment(void)
{
	uint8_t i;
	Vector3i_t accTemp, magTemp;
	int32_t accSum[3] = {0, 0, 0}, magSum[3] = {0, 0, 0};
	
	if(!ahrs.initAlignment){	
		if(GetSysTime_ms() > 500){	//系统上电500ms后进行初始对准
			//Roll和Pitch对准，读10次加速度取平均值
			for(i=0;i<10;i++){
				MPU6000AccRead(&accTemp, 0);
	
				accSum[0] += accTemp.x;
				accSum[1] += accTemp.y;
				accSum[2] += accTemp.z;
			}
			ahrs.vectorRollPitch.x = accSum[0] / 10;
			ahrs.vectorRollPitch.y = accSum[1] / 10;
			ahrs.vectorRollPitch.z = accSum[2] / 10;		

		    ahrs.vectorYaw.x = 100;
		    ahrs.vectorYaw.y = 0;
			ahrs.vectorYaw.z = 0;
		
			
			ahrs.initAlignment = 1;		
		}
	}
}

void rotateV(Vector3f_t *v, Vector3f_t delta)
{
	Vector3f_t v_tmp = *v;

	float mat[3][3];
	float cosx, sinx, cosy, siny, cosz, sinz;
	float coszcosx, coszcosy, sinzcosx, coszsinx, sinzsinx;

	cosx = cosf(delta.x);
	sinx = sinf(delta.x);
	cosy = cosf(delta.y);
	siny = sinf(delta.y);
	cosz = cosf(delta.z);
	sinz = sinf(delta.z);

	coszcosx = cosz * cosx;
	coszcosy = cosz * cosy;
	sinzcosx = sinz * cosx;
	coszsinx = sinx * cosz;
	sinzsinx = sinx * sinz;

	mat[0][0] = coszcosy;
	mat[0][1] = -cosy * sinz;
	mat[0][2] = siny;
	mat[1][0] = sinzcosx + (coszsinx * siny);
	mat[1][1] = coszcosx - (sinzsinx * siny);
	mat[1][2] = -sinx * cosy;
	mat[2][0] = (sinzsinx) - (coszcosx * siny);
	mat[2][1] = (coszsinx) + (sinzcosx * siny);
	mat[2][2] = cosy * cosx;

	v->x = v_tmp.x * mat[0][0] + v_tmp.y * mat[1][0] + v_tmp.z * mat[2][0];
	v->y = v_tmp.x * mat[0][1] + v_tmp.y * mat[1][1] + v_tmp.z * mat[2][1];
	v->z = v_tmp.x * mat[0][2] + v_tmp.y * mat[1][2] + v_tmp.z * mat[2][2];
}

void IMU_DCM(Vector3i_t gyro, Vector3f_t acc, Vector3i_t mag)
{
	static Vector3f_t delta;	
	static Vector3f_t lastGyro,lastGyroRaw;
	static float lastAccMag = 0;
	Vector3f_t gyro_dps;
	Vector3f_t VectorYawTemp;
	
	Vector3f_t anglerad;
	
	static uint32_t previousT;
	float	deltaT = (GetSysTime_us() - previousT) * 0.000001f;	
	previousT = GetSysTime_us();	
	
	//姿态初始对准
	AHRS_InitAlignment();
		
	//计算角加速度
	ahrs.gyroAccel.x = (gyro.x - lastGyroRaw.x) / deltaT * 0.006f;
	ahrs.gyroAccel.y = (gyro.y - lastGyroRaw.y) / deltaT * 0.006f;
	ahrs.gyroAccel.z = (gyro.z - lastGyroRaw.z) / deltaT * 0.006f;
	lastGyroRaw.x = gyro.x;
	lastGyroRaw.y = gyro.y;
	lastGyroRaw.z = gyro.z;
	
	//将角速度转化为度每秒的单位
	gyro_dps = Get_Gyro_In_Dps(gyro);	
	//角速度积分
	delta.x = (gyro_dps.x + lastGyro.x ) * 0.5f * deltaT; 
	delta.y = (gyro_dps.y + lastGyro.y) * 0.5f * deltaT; 
	delta.z = (gyro_dps.z + lastGyro.z) * 0.5f * deltaT;
	lastGyro = gyro_dps;


	
	rotateV(&ahrs.vectorRollPitch, delta);


	//互补滤波修正俯仰&横滚误差
	if(GetSysTime_ms() > 3000){
		if (80 < ahrs.accMag && ahrs.accMag < 120){
			ahrs.vectorRollPitch.x = Complementary_Filter_1st(ahrs.vectorRollPitch.x, (float)acc.x, ahrs.filter.gyro_cf);
			ahrs.vectorRollPitch.y = Complementary_Filter_1st(ahrs.vectorRollPitch.y, (float)acc.y, ahrs.filter.gyro_cf);
			ahrs.vectorRollPitch.z = Complementary_Filter_1st(ahrs.vectorRollPitch.z, (float)acc.z, ahrs.filter.gyro_cf);		
		}
	}
	else{
			ahrs.vectorRollPitch.x = Complementary_Filter_1st(ahrs.vectorRollPitch.x, (float)acc.x, 0.999f);
			ahrs.vectorRollPitch.y = Complementary_Filter_1st(ahrs.vectorRollPitch.y, (float)acc.y, 0.999f);
			ahrs.vectorRollPitch.z = Complementary_Filter_1st(ahrs.vectorRollPitch.z, (float)acc.z, 0.999f);				
	}


	//航向倾斜矫正
	VectorYawTemp = ahrs.vectorYaw;
	anglerad.x = -radians(ahrs.angle[ROLL]  - (float)sensor_cali.angle.offset.x * 0.1f);
	anglerad.y = -radians(ahrs.angle[PITCH]  - (float)sensor_cali.angle.offset.y * 0.1f);
	anglerad.z = 0;	
	rotateV(&VectorYawTemp, anglerad);	
	
	imu_data.angle.x = degrees(atan2f(ahrs.vectorRollPitch.y, sqrtf(ahrs.vectorRollPitch.x * ahrs.vectorRollPitch.x + ahrs.vectorRollPitch.z * ahrs.vectorRollPitch.z)));
	imu_data.angle.y = degrees(atan2f(-ahrs.vectorRollPitch.x, sqrtf(ahrs.vectorRollPitch.y * ahrs.vectorRollPitch.y + ahrs.vectorRollPitch.z * ahrs.vectorRollPitch.z)));
	imu_data.angle.z = degrees(atan2f(VectorYawTemp.y,VectorYawTemp.x)) + ahrs.magDeclination;  //磁偏角补偿，深圳磁偏角为-2度
	//ahrs.angle[YAW]= degrees(atan2f(mag.y,mag.x)) + ahrs.magDeclination;  //磁偏角补偿，深圳磁偏角为-2度
	
	if(ahrs.angle[YAW] > 360)
		ahrs.angle[YAW] -= 360;
	else if(ahrs.angle[YAW] < 0)
		ahrs.angle[YAW] += 360;		
}
#endif
