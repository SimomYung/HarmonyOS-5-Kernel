/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Priomap
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 30 00:54:41 2018
 */
#ifndef KLIBS_PRIOMAP_H
#define KLIBS_PRIOMAP_H

#include <hmkernel/rq.h>
#include <hmkernel/types.h>

struct priomap {
	__u64 ready_tbl;
};

void priomap_init(struct priomap *bitmap);
void priomap_set(struct priomap *bitmap, unsigned int prio);
void priomap_clear(struct priomap *bitmap, unsigned int prio);
int priomap_get_highest(struct priomap *bitmap);
int priomap_get_next_highest(struct priomap *bitmap, unsigned int prio);
#endif
