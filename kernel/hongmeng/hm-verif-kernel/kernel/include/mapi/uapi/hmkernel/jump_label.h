/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of jump label
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 08 16:33:22 2023
 */
#ifndef UAPI_HMKERNEL_JUMP_LABEL_H
#define UAPI_HMKERNEL_JUMP_LABEL_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmasm/jump_label.h>
#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif

struct static_key {
#ifdef CONFIG_JUMP_LABEL
	__s32 enabled;
#else
	vatomic32_t enabled;
#endif
};

struct jump_entry {
	unsigned long code;
	unsigned long target;
	unsigned long key;
};

#define JUMP_TABLE_SECTION	"__jump_table"

#ifdef CONFIG_JUMP_LABEL
extern struct jump_entry __start___jump_table[];
extern struct jump_entry __stop___jump_table[];

#define for_each_jump_table(iter, start, end)	\
	for ((iter) = (struct jump_entry *)(start);(iter) < (struct jump_entry *)(end);(iter)++)

static inline __bool static_branch_unlikely(struct static_key * const key)
{
	return arch_static_branch(key);
}
#else
static inline int static_key_count(struct static_key *key)
{
	return vatomic32_read(&key->enabled);
}

static inline __bool static_branch_unlikely(struct static_key * const key)
{
	__bool ret = false;

	if (unlikely(static_key_count(key) > 0)) {
		ret = true;
	}
	return ret;
}
#endif /* CONFIG_JUMP_LABEL */
#endif /* UAPI_HMKERNEL_JUMP_LABEL_H */
