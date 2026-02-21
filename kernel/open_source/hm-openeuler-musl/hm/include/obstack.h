/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei RTOS
 * Create: Fri Sept 3 11:09:00 2021
 */
#ifndef _OBSTACK_H
#define _OBSTACK_H

#include <stddef.h>
#include <string.h>
#include <stdint.h>

/* M is the alignment mask. Considering the result of P + M maybe overflows,
firstly align the gap of P - B.(B is the base address ) */
#define __bptr_align(B, P, M) ((uintptr_t)(B) + ((((uintptr_t)(P) - (uintptr_t)(B)) + (M)) & ~(M)))
#define __ptr_align(B, P, M)                                                  \
    __bptr_align (sizeof (ptrdiff_t) < sizeof (void *) ? (B) : 0, P, M)

#ifdef __cplusplus
extern "C" {
#endif

struct _obstack_chunk {
    char *chunk_limit;                      /* the end address of this chunk */
    struct _obstack_chunk* chunk_prev;
    char data[0];                           /* the space that stores data */
};

struct obstack {
    struct _obstack_chunk *top_chunk;           /* the base adress of the chunk in the top of stack */
    size_t chunk_size;                 /* default size of new chunk */
    char *object_base;                  /* base address of the growing object */
    char *next_free;                    /* the next byte address which can be allocated to growing object .
                                        if there is not a growing object , object_base = next_free */
    char *chunk_limit;                  /* the chunk_limit of top_chunk */
    size_t alignment_mask;

    void *(*func_alloc)(size_t);    /* allocate function that the obstack uses; */
    void (*func_free)(void*);       /* free function that the obstack uses; */

    unsigned maybe_has_empty_object : 1;    /* maybe has an empty object in this chunk */
};

#define obstack_chunk_size(h) ((h)->chunk_size)
#define obstack_base(h) ((h)->object_base)
#define obstack_next_free(h) ((h)->next_free)
#define obstack_room(h) ((size_t)((h)->chunk_limit - (h)->next_free))
#define obstack_object_size(h) ((size_t)((h)->next_free - (h)->object_base))
#define obstack_alignment_mask(h) ((h)->alignment_mask)

extern int _obstack_begin(struct obstack*, size_t, size_t, void *(*)(size_t), void (*)(void *));
extern size_t obstack_chunk_count(struct obstack *);
extern size_t obstack_memory_used(struct obstack *);

#define obstack_init(h) (_obstack_begin((h), 0, 0, obstack_chunk_alloc, obstack_chunk_free))

extern void obstack_blank(struct obstack *, size_t);
extern void obstack_grow(struct obstack *, const void *, size_t);
extern void obstack_grow0(struct obstack *, const void *, size_t);
extern void obstack_1grow(struct obstack *, char);
extern void obstack_ptr_grow(struct obstack *, void *);
extern void obstack_int_grow(struct obstack *, int);
extern void *obstack_finish(struct obstack *);
extern void obstack_free(struct obstack *, void *);

#define obstack_alloc(h, size) (obstack_blank((h), (size)), obstack_finish((h)))
#define obstack_copy(h, src, size) (obstack_grow((h), (char *)(src), (size)), obstack_finish((h)))
#define obstack_copy0(h, src, size) (obstack_grow0((h), (char *)(src), (size)), obstack_finish((h)))

#ifdef __cplusplus
}
#endif

#endif /* _OBSTACK_H */
