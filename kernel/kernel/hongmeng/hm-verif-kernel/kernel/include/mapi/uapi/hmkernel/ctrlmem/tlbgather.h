/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: tlb guarder ctrl - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 17 12:03:31 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_TLBGATHER_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_TLBGATHER_H

#include <hmasm/memory.h>
#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/kobjs/vspace/rangeop.h>

#define __CTRLMEM_SYSCALL_TLBGATHER_OP_UNMAP_PARTIAL 0x0
#define __CTRLMEM_SYSCALL_TLBGATHER_OP_FLUSH 0x1

#define __CTRLMEM_TLBGATHER_SIZE	(4096)

struct __ctrlmem_tlbgather_head_s {
	__u32 action;
	unsigned int free_pos;
};

struct __ctrlmem_tlbgather_piece_s {
	unsigned int asid;
	__uptr_t va;
	__u64 parec;
	unsigned long size;
};

#define __CTRLMEM_NR_TLBGATHER_PIECE	(((__CTRLMEM_TLBGATHER_SIZE -			\
					 sizeof(struct __ctrlmem_tlbgather_head_s)) /	\
					 sizeof(struct __ctrlmem_tlbgather_piece_s)))

struct __ctrlmem_tlbgather_page_s {
	struct __ctrlmem_tlbgather_head_s head;
	struct __ctrlmem_tlbgather_piece_s pieces[__CTRLMEM_NR_TLBGATHER_PIECE];
};

#define __TLBGATHER_PAGE_ACTION_OFFSET (offset_of(struct __ctrlmem_tlbgather_page_s, head.action))
#endif
