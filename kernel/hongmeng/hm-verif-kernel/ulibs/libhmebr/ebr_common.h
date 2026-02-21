/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declarations of ebr common part
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 29 10:55:00 2020
 */
#ifndef ULIBS_LIBHMEBR_EBR_COMMON_H
#define ULIBS_LIBHMEBR_EBR_COMMON_H

#include <lib/dlist.h>
#include <libhmebr/ebr.h>

struct epoch_unit {
	gdump_thread_t gdump;
	struct epoch_set *es;
} __aligned(EBR_ALIGNED_SIZE);

void *ebr_malloc(size_t size);
void ebr_free(void *p);

#endif
