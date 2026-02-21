#ifndef MALLOC_IMPL_H
#define MALLOC_IMPL_H

#include <sys/mman.h>
#include "dynlink.h"

struct chunk {
	size_t psize, csize;
	struct chunk *next, *prev;
};

struct bin {
	volatile int lock[2];
	struct chunk *head;
	struct chunk *tail;
};

#define SIZE_ALIGN (4*sizeof(size_t))
#define SIZE_MASK (-SIZE_ALIGN)
#define OVERHEAD (2*sizeof(size_t))
#define DONTCARE 16

#define CHUNK_SIZE(c) ((c)->csize & -2)
#define CHUNK_PSIZE(c) ((c)->psize & -2)
#define PREV_CHUNK(c) ((struct chunk *)((char *)(c) - CHUNK_PSIZE(c)))
#define NEXT_CHUNK(c) ((struct chunk *)((char *)(c) + CHUNK_SIZE(c)))
#define MEM_TO_CHUNK(p) (struct chunk *)((char *)(p) - OVERHEAD)
#define CHUNK_TO_MEM(c) (void *)((char *)(c) + OVERHEAD)
#define BIN_TO_CHUNK(i) (MEM_TO_CHUNK(&mal.bins[i].head))

#define C_INUSE  ((size_t)1)

#define IS_MMAPPED(c) !((c)->csize & (C_INUSE))

hidden void __bin_chunk(struct chunk *);

hidden extern int __malloc_replaced;

struct malloc_par {
	size_t mmap_threshold;
	size_t trim_threshold;
};

hidden extern struct malloc_par mal_par;

static inline size_t mmap_threshold(void)
{
	return __atomic_load_n((volatile size_t*)&mal_par.mmap_threshold, __ATOMIC_RELAXED);
}

static inline size_t trim_threshold(void)
{
	return __atomic_load_n((volatile size_t*)&mal_par.trim_threshold, __ATOMIC_RELAXED);
}

static inline void set_mmap_threshold(size_t val)
{
	__atomic_store_n((volatile size_t*)&mal_par.mmap_threshold, val, __ATOMIC_RELAXED);
}

static inline void set_trim_threshold(size_t val)
{
	__atomic_store_n((volatile size_t*)&mal_par.trim_threshold, val, __ATOMIC_RELAXED);
}

#define MMAP_THRESHOLD (mmap_threshold())
#define RECLAIM (trim_threshold())

hidden extern int __malloc_initialized;

hidden void malloc_init(void);

#endif
