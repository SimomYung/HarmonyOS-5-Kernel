#include <stdlib.h>
#include <ctype.h>

long long atoll(const char *s)
{
	/* atoll() shall be equivalent to strtoll(str, (char **)NULL, 10) */
	return strtoll(s, (char **)NULL, 10);
}
