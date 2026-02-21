#include <string.h>
#include <stdint.h>
#include <limits.h>

#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *memchr(const void *src, int c, size_t n)
{
	const unsigned char *s = src;
	c = (unsigned char)c;
/*
 * Because "HASZERO(*ws^k)" read 8 bytes every times, so the following
 * code may cause illegal read operation. This error will be found
 * by KASAN and cause an exception. To make sure that the code runs properly,
 * the below code will not be compiled.
 */
#ifndef LIBC_ENABLE_KASAN
#ifndef LIBC_ENABLE_KASAN_SYS
#ifdef __GNUC__
	for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
	if (n && *s != c) {
		typedef size_t __attribute__((__may_alias__)) word;
		const word *w;
		size_t k = ONES * c;
		for (w = (const void *)s; n>=SS && !HASZERO(*w^k); w++, n-=SS);
		s = (const void *)w;
	}
#endif
#endif
#endif
	for (; n && *s != c; s++, n--);
	return n ? (void *)s : 0;
}
