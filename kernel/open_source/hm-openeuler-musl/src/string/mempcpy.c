#define _GNU_SOURCE
#include <string.h>

static void *__mempcpy(void *dest, const void *src, size_t n)
{
	return (char *)memcpy(dest, src, n) + n;
}

weak_alias(__mempcpy, mempcpy);
