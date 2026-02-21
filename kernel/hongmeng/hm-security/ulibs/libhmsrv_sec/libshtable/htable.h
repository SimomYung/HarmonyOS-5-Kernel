/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure hash table head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:50:02 2019
 */
#ifndef ULIBS_LIBSHTABLE_HTABLE_H_
#define ULIBS_LIBSHTABLE_HTABLE_H_

#include "libshtable/types.h"
#include "libshtable/shtable.h"

struct sht_bucket_s {
	sht_idx_t node;
};

struct sht_node_s {
	struct sht_list_node	node;
	seq_t		node_seq;
	seq_t		ctx_seq;
};

void sht_htable_init(const struct shtable *sht);
void sht_htable_fini(const struct shtable *sht);
#endif
