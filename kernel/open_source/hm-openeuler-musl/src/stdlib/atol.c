#include <stdlib.h>
#include <ctype.h>

long atol(const char *s)
{
	/* atol() shall be equivalent to strtol(str, (char **)NULL, 10) */
	return strtol(s, (char **)NULL, 10);
}
