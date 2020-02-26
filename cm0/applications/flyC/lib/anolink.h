#ifndef __ANOLINK_H
#define __ANOLINK_H

#include "config.h"


void Data_Receive_Anl(uint8_t *data_buf,uint8_t num);
	//检查是否有接收到无线数据
void Check_Event(void);
	//数据发送
void Data_Exchange(void);
	//失控保护检查
void Failsafe_Check(void);


#endif









