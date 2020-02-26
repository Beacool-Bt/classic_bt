
#include "scheduler.h"


	uint32_t idleTime;
	

static void Loop_500Hz(void)	//2ms执行一次
{	

    uint32_t tnow = systick_get_us();
	//更新传感器数据	
	IMU_UpdateSensor();		
	
	//计算飞行器姿态
	IMU_GetAttitude();
	
	//飞行器姿态内环控制
	Attitude_Inner_Loop();	
	idleTime = systick_get_us() - tnow;
}

static void Loop_200Hz(void)	//5ms执行一次
{
	//飞行器姿态外环控制
  Attitude_Outter_Loop();	
}

static void Loop_100Hz(void)	//10ms执行一次
{
    //检查是否有接收到无线数据
	Check_Event();
	//发送飞行器数据
	Data_Exchange();
}

static void Loop_50Hz(void)	//20ms执行一次
{
	//遥控通道数据处理
	RC_CalCommand();
	
	//摇杆位置检查
	RC_CheckSticks();
	
	//失控保护检查
	Failsafe_Check();
	
	//LED指示灯控制
	Pilot_Light();
	//printf("50hz run\r\n");
	//ws_rx_data();
}

static sched_task_t sched_tasks[] = 
{
// {Loop_1000Hz,1000,  0, 0},
 {Loop_500Hz , 500,  0, 0},
 {Loop_200Hz , 200,  0, 0},
 {Loop_100Hz , 100,  0, 0},
 {Loop_50Hz  ,  50,  0, 0},
};

#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

void Scheduler_Setup(void)
{
 uint8_t index = 0;
	
 //初始化任务表
 for(index=0;index < TASK_NUM;index++)
 {
  sched_tasks[index].interval_ticks = TICK_PER_SECOND/sched_tasks[index].rate_hz;
	if(sched_tasks[index].interval_ticks < 1)
	{
	 sched_tasks[index].interval_ticks = 1;
	}
 }
}
void Scheduler_Run(void)
{
 uint8_t index = 0;
	while(1)
	{
   for(index=0;index < TASK_NUM;index++)
   {
    uint32_t tnow = systick_get_ms();
    if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
    {
	   sched_tasks[index].last_run = tnow;
	   sched_tasks[index].task_func();
	  }	 
   }
  }
}
