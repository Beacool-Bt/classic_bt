
#include "config.h"

int main(void)
{
	//初始化飞控板的硬件设置
	board_init();
	
	//初始化参数
	Param_Init();
	
	//初始化IMU（惯性测量单元）
	IMU_Init();	

	FlyControl_Init();
	
	Scheduler_Setup();
	
    Scheduler_Run();
	
 	return 0;
}

