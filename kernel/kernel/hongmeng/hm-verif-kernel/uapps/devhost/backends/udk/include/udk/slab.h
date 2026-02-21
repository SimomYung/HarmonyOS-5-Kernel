/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: This is the header of udk_slab interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 03 11:29:28 2020
 */
#ifndef __DEVHOST_UDK_SLAB_H__
#define __DEVHOST_UDK_SLAB_H__

#include <udk/types.h>

struct udk_slab {
	void *buf;
	size_t size;
	size_t unit_size;
	void *info;
};

/* size must be aligned to PAGE_SIZE */
int udk_slab_init(struct udk_slab *slab, size_t size, size_t unit_size);
void udk_slab_destroy(struct udk_slab *slab);
int udk_slab_unit_alloc(struct udk_slab *slab, void **va);
int udk_slab_unit_free(struct udk_slab *slab, const void *va);
unsigned int udk_slab_get_unit_free_num(struct udk_slab *slab);

#endif /* __DEVHOST_UDK_SLAB_H__ */
