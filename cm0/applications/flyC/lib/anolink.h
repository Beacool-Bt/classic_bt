#ifndef __ANOLINK_H
#define __ANOLINK_H

#include "config.h"


void Data_Receive_Anl(uint8_t *data_buf,uint8_t num);
	//����Ƿ��н��յ���������
void Check_Event(void);
	//���ݷ���
void Data_Exchange(void);
	//ʧ�ر������
void Failsafe_Check(void);


#endif









