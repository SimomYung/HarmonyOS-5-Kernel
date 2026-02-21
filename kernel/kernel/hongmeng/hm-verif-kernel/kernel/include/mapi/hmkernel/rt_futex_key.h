/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Rt futex key function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 14 16:23:30 2020
 */
#ifndef MAPI_HMKERNEL_RT_FUTEX_KEY_H
#define MAPI_HMKERNEL_RT_FUTEX_KEY_H

#include <lib/string.h>
#include <hmkernel/memory.h>

struct capability_s;
struct rt_futex_key_s {
	bool is_private;
	union {
		paddr_t pa;
		struct {
			struct capability_s *vspace;
			vaddr_t va;
		} priv;
	} key;
};

static inline unsigned long
rt_futex_key_hash(const struct rt_futex_key_s *hashkey,
		  unsigned long hash_table_sz)
{
	unsigned long hash;
	BUG_ON(hash_table_sz == 0UL);
	if (hashkey->is_private) {
		hash = hashkey->key.priv.va % hash_table_sz;
	} else {
		hash = hashkey->key.pa % hash_table_sz;
	}

	return hash;
}

static inline bool
__rt_futex_key_compare_private(const struct rt_futex_key_s *key1,
			       const struct rt_futex_key_s *key2)
{
	return (key1->is_private && key2->is_private) &&
	       (key1->key.priv.va == key2->key.priv.va) &&
	       ((uptr_t)key1->key.priv.vspace == (uptr_t)key2->key.priv.vspace);
}

static inline bool
__rt_futex_key_compare_nonprivate(const struct rt_futex_key_s *key1,
				  const struct rt_futex_key_s *key2)
{
	return (!key1->is_private && !key2->is_private) &&
	       (key1->key.pa == key2->key.pa);
}

static inline bool
rt_futex_key_is_equal(const struct rt_futex_key_s *key1,
		      const struct rt_futex_key_s *key2)
{
	return (__rt_futex_key_compare_private(key1, key2) ||
		__rt_futex_key_compare_nonprivate(key1, key2));
}

static inline void
rt_futex_key_init(struct rt_futex_key_s *hashkey,
		  void *addr, bool is_private, struct capability_s *vs)
{
	if (is_private) {
		hashkey->is_private = true;
		hashkey->key.priv.vspace = vs;
		hashkey->key.priv.va = ptr_to_vaddr(addr);
	} else {
		hashkey->is_private = false;
		hashkey->key.pa = ptr_to_vaddr(addr);
	}
}
#endif
