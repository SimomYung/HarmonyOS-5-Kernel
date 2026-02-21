/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation for ebr malloc/free function
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 15:02:45 2023
 */

#include "ebr_common.h"
#include <malloc.h>

void ebr_unit_cache_init(void) {  }

void *ebr_malloc(size_t size)
{
	return malloc(size);
}

void ebr_free(void *p)
{
	free(p);
}

void *ebr_unit_malloc(size_t size)
{
	return malloc(size);
}

void ebr_unit_free(void *p)
{
	free(p);
}
