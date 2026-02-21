#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *__strchrnul(const char *s, int c)
{
	c = (unsigned char)c;
	if (!c) return (char *)s + strlen(s);
/*
 * Because "HASZERO(*ws^k)" read 8 bytes every times, so the following
 * code may cause illegal read operation. This error will be found
 * by KASAN and cause an exception. To make sure that the code runs properly,
 * the below code will not be compiled.
 */
#ifndef LIBC_ENABLE_KASAN
#ifndef LIBC_ENABLE_KASAN_SYS
#ifdef __GNUC__
	typedef size_t __attribute__((__may_alias__)) word;
	const word *w;
	for (; (uintptr_t)s % ALIGN; s++)
		if (!*s || *(unsigned char *)s == c) return (char *)s;
	size_t k = ONES * c;
	for (w = (void *)s; !HASZERO(*w) && !HASZERO(*w^k); w++);
	s = (void *)w;
#endif
#endif
#endif
	for (; *s && *(unsigned char *)s != c; s++);
	return (char *)s;
}

weak_alias(__strchrnul, strchrnul);
