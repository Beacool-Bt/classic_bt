#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "board.h"
#include "config.h"

typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;
uint16_t interval_ticks;
uint32_t last_run;
}sched_task_t;

void Scheduler_Setup(void);
void Scheduler_Run(void);

#endif











