/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei RTOS
 * Create: Fri Sept 3 11:09:00 2021
 */
#include "obstack.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <libintl.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define DEFAULT_CHUNK_SIZE 4096

static void default_obstack_handler(void)
{
    err(EXIT_FAILURE, "%s\n", gettext("Obstack memory exhausted"));
}

static size_t min_chunk_size(size_t alignment_mask)
{
    struct _obstack_chunk chunk;
    uintptr_t object_base = (uintptr_t)__ptr_align(&chunk, chunk.data, alignment_mask);
    return (size_t)(object_base - (uintptr_t)(&chunk));
}

int _obstack_begin(struct obstack *h,
                    size_t _size, size_t _alignment,
                    void *(*_alloc_fun)(size_t), void (*_free_fun)(void *))
{
    if (h == NULL || _alloc_fun == NULL || _free_fun == NULL) {
        return 0;
    }

    h->func_alloc = _alloc_fun;
    h->func_free = _free_fun;

    size_t size = _size == 0 ? DEFAULT_CHUNK_SIZE : _size;

    /* Calculate the default alignment to make sure that the obstack can hold any type of data by default */
    size_t alignment = _alignment != 0 ? _alignment :
        max(__alignof__(long double), max(__alignof__(uintmax_t), __alignof__(void *)));

    if (size < min_chunk_size(alignment - 1)) {
        return 0;
    }

    h->chunk_size = size;
    h->alignment_mask = alignment - 1;

    struct _obstack_chunk  *chunk = h->func_alloc(size);
    if (chunk == NULL) {
        default_obstack_handler();
        return 0;
    }

    chunk->chunk_prev = NULL;
    chunk->chunk_limit = (char *)chunk + h->chunk_size;

    h->object_base = h->next_free = (char *)__ptr_align(chunk, chunk->data, h->alignment_mask);
    h->top_chunk = chunk;
    h->maybe_has_empty_object = 0;
    h->chunk_limit = chunk->chunk_limit;

    return 1;
}

static int _obstack_create_chunk(struct obstack *h, size_t length)
{
    if (!h) {
        return 0;
    }
    size_t object_size = h->next_free - h->object_base;
    size_t base_size = object_size + length;
    size_t size_with_align = base_size + h->alignment_mask;
    size_t size_bigger = size_with_align + (base_size >> 3) + 100;
    size_t new_size = max(size_with_align, size_bigger);
    new_size = max(new_size, h->chunk_size);

    struct _obstack_chunk *new_chunk = NULL;
    struct _obstack_chunk *old_chunk = h->top_chunk;

    if (object_size <= base_size && base_size <= size_with_align) {
        new_chunk = h->func_alloc(new_size);
    }

    if (new_chunk == NULL) {
        default_obstack_handler();
        return 0;
    }

    char *dist = (char *)__ptr_align(new_chunk, new_chunk->data, h->alignment_mask);
    (void)memcpy(dist, h->object_base, object_size);

    if (h->maybe_has_empty_object == 0 && h->object_base ==
        (char *)__ptr_align(old_chunk, old_chunk->data, h->alignment_mask)) {
        struct _obstack_chunk *tmp = old_chunk->chunk_prev;
        h->func_free(old_chunk);
        old_chunk = tmp;
    }

    new_chunk->chunk_prev = old_chunk;
    h->chunk_limit = new_chunk->chunk_limit = (char*)new_chunk + new_size;
    h->top_chunk = new_chunk;
    h->maybe_has_empty_object = 0;
    h->object_base = (char *)dist;
    h->next_free = (char *)dist + object_size;

    return 1;
}

static void _obstack_free(struct obstack *h, void *object)
{
    if (!h) {
        return;
    }
    struct _obstack_chunk *chunk = h->top_chunk;
    struct _obstack_chunk *prev;

    while (chunk != NULL) {
        if (object >= (void *)chunk && object < (void *)h->chunk_limit) {
            break;
        }
        prev = chunk->chunk_prev;
        h->func_free(chunk);
        chunk = prev;
    }

    if (chunk != NULL) {
        h->top_chunk = chunk;
        h->object_base = h->next_free = object;
        h->chunk_limit = chunk->chunk_limit;
        h->maybe_has_empty_object = 1;
    } else if (object) {
        err(EXIT_FAILURE, "%s\n", gettext("Obstack: free non-self memory"));
    } else {
        (void)memset(h, 0, sizeof(struct obstack));
    }
}

size_t obstack_memory_used(struct obstack *h)
{
    if (h == NULL) {
        return 0;
    }

    size_t res = 0;
    struct _obstack_chunk *chunk;

    for (chunk = h->top_chunk; chunk != NULL; chunk = chunk->chunk_prev) {
        res += chunk->chunk_limit - (char *)chunk;
    }
    return res;
}

size_t obstack_chunk_count(struct obstack* h)
{
    if (h == NULL) {
        return 0;
    }

    size_t count = 0;
    struct _obstack_chunk *_chunk;

    for (_chunk = h->top_chunk; _chunk != NULL; _chunk = _chunk->chunk_prev) {
        count++;
    }
    return count;
}

static void create_newchunk_ifneed(struct obstack *h, size_t size)
{
    if (obstack_room(h) < size) {
        (void)_obstack_create_chunk(h, size);
    }
}

void obstack_blank(struct obstack *h, size_t size)
{
    if (h == NULL) {
        return;
    }

    create_newchunk_ifneed(h, size);
    h->next_free += size;
}

void obstack_grow(struct obstack *h, const void  *src, size_t size)
{
    if (h == NULL || src == NULL) {
        return;
    }

    create_newchunk_ifneed(h, size);
    (void)memcpy((void *)h->next_free, src, size);
    h->next_free += size;
}

void obstack_grow0(struct obstack *h, const void *src, size_t size)
{
    if (h == NULL || src == NULL) {
        return;
    }

    create_newchunk_ifneed(h, size);
    (void)memcpy((void *)h->next_free, src, size);
    *(h->next_free + size) = 0;
    h->next_free += size + 1;
}

void obstack_1grow(struct obstack *h, char c)
{
    if (h == NULL) {
        return;
    }

    create_newchunk_ifneed(h, 1);
    *(h->next_free)++ = c;
}

void obstack_ptr_grow(struct obstack *h, void *p)
{
    if (h == NULL) {
        return;
    }

    create_newchunk_ifneed(h, sizeof(void *));
    *((void **)h->next_free) = p;
    h->next_free += sizeof(void *);
}

void obstack_int_grow(struct obstack *h, int n)
{
    if (h == NULL) {
        return;
    }

    create_newchunk_ifneed(h, sizeof(int));
    *((int *)h->next_free) = n;
    h->next_free += sizeof(int);
}

void *obstack_finish(struct obstack *h)
{
    if (h == NULL) {
        return NULL;
    }

    if (h->object_base == h->next_free) {
        (h->maybe_has_empty_object = 1);
    }
    h->next_free = (char *) __ptr_align(h->object_base, h->next_free, h->alignment_mask);
    if (h->next_free > h->chunk_limit) {
        (h->next_free = h->chunk_limit);
    }
    void *res = h->object_base;
    h->object_base = h->next_free;
    return res;
}

void obstack_free(struct obstack *h, void *obj)
{
    if (h == NULL) {
        return;
    }

    obj >= (void *)h->top_chunk && obj < (void *)h->chunk_limit ?
        (h->object_base = h->next_free = (char *)obj) : _obstack_free(h, obj);
}
