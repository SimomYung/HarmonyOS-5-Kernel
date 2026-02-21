/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: This is the implementation of udk_slab related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 03 11:07:18 2020
 */
#include <udk/slab.h>

#include <udk/iolib.h>
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmshmalloc/shm_allocator.h>

int udk_slab_init(struct udk_slab *slab, size_t size, size_t unit_size)
{
	size_t real_size = size + SHARE_MEM_HEAD_LEN;

	if (slab == NULL ||
	    size == 0 ||
	    unit_size == 0 ||
	    size != PAGE_ALIGN_UP(size)) {
		return -EINVAL;
	}

	slab->buf = udk_malloc(real_size);
	if (slab->buf == NULL) {
		return -ENOMEM;
	}

	slab->info = (void *)shmalloc_init(slab->buf, real_size, unit_size);
	if (slab->info == NULL) {
		free(slab->buf);
		slab->buf = NULL;
		return -ENOMEM;
	}
	slab->size = size;
	slab->unit_size = unit_size;

	return 0;
}

void udk_slab_destroy(struct udk_slab *slab)
{
	if (slab == NULL) {
		return;
	}
	if (slab->buf != NULL) {
		free(slab->buf);
		slab->buf = NULL;
	}
	slab->size = 0;
	slab->unit_size = 0;
	slab->info = NULL;
}

int udk_slab_unit_alloc(struct udk_slab *slab, void **va)
{
	int ret;
	if (slab == NULL || va == NULL) {
		return -EINVAL;
	}
	ret = shmalloc_unit_alloc((struct share_mem_ctrl_info *)slab->info,
				  (uintptr_t *)va);
	return -hmerrno2posix(ret);
}

int udk_slab_unit_free(struct udk_slab *slab, const void *va)
{
	unsigned int offset;
	int ret;
	if (slab == NULL || va == NULL || va < slab->buf) {
		return -EINVAL;
	}
	offset = (unsigned int)ptr_diff(va, slab->buf);
	ret = shmalloc_unit_free((struct share_mem_ctrl_info *)slab->info,
				 offset);
	return -hmerrno2posix(ret);
}

unsigned int udk_slab_get_unit_free_num(struct udk_slab *slab)
{
	if (slab == NULL) {
		return 0;
	}
	return shmalloc_get_unit_free_num((struct share_mem_ctrl_info *)slab->info);
}
