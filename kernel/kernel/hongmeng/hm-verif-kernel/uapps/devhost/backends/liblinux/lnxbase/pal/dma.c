/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for dma operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <errno.h>
#include <hmasm/page.h>
#include <sys/mman.h>
#include <libmem/utils.h>
#include <devhost/memory.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/vmap.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libhmucap/ucap.h>

#include <devhost/log.h>

void *liblinux_pal_alloc_dma_pool(unsigned int *offset, unsigned long long *paddr)
{
	return devhost_dma_pool_alloc(paddr, offset);
}

void *liblinux_pal_alloc_dma_pool_ex(unsigned int *offset, unsigned long long *paddr,
				     unsigned int size)
{
	return devhost_dma_pool_alloc_ex(paddr, offset, size);
}

int liblinux_pal_free_dma_pool(unsigned int offset)
{
	int ret;

	ret = devhost_dma_pool_free(offset);
	return -hmerrno2posix(ret);
}

struct dma_vmap_node {
	struct vmap_node vmap;
	unsigned long long paddr;
	int shm_id;
};

static
void *__dma_alloc(unsigned int size, bool coherent, unsigned long long *paddr, const void *caller)
{
	struct dma_vmap_node *node = NULL;
	void *ptr = NULL;
	unsigned int align_size = PAGE_ALIGN_UP(size);

	node = (struct dma_vmap_node *)malloc(sizeof(struct dma_vmap_node));
	if (node == NULL) {
		return NULL;
	}
	mem_zero_s(*node);

	ptr = devhost_backend_mem_alloc(align_size,
					MPROT_READ | MPROT_WRITE | MPROT_DMA |
					(coherent ? MPROT_NOCACHE : 0),
					&node->paddr, &node->shm_id);
	if (ptr == MAP_FAILED) {
		goto err_alloc;
	}

	node->vmap.vaddr = ptr;
	node->vmap.size = align_size;
	node->vmap.flags = 0;
	if (coherent) {
		node->vmap.type = VMAP_TYPE_DMA_COHERENT;
	} else {
		node->vmap.type = VMAP_TYPE_DMA;
	}
	node->vmap.caller = caller;

	if (vmap_tree_insert(&node->vmap) < 0) {
		dh_debug("insert dma vmap_node failed, size=0x%lx\n",
			 node->vmap.size);
		goto err_insert;
	}

	if (paddr != NULL) {
		*paddr = node->paddr;
	}

	return ptr;

err_insert:
	devhost_backend_mem_free(node->shm_id, ptr, align_size);
err_alloc:
	free(node);
	return NULL;
}

void *liblinux_pal_dma_alloc(unsigned int size, unsigned long long *paddr)
{
	return __dma_alloc(size, false, paddr, lnxbase_return_address(1U));
}

void *liblinux_pal_dma_alloc_coherent(unsigned int size, unsigned long long *paddr)
{
	return __dma_alloc(size, true, paddr, lnxbase_return_address(1U));
}

int liblinux_pal_dma_free(const void *vaddr, unsigned int size)
{
	struct vmap_node *vmap = NULL;
	struct dma_vmap_node *node = NULL;
	unsigned int align_size = PAGE_ALIGN_UP(size);

	vmap = vmap_tree_remove(vaddr);
	if (vmap == NULL) {
		return -EINVAL;
	}

	node = container_of(vmap, struct dma_vmap_node, vmap);

	BUG_ON(align_size != node->vmap.size);

	devhost_backend_mem_free(node->shm_id, (void *)node->vmap.vaddr, node->vmap.size);

	free(node);

	return 0;
}
