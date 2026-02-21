/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Hash function for network
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 25 17:21:00 2023
 */

#ifndef __UX_HASH_H_
#define __UX_HASH_H_

#include <libalgo/rb_htable.h>

enum hash_type {
	H_UNIX_PATH,
	H_MAX
};

typedef long (*cmp_key_fn)(void *cur_data, void *key);
typedef unsigned int (*calc_hval_fn)(void *key);

void ux_htable_register(enum hash_type type, cmp_key_fn cmp_key, calc_hval_fn calc_hval);
int ux_htable_insert(struct easy_htable *htable, enum hash_type type, void *key, struct rbt_hnode *node);
int ux_htable_remove(struct easy_htable *htable, enum hash_type type, void *key, struct rbt_hnode *node);
void *ux_htable_lookup(struct easy_htable *htable, enum hash_type type, void *key);

#endif
