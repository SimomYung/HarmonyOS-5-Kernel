#include <string.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n)
{
	unsigned char *d = dest;
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
	typedef size_t __attribute__((__may_alias__)) word;
	word *wd;
	const word *ws;
	if (((uintptr_t)s & ALIGN) == ((uintptr_t)d & ALIGN)) {
		for (; ((uintptr_t)s & ALIGN) && n && (*d=*s)!=c; n--, s++, d++);
		if ((uintptr_t)s & ALIGN) goto tail;
		size_t k = ONES * c;
		wd=(void *)d; ws=(const void *)s;
		for (; n>=sizeof(size_t) && !HASZERO(*ws^k);
		       n-=sizeof(size_t), ws++, wd++) *wd = *ws;
		d=(void *)wd; s=(const void *)ws;
	}
#endif
#endif
#endif
	for (; n && (*d=*s)!=c; n--, s++, d++);
#ifndef LIBC_ENABLE_KASAN
#ifndef LIBC_ENABLE_KASAN_SYS
tail:
#endif
#endif
	if (n) return d+1;
	return 0;
}
