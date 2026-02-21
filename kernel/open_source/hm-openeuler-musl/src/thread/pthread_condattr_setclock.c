#include "pthread_impl.h"

int pthread_condattr_setclock(pthread_condattr_t *a, clockid_t clk)
{
	if (clk != CLOCK_MONOTONIC && clk != CLOCK_REALTIME)
		return EINVAL;
	a->__attr &= 0x80000000;
	a->__attr |= clk;
	return 0;
}
