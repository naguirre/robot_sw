#include <tick/tick.h>

#include <stdlib.h>
#include <sys/time.h>

uint32_t Tick::GetMillis(void)
{
    struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((uint32_t)(tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
} 