#include <string.h>
#include <stdint.h>

#if __SIZE_MAX__ == 4294967295UL
#define hasnull4(_w)		( (((_w) - 0x01010101UL) & ( ~(_w) & 0x80808080UL)) != 0UL)
#define hasnull(_w) hasnull4((_w))
#elif __SIZE_MAX__ == 18446744073709551615ULL
#define hasnull8(_w)		( (((_w) - 0x0101010101010101ULL) & ( ~(_w) & 0x8080808080808080ULL)) != 0ULL)
#define hasnull(_w) hasnull8((_w))
#else
#error __SIZE_MAX__ not defined!
#endif

#define strncmp_loop_once(chk1, chk2, str_ptr1, str_ptr2, eexitvar) \
	(chk1) = (unsigned char) *(str_ptr1)++; (chk2) = (unsigned char) *(str_ptr2)++; \
	if ((chk1) == '\0' || (chk1) != (chk2)) { (eexitvar) = 1; break; }

#define strncmp_loop_unroll4(chk1, chk2, str_ptr1, str_ptr2, eexitvar) \
	strncmp_loop_once(chk1, chk2, str_ptr1, str_ptr2, eexitvar) \
	strncmp_loop_once(chk1, chk2, str_ptr1, str_ptr2, eexitvar) \
	strncmp_loop_once(chk1, chk2, str_ptr1, str_ptr2, eexitvar) \
	strncmp_loop_once(chk1, chk2, str_ptr1, str_ptr2, eexitvar) \


#define BYTES_IN_64BIT_ARCH		sizeof(uint64_t)
#define BYTES_IN_32BIT_ARCH		sizeof(uint32_t)

int strncmp(const char *src1, const char *src2, size_t len)
{
	/* avoid code scan warning G.FUD.09 Avoid modifying the value of function parameters */
	const char* s1 = src1;
	const char* s2 = src2;
	size_t n = len;

	unsigned char ck1 = '\0';
	unsigned char ck2 = '\0';

	if (n < sizeof(n)) goto byte_cmp;

	size_t blk_n = n >> (sizeof(n) == 8 ? 3 : 2); /* calc n blocks */
	n &= sizeof(n)-1; /* mask off the blk amount */
	do {
		int exit_early = 0;
		/* check alignment */
		if ( (((uintptr_t)s1|(uintptr_t)s2) & (sizeof(n)-1)) == 0 ) { /* we're dbl aligned */
			if(!hasnull(*(size_t*)s1) && !hasnull(*(size_t*)s2) && *(size_t*)s1 == *(size_t*)s2) { /* no nulls */
				s1+=sizeof(size_t);
				s2+=sizeof(size_t);
				continue; /* let's do the loop again */
			}
		}
		/* unroll our loop */
		int tmp = sizeof(n);
		switch(tmp) {
		case BYTES_IN_64BIT_ARCH:
			strncmp_loop_unroll4(ck1, ck2, s1, s2, exit_early);
		/* fall-thru */
		case BYTES_IN_32BIT_ARCH:
			strncmp_loop_unroll4(ck1, ck2, s1, s2, exit_early);
			break;
		/* fall-thru */
		default: /* should not be reachable */
			blk_n = 1; /* exit the do_while() and fall back to byte compare */
			break;
		}
		/* We do this to avoid doing direct return inside of the switch block */
		if(exit_early != 0) {
			blk_n = 1; /* exit the do..while() */
			n=0; /* skip the while(n) */
		}
	} while ((--blk_n) != 0);
byte_cmp:
	/* finish off any bits or a very small strncmp */
	while(n != 0) {
		ck1 = (unsigned char) *s1++;
		ck2 = (unsigned char) *s2++;
		if (ck1 == '\0' || ck1 != ck2) {
			break; // return (ck1-ck2);
		}
		n--;
	}
	return(ck1-ck2);
}
