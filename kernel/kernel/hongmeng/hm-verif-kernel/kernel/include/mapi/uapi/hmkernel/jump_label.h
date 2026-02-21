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

static inline __s32 static_key_set_enabled(__uptr_t enabled_p,
					   unsigned int bitmask,
					   bool enable)
{
	unsigned int new_val;
	unsigned int expt_val;
	unsigned int old = vatomic32_read_rlx((vatomic32_t *)enabled_p);
	do {
		new_val = expt_val = old;
		if (enable) {
			new_val |= bitmask;
		} else {
			new_val &= ~bitmask;
		}
		old = vatomic32_cmpxchg((vatomic32_t *)enabled_p, expt_val, new_val);
	} while (old != expt_val);

	return (__s32)new_val;
}

#ifdef CONFIG_JUMP_LABEL
extern struct jump_entry __start___jump_table[];
extern struct jump_entry __stop___jump_table[];

#define for_each_jump_table(iter, start, end)	\
	for ((iter) = (struct jump_entry *)(start);(iter) < (struct jump_entry *)(end);(iter)++)

static inline __bool static_branch_unlikely(struct static_key * const key)
{
	return arch_static_branch(key);
}

int static_key_enable(struct static_key *key, unsigned int bitmask);
int static_key_disable(struct static_key *key, unsigned int bitmask);
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

static inline int static_key_enable(struct static_key *key, unsigned int bitmask)
{
	(void)static_key_set_enabled((__uptr_t)(void *)&key->enabled,
				     bitmask, true);
	return E_HM_OK;
}

static inline int static_key_disable(struct static_key *key, unsigned int bitmask)
{
	(void)static_key_set_enabled((__uptr_t)(void *)&key->enabled,
				     bitmask, false);
	return E_HM_OK;
}
#endif /* CONFIG_JUMP_LABEL */
#endif /* UAPI_HMKERNEL_JUMP_LABEL_H */
