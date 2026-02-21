#include <string.h>
#include <stdint.h>
#include <endian.h>

#ifdef __GNUC__
typedef __attribute__((__may_alias__)) size_t WT;
#define WS (sizeof(WT))
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define MERGE(wa, sh_0, wb, sh_1) (((wa) >> (sh_0)) | ((wb) << (sh_1)))
#else
#define MERGE(wa, sh_0, wb, sh_1) (((wa) << (sh_0)) | ((wb) >> (sh_1)))
#endif

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	if (d==s) return d;
	if ((uintptr_t)s-(uintptr_t)d-n <= -2*n) return memcpy(d, s, n);

	if (d<s) {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)d % WS) {
				if (!n--) return dest;
				*d++ = *s++;
			}
			for (; n>=WS; n-=WS, d+=WS, s+=WS) *(WT *)d = *(WT *)s;
#ifndef LIBC_ENABLE_KASAN
#ifndef LIBC_ENABLE_KASAN_SYS
		} else {
			while ((uintptr_t)d % WS) {
				if (!n--) return dest;
				*d++ = *s++;
			}
			size_t wa;
			size_t wb;
			unsigned int offset = (uintptr_t)s % WS;
			unsigned int shift_0 = offset << 3;
			unsigned int shift_1 = (WS << 3) - shift_0;
			if (n >= WS) {
				s = s - offset;
				wa = *(WT *)s;
				while (n >= (WS << 2)) {
					s += WS;
					wb = *(WT *)s;
					*(WT *)d = MERGE(wa, shift_0, wb, shift_1);
					d += WS;
					n -= WS;
					s += WS;
					wa = *(WT *)s;
					*(WT *)d = MERGE(wb, shift_0, wa, shift_1);
					d += WS;
					n -= WS;
					s += WS;
					wb = *(WT *)s;
					*(WT *)d = MERGE(wa, shift_0, wb, shift_1);
					d += WS;
					n -= WS;
					s += WS;
					wa = *(WT *)s;
					*(WT *)d = MERGE(wb, shift_0, wa, shift_1);
					d += WS;
					n -= WS;
				}
				if (n >= (WS << 1)) {
					s += WS;
					wb = *(WT *)s;
					*(WT *)d = MERGE(wa, shift_0, wb, shift_1);
					d += WS;
					n -= WS;
					s += WS;
					wa = *(WT *)s;
					*(WT *)d = MERGE(wb, shift_0, wa, shift_1);
					d += WS;
					n -= WS;
				}
				if (n >= WS) {
					s += WS;
					wb = *(WT *)s;
					*(WT *)d = MERGE(wa, shift_0, wb, shift_1);
					d += WS;
					n -= WS;
				}
				s = s + offset;
				for (; n != 0; n--) {
					*d++ = *s++;
				}
				return dest;
			}
#endif /* LIBC_ENABLE_KASAN_SYS */
#endif /* LIBC_ENABLE_KASAN */
		}
#endif
		for (; n; n--) *d++ = *s++;
	} else {
#ifdef __GNUC__
		if ((uintptr_t)s % WS == (uintptr_t)d % WS) {
			while ((uintptr_t)(d+n) % WS) {
				if (!n--) return dest;
				d[n] = s[n];
			}
			while (n>=WS) n-=WS, *(WT *)(d+n) = *(WT *)(s+n);
#ifndef LIBC_ENABLE_KASAN
#ifndef LIBC_ENABLE_KASAN_SYS
		} else {
			while ((uintptr_t)(d+n) % WS) {
				if (!n--) return dest;
				d[n] = s[n];
			}
			size_t wa;
			size_t wb;
			unsigned int offset = (uintptr_t)(s + n) % WS;
			unsigned int shift_0 = offset << 3;
			unsigned int shift_1 = (WS << 3) - shift_0;
			if (n >= WS) {
				s = s - offset;
				wa = *(WT *)(s + n);
				while (n >= (WS << 2)) {
					n -= WS;
					wb = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wb, shift_0, wa, shift_1);
					n -= WS;
					wa = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wa, shift_0, wb, shift_1);
					n -= WS;
					wb = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wb, shift_0, wa, shift_1);
					n -= WS;
					wa = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wa, shift_0, wb, shift_1);
				}
				if (n >= (WS << 1)) {
					n -= WS;
					wb = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wb, shift_0, wa, shift_1);
					n -= WS;
					wa = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wa, shift_0, wb, shift_1);
				}
				if (n >= WS) {
					n -= WS;
					wb = *(WT *)(s + n);
					*(WT *)(d + n) = MERGE(wb, shift_0, wa, shift_1);
				}
				s = s + offset;
				while (n != 0) {
					n--;
					d[n] = s[n];
				}
				return dest;
			}
#endif /* LIBC_ENABLE_KASAN_SYS */
#endif /* LIBC_ENABLE_KASAN */
		}
#endif
		while (n) n--, d[n] = s[n];
	}

	return dest;
}
