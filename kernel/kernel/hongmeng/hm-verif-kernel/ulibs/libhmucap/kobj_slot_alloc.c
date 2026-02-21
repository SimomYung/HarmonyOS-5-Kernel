/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Kobj slot allocation interfaces.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 12 15:14:31 2024
 */

#include <libhmucap/kobjpool.h>

void *kobj_slot_malloc(void)
{
	size_t size = kobj_slot_size();
	void *slot = malloc(size);

	return slot;
}

void kobj_slot_free(void *ptr)
{
	if (ptr != NULL) {
		free(ptr);
	}
}
