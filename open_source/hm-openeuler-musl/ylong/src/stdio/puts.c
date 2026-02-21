#include "stdio_impl.h"

int puts(const char *s)
{
	return printf("%s\n", s);
}
