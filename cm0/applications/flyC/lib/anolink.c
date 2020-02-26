#include "anolink.h"
#include "sensor.h"
#include "yc11xx_uart.h"
#include "drv_wireless.h"

#define LOCAL_RF

static	uint8_t data_to_send[50];
static	uint8_t data_to_recv[32];

static	void Send_Status(void);
static	void Send_Senser(void);
static	void Send_RCData(void);
static	void Send_MotoPWM(void);
static	void Send_PID1(void);
static	void Send_PID2(void);
static	void Send_Check(uint16_t check);

static	void Send_Data(uint8_t *dataToSend , uint8_t length);
	
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

	struct flag{
		uint8_t Send_Status;
		uint8_t Send_Senser;
		uint8_t Send_PID1;
		uint8_t Send_PID2;
		uint8_t Send_PID3;
		uint8_t Send_RCData;
		uint8_t Send_Offset;
		uint8_t Send_MotoPwm;
	}f;

void Data_Receive_Anl(uint8_t *data_buf,uint8_t num)
{
	uint8_t sum = 0;
	
	for(uint8_t i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头
	
	config.f.failsafe = 0;
/////////////////////////////////////////////////////////////////////////////////////
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
			sensor.Acc_CALIBRATED = 1;
		if(*(data_buf+4)==0X02)
			sensor.Gyro_CALIBRATED = 1;
		if(*(data_buf+4)==0X03)
		{
			sensor.Acc_CALIBRATED = 1;		
			sensor.Gyro_CALIBRATED = 1;			
		}
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
 			f.Send_PID1 = 1;
 			f.Send_PID2 = 1;
 			f.Send_PID3 = 1;
		}
		if(*(data_buf+4)==0X02)
		{
			
		}
	}

	if(*(data_buf+2)==0X03)
	{
		rc_data.rawData[THROTTLE] = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
		rc_data.rawData[YAW] = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
		rc_data.rawData[ROLL] = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
		rc_data.rawData[PITCH] =2760 - (vs16)(*(data_buf+10)<<8)|*(data_buf+11);
		rc_data.rawData[AUX1] = (vs16)(*(data_buf+12)<<8)|*(data_buf+13);
		rc_data.rawData[AUX2] = (vs16)(*(data_buf+14)<<8)|*(data_buf+15);
		rc_data.rawData[AUX3] = (vs16)(*(data_buf+16)<<8)|*(data_buf+17);
		rc_data.rawData[AUX4] = (vs16)(*(data_buf+18)<<8)|*(data_buf+19);
	}

	if(*(data_buf+2)==0X10)								//PID1
	{
		pid[PIDROLL].kP = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5)) / 100;
		pid[PIDROLL].kI = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7)) / 100;
		pid[PIDROLL].kD = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9)) / 1000;
		pid[PIDPITCH].kP = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11)) / 100;
		pid[PIDPITCH].kI = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13)) / 100;
		pid[PIDPITCH].kD = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15)) / 1000;
		pid[PIDYAW].kP = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17)) / 100;
		pid[PIDYAW].kI = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19)) / 100;
		pid[PIDYAW].kD = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21)) / 1000;
		Send_Check(sum);
	}
	if(*(data_buf+2)==0X11)								//PID2
	{
		pid[PIDALT].kP = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5)) / 100;
		pid[PIDALT].kI = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7)) / 100;
		pid[PIDALT].kD = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9)) / 1000;
		pid[PIDLEVEL].kP = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11)) / 100;
		pid[PIDLEVEL].kI = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13)) / 100;
		pid[PIDLEVEL].kD = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15)) / 1000;
		pid[PIDMAG].kP = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17)) / 100;
		pid[PIDMAG].kI = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19)) / 100;
		pid[PIDMAG].kD = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21)) / 1000;
		Send_Check(sum);
	}
	if(*(data_buf+2)==0X12)								//PID3
	{
		Send_Check(sum);
		Param_SavePID();
	}
		if(*(data_buf+2)==0X13)								//PID4
	{
		Send_Check(sum);
	}
		if(*(data_buf+2)==0X14)								//PID5
	{
		Send_Check(sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		Send_Check(sum);
	}

	if(*(data_buf+2)==0X16)								//OFFSET
	{

	}

/////////////////////////////////////////////////////////////////////////////////////////////////
	if(*(data_buf+2)==0x18)					
	{

	}
}
void ano_Receive_Prepare(uint8_t data)
{
	static uint8_t _data_len = 0, _data_cnt = 0;
	static uint8_t state = 0;
	

	
	// 下行数据: 匿名地面站数据解析
	if(state==0 && data==0xAA)
	{
		state=1;
		data_to_recv[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		data_to_recv[1]=data;
	}
	else if(state==2&&data<=0xFF)
	{
		state=3;
		data_to_recv[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		data_to_recv[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		data_to_recv[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		data_to_recv[4+_data_cnt]=data;
		
		Data_Receive_Anl(data_to_recv,_data_cnt+5);
	}
	else
		state = 0;
}
uint8_t recv_buffer[256];

void Check_Event(void)
{

	uint8_t i = 0;
#ifdef LOCAL_RF
	ws_rx_data_single();
	uint8_t len = (uint8_t)ws_rx_data(recv_buffer);
#else
	uint8_t len = USART_ReadDatatoBuff(UARTB, recv_buffer,0);
#endif
	 for(i=0;i < len;i++)
	 	{
	    ano_Receive_Prepare(recv_buffer[i]);
	 	}
#ifdef ANO_DT_USE_NRF24l01	
	NRF_Check_Event();
#endif
	
}

void Data_Exchange(void)
{
	static uint8_t cnt = 0;
	
	switch(cnt)
	{
		case 1: 
			f.Send_RCData = 1;
			break;
		case 2:
			f.Send_MotoPwm = 1;
			break;
		case 30:
			cnt = 0;
			break;
		default:
			if(cnt%3)
				f.Send_Senser = 1;	
			else
				f.Send_Status = 1;
						
	}
	cnt++;
	
	if(f.Send_Status){
		f.Send_Status = 0;
		Send_Status();
	}	
	if(f.Send_Senser){
		f.Send_Senser = 0;
		Send_Senser();
	}	
	if(f.Send_RCData){
		f.Send_RCData = 0;
		Send_RCData();
	}		
	if(f.Send_MotoPwm){
		f.Send_MotoPwm = 0;
		Send_MotoPWM();
	}	
	if(f.Send_PID1){
		f.Send_PID1 = 0;
		Send_PID1();
	}	
	if(f.Send_PID2){
		f.Send_PID2 = 0;
		Send_PID2();
	}	
}
extern uint32_t idleTime;

void Send_Status(void)
{
	uint8_t _cnt=0,i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	vs16 _temp;
	_temp = (int)(imu_data.angle.x*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(imu_data.angle.y*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(imu_data.angle.z*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	vs32 _temp2 = idleTime;//UltraAlt * 100;
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	//
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=config.f.ARMED;
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}


void Send_Senser(void)
{
	uint8_t _cnt=0;
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	_temp = imu_data.Acc.x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = imu_data.Acc.y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = imu_data.Acc.z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = imu_data.Gyro.x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = imu_data.Gyro.y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = imu_data.Gyro.z;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (vs16)idleTime;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	data_to_send[_cnt++] = 0;
	data_to_send[_cnt++] = 0;
	data_to_send[_cnt++] = 0;
	data_to_send[_cnt++] = 0;
	
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	Send_Data(data_to_send, _cnt);
}

void Send_RCData(void)
{
	uint8_t _cnt=0,i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[THROTTLE]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[THROTTLE]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[YAW]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[YAW]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[ROLL]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[ROLL]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[PITCH]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[PITCH]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[AUX1]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[AUX1]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[AUX2]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[AUX2]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[AUX3]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[AUX3]);
	data_to_send[_cnt++]=BYTE1(rc_data.rawData[AUX4]);
	data_to_send[_cnt++]=BYTE0(rc_data.rawData[AUX4]);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}

void Send_MotoPWM(void)
{
	uint8_t _cnt=0,i=0;
	uint16_t Moto_PWM[6];
	Motor_GetPWM(Moto_PWM);
	
	for(i=0;i<6;i++)
		Moto_PWM[i] -= 1000;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Moto_PWM[0]);
	data_to_send[_cnt++]=BYTE0(Moto_PWM[0]);
	data_to_send[_cnt++]=BYTE1(Moto_PWM[1]);
	data_to_send[_cnt++]=BYTE0(Moto_PWM[1]);
	data_to_send[_cnt++]=BYTE1(Moto_PWM[2]);
	data_to_send[_cnt++]=BYTE0(Moto_PWM[2]);
	data_to_send[_cnt++]=BYTE1(Moto_PWM[3]);
	data_to_send[_cnt++]=BYTE0(Moto_PWM[3]);
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
    data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}

void Send_PID1(void)
{
	uint8_t _cnt=0,i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = pid[PIDROLL].kP * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDROLL].kI * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDROLL].kD * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDPITCH].kP * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDPITCH].kI * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDPITCH].kD * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDYAW].kP * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDYAW].kI * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDYAW].kD * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	Send_Data(data_to_send, _cnt);
}

