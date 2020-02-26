#ifndef __SYSTICK_H
#define __SYSTICK_H
#include <stdbool.h>
#include "yc_drv_common.h"
#include "core_cm0.h"

extern uint32_t tick_count;
void systick_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t systick_get_us(void);
uint32_t systick_get_ms(void);

#endif
