#include <stdio.h>
#include <sys/stat.h>
#include "Drv_debug.h"

int _fstat (int fd, struct stat *pStat)
{
	pStat->st_mode = S_IFCHR;
	return 0;
}

int _close(int fd)
{
	return -1;
}

int _write (int fd, char *ptr, int len)
{
	int i = 0;

	if (fd > 2)
		return -1;

	while(*ptr && (i < len))\
	{
		log_txd(*ptr);
		i++;
		ptr++;
	}
   
 	return len;
}

int _isatty (int fd)
{
	return 1;
}

int _lseek(int fd, int offset, int fromwhere)
{
	
}

int _read (int fd, char *pBuffer, int size)
{

}

caddr_t _sbrk(int increment)
{
	extern char end asm("end");
	register char *pStack asm("sp");
	static char *s_pHeapEnd;
	if (!s_pHeapEnd)
		s_pHeapEnd = &end;

	if (s_pHeapEnd + increment > pStack)
		return (caddr_t)-1;

	char *pOldHeapEnd = s_pHeapEnd;
	s_pHeapEnd += increment;
	return (caddr_t)pOldHeapEnd;
}

void _exit(int status)
{
	while(1);
}