void Send_PID2(void)
{
	uint8_t _cnt=0,i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x11;
	data_to_send[_cnt++]=0;
	
	vs16 _temp;
	_temp = pid[PIDALT].kP * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDALT].kI * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDALT].kD * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDLEVEL].kP * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDLEVEL].kI * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDLEVEL].kD * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDMAG].kP * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDMAG].kI * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = pid[PIDMAG].kD * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	Send_Data(data_to_send, _cnt);
}

void Send_Check(uint16_t check)
{
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xF0;
	data_to_send[3]=3;
	data_to_send[4]=0xBA;
	
	data_to_send[5]=BYTE1(check);
	data_to_send[6]=BYTE0(check);
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<7;i++)
		sum += data_to_send[i];
	
	data_to_send[7]=sum;

	Send_Data(data_to_send, 8);
}

void Send_Data(uint8_t *dataToSend , uint8_t length)
{
    //printf("send data\r\n");
 #ifdef LOCAL_RF
	ws_tx_data(dataToSend,length);
 #else
    while(length--)
    	{
	USART_SendData(UARTB, *dataToSend++);
    	}
 #endif
#ifdef ANO_DT_USE_NRF24l01
	NRF_TxPacket(data_to_send,length);
#endif
	
}


void Failsafe_Check(void)
{
		static uint8_t failsafeCnt = 0;
		if(failsafeCnt > 30)
		{
			failsafeCnt = 0;
			if(!config.f.failsafe)
				config.f.failsafe = 1;
			else
			{	
				config.f.ARMED = 0;
			}
		}
		failsafeCnt++;	
}
