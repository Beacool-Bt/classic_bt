#include "board.h"
#include "drv_debug.h"


/*底层硬件初始化*/
void board_init(void)
{
	ws_init();
	
	log_init();      /* 初始化LOG输出 */
	
	printf("main start\r\n");
	
	systick_init();     /* 初始化systick */	
	
	led_init();			/* 初始化led */

	i2c_init();          /* 初始化I2C */
		
	motors_hw_init();    /* 电机初始化 */
	printf("board init end\r\n");
	
}


