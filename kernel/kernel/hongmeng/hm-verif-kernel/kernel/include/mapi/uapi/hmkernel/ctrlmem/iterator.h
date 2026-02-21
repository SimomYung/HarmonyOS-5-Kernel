/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: tlb guarder ctrl - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 17 12:03:31 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_ITERATOR_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_ITERATOR_H

#include <hmkernel/types.h>

#define __CTRLMEM_ITERATOR_BATCH_MAP_PGSTRVA 0
#define __CTRLMEM_ITERATOR_BATCH_TLBG_UNMAP 1

#define __CTRLMEM_ITERATOR_SIZE	(4 * 4096ULL)

struct __ctrlmem_iterator_batch_map_head_s {
	__u32 num;
	__u32 modelid;
	__uptr_t va_start;
	__uptr_t va_stop;
};

struct __ctrlmem_iterator_batch_map_piece_s {
	void *ppte;
	__uptr_t old_val;
	__u64 pa;
};

struct __ctrlmem_iterator_batch_unmap_head_s {
	__u32 num;
	__u32 stop_num;
	void *tlbg;
	void *kobj_vspace;
};

struct __ctrlmem_iterator_batch_unmap_piece_s {
	void *ppte;
	__uptr_t va;
	__u64 pa;
};

struct __ctrlmem_iterator_type_s {
	__u32 type;
};

struct __ctrlmem_iterator_head_s {
	struct __ctrlmem_iterator_type_s t;
	union {
		struct __ctrlmem_iterator_batch_map_head_s map_head;
		struct __ctrlmem_iterator_batch_unmap_head_s unmap_head;
	} u;
};

struct __ctrlmem_iterator_piece_s {
	union {
		struct __ctrlmem_iterator_batch_map_piece_s map_piece;
		struct __ctrlmem_iterator_batch_unmap_piece_s unmap_piece;
	} u;
};

#define __CTRLMEM_NR_ITERATOR_PIECE	(((__CTRLMEM_ITERATOR_SIZE -			\
					 sizeof(struct __ctrlmem_iterator_head_s)) /	\
					 sizeof(struct __ctrlmem_iterator_piece_s)))

struct __ctrlmem_iterator_page_s {
	struct __ctrlmem_iterator_head_s head;
	struct __ctrlmem_iterator_piece_s pieces[__CTRLMEM_NR_ITERATOR_PIECE];
};

#endif
