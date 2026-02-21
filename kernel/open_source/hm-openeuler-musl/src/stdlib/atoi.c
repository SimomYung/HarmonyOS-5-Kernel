#include <stdlib.h>
#include <ctype.h>

int atoi(const char *s)
{
	/* atoi() shall be equivalent to (int)strtol(str, (char **)NULL, 10) */
	return (int) strtol(s, (char **)NULL, 10);
}
