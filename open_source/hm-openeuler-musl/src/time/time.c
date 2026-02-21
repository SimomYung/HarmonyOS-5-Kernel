#include <time.h>
#include "syscall.h"

time_t time(time_t *t)
{
	struct timespec ts;
	int ret = __clock_gettime(CLOCK_REALTIME, &ts);
	if (ret) return ret;
	if (t) *t = ts.tv_sec;
	return ts.tv_sec;
}
