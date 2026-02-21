/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Malloc
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 15 15:57:04 2019
 */
#include <stdlib.h>

#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

void *malloc(size_t size)
{
	hm_panic("unexpected use of malloc: size=%zx\n", size);
	return NULL;
}

void free(void *ptr)
{
	UNUSED(ptr);
	hm_panic("unexpected use of free\n");
}
