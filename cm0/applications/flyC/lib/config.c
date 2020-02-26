#include "config.h"
#include "drv_led.h"



config_t config;

//Ö¸Ê¾µÆ
void Pilot_Light(void)
{
	static uint8_t cnt = 0;
	
	if(config.f.ARMED)
	{
		cnt++;
		switch(cnt)
		{
			case 1:
				led_on();
				break;
			case 10:
				led_off();
				break;
			case 20:
				cnt = 0;
				break;
		}
	}
	else
	{
		led_on(); 
	}
	
}

