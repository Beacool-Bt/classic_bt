#include "imu.h"
#include "sensor.h"
#include "mpu6050.h"
imu_data_t imu_data;

int32_t accRatio;
	

	//�˲���������ʼ��
static	void Filter_Init(void);

//IMU��ʼ��
void IMU_Init()
{

    imu_data.initAlignment = 0;	
	//�˲���������ʼ��
	Filter_Init();
	//��������ʼ��
	Sensor_Init();	

}

//���´���������
void IMU_UpdateSensor()
{
   Vector3i_t temp;
	//��ȡ���ٶ�
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
	//��ȡ���ٶȣ���λΪ��ÿ��
	imu_data.Gyro = Sensor_GetGyro();
	 temp= imu_data.Gyro;
	imu_data.Gyro.x = temp.y;
	imu_data.Gyro.y = -temp.x;
	imu_data.Gyrof.x = imu_data.Gyro.x*MPU6050G_S2000DPS;
	imu_data.Gyrof.y = imu_data.Gyro.y*MPU6050G_S2000DPS;
	imu_data.Gyrof.z = imu_data.Gyro.z*MPU6050G_S2000DPS;
	//��ȡ���ٶȲ���ֵ
	imu_data.Acc = Sensor_GetAcc();
	 temp= imu_data.Acc;
    imu_data.Acc.x = temp.y;
	imu_data.Acc.y = -temp.x;
	imu_data.Accf.x = imu_data.Acc.x*MPU6050A_8mg;
	imu_data.Accf.y = imu_data.Acc.y*MPU6050A_8mg;
	imu_data.Accf.z = imu_data.Acc.z*MPU6050A_8mg;
	 	
}


//�����������̬
void IMU_GetAttitude()
{
	float deltaT;
	static uint32_t previousT=0;
	uint32_t tnow = 0;
	
	//���ٶȶ��׵�ͨ�˲�	
	imu_data.gyro_Lpf = LowPassFilter_2nd(&imu_data.Gyro_lpf_2nd, imu_data.Gyrof);
	//���ٶȶ��׵�ͨ�˲�
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
	
	
	//�����˲���ϵ������
	imu_data.gyro_cf= Complementary_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);
	//���ٶȵ�ͨ�˲�����������
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT, &imu_data.Acc_lpf_2nd);
	//���ٶȶ��׵�ͨ�˲���ϵ������
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_LPF_CUT, &imu_data.Gyro_lpf_2nd);		
}



float Kp = 0.4f;		/*��������*/
float Ki = 0.001f;		/*��������*/
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;		/*��������ۼ�*/

static float q0 = 1.0f;	/*��Ԫ��*/
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	

//static float baseZacc = 1.0;		/*��̬Z����ٶ�*/

static float invSqrt(float x);	/*���ٿ�ƽ����*/

