/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Kernel lastklog headfile for users
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 4 15:32:32 2024
 */

#ifndef UAPI_HMKERNEL_LASTKLOG_H
#define UAPI_HMKERNEL_LASTKLOG_H

#include <hmkernel/types.h>

#define __LASTKLOG_MAGIC_A 0x44414548 /* HEAD */
#define __LASTKLOG_MAGIC_B 0x4b4c464f /* OFLK */
#define __LASTKLOG_MAGIC_C 0x0a47534d /* MSG\n */

struct __lastklog_header_s {
	__u32 magic_a;
	__u32 magic_b;
	__u32 magic_c;
};

static inline __bool __lastklog_header_is_valid(const void *vaddr)
{
	struct __lastklog_header_s *header = (struct __lastklog_header_s *)(__uptr_t)vaddr;
	return ((header != NULL) &&
			(header->magic_a == __LASTKLOG_MAGIC_A) &&
			(header->magic_b == __LASTKLOG_MAGIC_B) &&
			(header->magic_c == __LASTKLOG_MAGIC_C));
}

#endif
