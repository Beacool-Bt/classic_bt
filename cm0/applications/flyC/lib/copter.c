
#include "config.h"

int main(void)
{
	//��ʼ���ɿذ��Ӳ������
	board_init();
	
	//��ʼ������
	Param_Init();
	
	//��ʼ��IMU�����Բ�����Ԫ��
	IMU_Init();	

	FlyControl_Init();
	
	Scheduler_Setup();
	
    Scheduler_Run();
	
 	return 0;
}

