#include <stdio.h>
#include <string.h>


void _assert_handler(const char* func, uint16_t line)
{
	//printf("Assert trigger at: %s, line :%d\n",func,line);
	while(1);
}

void _delay_ms(uint16_t ms)
{
	
}

