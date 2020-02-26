#include "board.h"
#include "drv_debug.h"


/*�ײ�Ӳ����ʼ��*/
void board_init(void)
{
	ws_init();
	
	log_init();      /* ��ʼ��LOG��� */
	
	printf("main start\r\n");
	
	systick_init();     /* ��ʼ��systick */	
	
	led_init();			/* ��ʼ��led */

	i2c_init();          /* ��ʼ��I2C */
		
	motors_hw_init();    /* �����ʼ�� */
	printf("board init end\r\n");
	
}


