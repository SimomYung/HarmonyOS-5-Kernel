#include <time.h>

char *asctime(const struct tm *tm)
{
	/*
	 * The maximum value of int is 4294967295, and its length is 6 bytes
	 * longer than the current year (2022). In order to avoid overflow,
	 * the original value (26) should plus 7 (including a terminator).
	 */
	static char buf[33];
	return __asctime_r(tm, buf);
}
