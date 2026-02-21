/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Data structures and interfaces for allocators
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_NODE_H
#define KLIBS_COOBJ_NODE_H

#include <lib/coobj/node_array.h>
#include <lib/coobj/node_radix.h>
#include <lib/coobj/node_radix1var.h>

/**************************************************
 * urenter utility
 **************************************************/
static inline
__bool ____coobj_urenter_acquire_ptr(
	struct ____coobj_urenter_node_s *node)
{
	__bool acquired = __false;
	__uptr_t local_status = __atomic_add_fetch(&node->status, 2, __ATOMIC_RELEASE);
	if ((local_status & 0x1) != 1) {
		asm volatile("":::"memory");
		if (node->page_uva != NULL) {
			acquired = __true;
		}
		asm volatile("":::"memory");
	}
	return acquired;
}

static inline
void ____coobj_urenter_release_ptr(
	struct ____coobj_urenter_node_s *node,
	struct ____coobj_base_user_s *base_user,
	struct ____coobj_chunkid_s chunkid)
{
	__uptr_t local_status = __atomic_sub_fetch(&node->status, 2, __ATOMIC_RELEASE);
	if (local_status == 1) {
		____coobj_base_user_urenter_notify_noref(base_user, chunkid);
	}
}

/**************************************************
 * common data structure
 **************************************************/
#define ____COOBJ_PAGE_ORDER_4K 12
#define ____COOBJ_PAGE_ORDER_16K 14
#define ____COOBJ_PAGE_ORDER_64K 16

struct ____coobj_base_param_s {
	/* page_order: The order of page */
	__uptr_t page_order;
};
static inline
struct ____coobj_base_param_s coobj_base_param(__uptr_t page_order)
{
	return (struct ____coobj_base_param_s) {
		.page_order = page_order,
	};
}

#endif
