#ifndef _MALLOC_H
#define _MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t

#include <bits/alltypes.h>

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *valloc (size_t);
void *memalign(size_t, size_t);

size_t malloc_usable_size(void *);

int malloc_trim(size_t);

/* SVID2/XPG mallopt options
 * Both are unsupported */
#define M_MXFAST		1
#define M_NLBLKS		2
#define M_GRAIN			3
#define M_KEEP			4

/* mallopt options */
#define M_TRIM_THRESHOLD	(-1)
#define M_TOP_PAD		(-2) /* UNSUPPORTED */
#define M_MMAP_THRESHOLD	(-3)
#define M_MMAP_MAX		(-4) /* UNSUPPORTED */
#define M_CHECK_ACTION		(-5) /* UNSUPPORTED */
#define M_PERTURB		(-6) /* UNSUPPORTED */
#define M_ARENA_TEST		(-7) /* UNSUPPORTED */
#define M_ARENA_MAX		(-8) /* UNSUPPORTED */

int mallopt(int, int);

void *reallocarray(void *p, size_t nmemb, size_t elem_size);

struct mallinfo {
	size_t arena;    /* total amount of memory allocated */
	size_t ordblks;  /* number of oridinary free blocks */
	size_t smblks;   /* number of fastpath free blocks */
	size_t hblks;    /* number of blocks allocated using mmap */
	size_t hblkhd;   /* number of bytes in blocks allocated using mmap */
	size_t usmblks;  /* this field is unused */
	size_t fsmblks;  /* number of bytes in fastpath free blocks */
	size_t uordblks; /* number of bytes used in blocks */
	size_t fordblks; /* number of bytes freed in blocks */
	size_t keepcost; /* total amount releasable blocks */
};
struct mallinfo mallinfo(void);

#ifdef __cplusplus
}
#endif

#endif