void IMU_Update(Vector3f_t acc, Vector3f_t gyro, float dt)	/*�����ں� �����˲�*/
{
	float normalise;
	float ex, ey, ez;
	float q0s, q1s, q2s, q3s;	/*��Ԫ����ƽ��*/
	static float R11,R21;		/*����(1,1),(2,1)��*/
	static float vecxZ, vecyZ, veczZ;	/*��������ϵ�µ�Z��������*/
	float halfT =0.5f * dt;
//	Vector3f_t tempacc =acc;

	gyro.x = gyro.x * DEG_TO_RAD;	/* ��ת���� */
	gyro.y = gyro.y * DEG_TO_RAD;
	gyro.z = gyro.z * DEG_TO_RAD;

	/* ĳһ��������ٶȲ�Ϊ0 */
	if((acc.x != 0.0f) || (acc.y != 0.0f) || (acc.z != 0.0f))
	{
		/*��λ�����ټƲ���ֵ*/
		normalise = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
		acc.x *= normalise;
		acc.y *= normalise;
		acc.z *= normalise;

		/*���ټƶ�ȡ�ķ������������ټƷ���Ĳ�ֵ����������˼���*/
		ex = (acc.y * veczZ - acc.z * vecyZ);
		ey = (acc.z * vecxZ - acc.x * veczZ);
		ez = (acc.x * vecyZ - acc.y * vecxZ);
		
		/*����ۼƣ�����ֳ������*/
		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;
		
		/*�ò���������PI����������ƫ�����������ݶ����е�ƫ����*/
		gyro.x += Kp * ex + exInt;
		gyro.y += Kp * ey + eyInt;
		gyro.z += Kp * ez + ezInt;
	}
	/* һ�׽����㷨����Ԫ���˶�ѧ���̵���ɢ����ʽ�ͻ��� */
	float q0Last = q0;
	float q1Last = q1;
	float q2Last = q2;
	float q3Last = q3;
	q0 += (-q1Last * gyro.x - q2Last * gyro.y - q3Last * gyro.z) * halfT;
	q1 += ( q0Last * gyro.x + q2Last * gyro.z - q3Last * gyro.y) * halfT;
	q2 += ( q0Last * gyro.y - q1Last * gyro.z + q3Last * gyro.x) * halfT;
	q3 += ( q0Last * gyro.z + q1Last * gyro.y - q2Last * gyro.x) * halfT;
	
	/*��λ����Ԫ��*/
	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	/*��Ԫ����ƽ��*/
	q0s = q0 * q0;
	q1s = q1 * q1;
	q2s = q2 * q2;
	q3s = q3 * q3;
	
	R11 = q0s + q1s - q2s - q3s;	/*����(1,1)��*/
	R21 = 2 * (q1 * q2 + q0 * q3);	/*����(2,1)��*/

	/*��������ϵ�µ�Z��������*/
	vecxZ = 2 * (q1 * q3 - q0 * q2);/*����(3,1)��*/
	vecyZ = 2 * (q0 * q1 + q2 * q3);/*����(3,2)��*/
	veczZ = q0s - q1s - q2s + q3s;	/*����(3,3)��*/
	
	if (vecxZ>1) vecxZ=1;
	if (vecxZ<-1) vecxZ=-1;
	
	/*����roll pitch yaw ŷ����*/
	imu_data.angle.y = asinf(vecxZ) * RAD_TO_DEG; 
	imu_data.angle.x = atan2f(vecyZ, veczZ) * RAD_TO_DEG;
	imu_data.angle.z = -atan2f(R21, R11) * RAD_TO_DEG;
	
	//state->acc.z= tempacc.x* vecxZ + tempacc.y * vecyZ + tempacc.z * veczZ - baseZacc;	/*Z����ٶ�(ȥ���������ٶ�)*/
}

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x)	/*���ٿ�ƽ����*/
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
		if(GetSysTime_ms() > 500){	//ϵͳ�ϵ�500ms����г�ʼ��׼
			//Roll��Pitch��׼����10�μ��ٶ�ȡƽ��ֵ
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
	
	//��̬��ʼ��׼
	AHRS_InitAlignment();
		
	//����Ǽ��ٶ�
	ahrs.gyroAccel.x = (gyro.x - lastGyroRaw.x) / deltaT * 0.006f;
	ahrs.gyroAccel.y = (gyro.y - lastGyroRaw.y) / deltaT * 0.006f;
	ahrs.gyroAccel.z = (gyro.z - lastGyroRaw.z) / deltaT * 0.006f;
	lastGyroRaw.x = gyro.x;
	lastGyroRaw.y = gyro.y;
	lastGyroRaw.z = gyro.z;
	
	//�����ٶ�ת��Ϊ��ÿ��ĵ�λ
	gyro_dps = Get_Gyro_In_Dps(gyro);	
	//���ٶȻ���
	delta.x = (gyro_dps.x + lastGyro.x ) * 0.5f * deltaT; 
	delta.y = (gyro_dps.y + lastGyro.y) * 0.5f * deltaT; 
	delta.z = (gyro_dps.z + lastGyro.z) * 0.5f * deltaT;
	lastGyro = gyro_dps;


	
	rotateV(&ahrs.vectorRollPitch, delta);


	//�����˲���������&������
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


	//������б����
	VectorYawTemp = ahrs.vectorYaw;
	anglerad.x = -radians(ahrs.angle[ROLL]  - (float)sensor_cali.angle.offset.x * 0.1f);
	anglerad.y = -radians(ahrs.angle[PITCH]  - (float)sensor_cali.angle.offset.y * 0.1f);
	anglerad.z = 0;	
	rotateV(&VectorYawTemp, anglerad);	
	
	imu_data.angle.x = degrees(atan2f(ahrs.vectorRollPitch.y, sqrtf(ahrs.vectorRollPitch.x * ahrs.vectorRollPitch.x + ahrs.vectorRollPitch.z * ahrs.vectorRollPitch.z)));
	imu_data.angle.y = degrees(atan2f(-ahrs.vectorRollPitch.x, sqrtf(ahrs.vectorRollPitch.y * ahrs.vectorRollPitch.y + ahrs.vectorRollPitch.z * ahrs.vectorRollPitch.z)));
	imu_data.angle.z = degrees(atan2f(VectorYawTemp.y,VectorYawTemp.x)) + ahrs.magDeclination;  //��ƫ�ǲ��������ڴ�ƫ��Ϊ-2��
	//ahrs.angle[YAW]= degrees(atan2f(mag.y,mag.x)) + ahrs.magDeclination;  //��ƫ�ǲ��������ڴ�ƫ��Ϊ-2��
	
	if(ahrs.angle[YAW] > 360)
		ahrs.angle[YAW] -= 360;
	else if(ahrs.angle[YAW] < 0)
		ahrs.angle[YAW] += 360;		
}
#endif
