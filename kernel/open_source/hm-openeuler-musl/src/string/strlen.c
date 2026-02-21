#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

size_t strlen(const char *s)
{
	const char *a = s;
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
	for (; (uintptr_t)s % ALIGN; s++) if (!*s) return s-a;
	for (w = (const void *)s; !HASZERO(*w); w++);
	s = (const void *)w;
#endif
#endif
#endif
	for (; *s; s++);
	return s-a;
}
