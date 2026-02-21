#include <time.h>
#include <stdio.h>
#include <langinfo.h>
#include <errno.h>
#include "locale_impl.h"
#include "atomic.h"

char *__asctime_r(const struct tm *restrict tm, char *restrict buf)
{
	if (tm == NULL || buf == NULL) {
		errno = EINVAL;
		return NULL;
	}
	if (snprintf(buf, 26, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
		(tm->tm_wday >= 0 && tm->tm_wday < 7) ? __nl_langinfo_l(ABDAY_1+tm->tm_wday, C_LOCALE) : "???",
		(tm->tm_mon >= 0 && tm->tm_mon < 12) ? __nl_langinfo_l(ABMON_1+tm->tm_mon, C_LOCALE) : "???",
		tm->tm_mday, tm->tm_hour,
		tm->tm_min, tm->tm_sec,
		1900 + tm->tm_year) >= 26)
	{
		errno = EOVERFLOW;
		return NULL;
	}
	return buf;
}

weak_alias(__asctime_r, asctime_r);
