/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSKIPLIST_TYPES_H
#define VSKIPLIST_TYPES_H

#include <vsync/vtypes.h>
#include <vsync/atomic.h>
#include <vsync/atomic/atomicptr_markable.h>
#include <vsync/common/assert.h>

#ifndef SKIPLIST_MAX_HEIGHT
#if defined(VSYNC_VERIFICATION)
#define SKIPLIST_MAX_HEIGHT 4U
#else
#define SKIPLIST_MAX_HEIGHT 32U
#endif
#endif

V_STATIC_ASSERT(SKIPLIST_MAX_HEIGHT <= VUINT8_MAX,
		"SKIPLIST_MAX_HEIGHT must be <= VUINT8_MAX");

typedef vuintptr_t vskiplist_key_t;

typedef struct vskiplist_node_s {
	vatomicptr_markable(vskiplist_node_t *)
		next[SKIPLIST_MAX_HEIGHT]; // TODO: support dynamic mem alloc
	vatomicsz_t non_snipped_level;
	vsize_t height;
} vskiplist_node_t;

typedef struct skiplist_seed_s {
	vsize_t max_height;
	vatomicsz_t seed;
} vskiplist_seed_t;

typedef int (*vskiplist_cmp_node_t)(vskiplist_node_t *, vskiplist_key_t);
typedef void (*vskiplist_handle_node_t)(vskiplist_node_t *, void *);

typedef struct vskiplist_s {
	vskiplist_node_t head;
	vskiplist_node_t tail;
	vskiplist_seed_t seed;
	vskiplist_cmp_node_t fun_cmp;
	vskiplist_handle_node_t fun_retire;
	void *fun_retire_arg;
} vskiplist_t;

#endif
