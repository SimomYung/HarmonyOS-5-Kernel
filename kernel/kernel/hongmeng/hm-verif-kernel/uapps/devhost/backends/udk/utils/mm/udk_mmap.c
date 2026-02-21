/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Memory mapping routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:35:13 2019
 */
#include <udk/mm.h>

#include <limits.h>

#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libalgo/radix_tree.h>
#include <hmkernel/mm/pgtable.h>
#include <libdevhost/mm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>

#include <devhost/memory.h>

#include <udk/log.h>
#include <udk/cache.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/bitops.h>
#include <internal/init.h>

#define UDK_MMAP_RETRY_NUMS	512
#define UDK_MMAP_SLEEP_TIME	10000U

struct udk_mmap_arg_s {
	const void *addr;
	unsigned long len;
	int fd;
	uint32_t prot;
	uint32_t flags;
	unsigned long offset;
};

struct map_radix_tree {
	struct radix_tree_root root;
	struct udk_mutex lock;
};

struct map_entry {
	unsigned long vaddr;
	unsigned long long paddr;
	unsigned long size;
};

static struct map_radix_tree map_root = {
	.root = RADIX_TREE_INIT,
	.lock = UDK_MUTEX_INIT,
};

static void map_tree_lock()
{
	udk_mutex_lock(&map_root.lock);
}

static void map_tree_unlock()
{
	udk_mutex_unlock(&map_root.lock);
}

static int map_tree_insert(unsigned long vaddr, unsigned long long paddr, unsigned long size)
{
	int ret;
	struct map_entry *entry = udk_malloc(sizeof(struct map_entry));

	if (entry == NULL) {
		return -ENOMEM;
	}
	entry->vaddr = vaddr;
	entry->paddr = paddr;
	entry->size = size;

	map_tree_lock();
	ret = radix_tree_insert_item(&map_root.root, vaddr, (void *)entry);
	if (ret < 0) {
		udk_free(entry);
	}
	map_tree_unlock();

	return ret;
}

static unsigned long map_tree_delete(unsigned long vaddr)
{
	struct map_entry *entry = NULL;
	unsigned long size = 0UL;

	map_tree_lock();
	entry = (struct map_entry *)radix_tree_delete_item(&map_root.root, vaddr);
	if (entry != NULL) {
		size = entry->size;
		udk_free(entry);
	}
	map_tree_unlock();

	return size;
}

int udk_iomem_mmap(unsigned long vaddr,
		   unsigned long long paddr,
		   unsigned long size,
		   unsigned long prot,
		   unsigned int flags)
{
	int ret;

	ret = libdh_iomem_map(ulong_to_ptr(vaddr, void), paddr, size,
			      (unsigned int)prot, flags);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_iomem_mmap_batch(unsigned long vaddr,
			const struct mem_raw_ranges *pa_array,
			unsigned int pa_array_size,
			unsigned int prot,
			unsigned int flags)
{
	int ret;

	ret = libdh_iomem_map_batch(ulong_to_ptr(vaddr, void), pa_array, pa_array_size,
			      (unsigned int)prot, flags);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

void udk_iomem_munmap(unsigned long vaddr, unsigned long size)
{
	libdh_iounmap((void *)vaddr, size);
}

static void *__ioremap(unsigned long long paddr, unsigned long size, unsigned int flags)
{
	void *vaddr = NULL;
	int ret;

	vaddr = libdh_ioremap_ex(paddr, size, PROT_READ | PROT_WRITE, flags);
	if (vaddr != NULL) {
		ret = map_tree_insert((uintptr_t)vaddr, paddr, size);
		if (ret < 0) {
			libdh_iounmap(vaddr, size);
			vaddr = NULL;
		}
	}

	return vaddr;
}

void* udk_ioremap(unsigned long long paddr, unsigned long size)
{
	return __ioremap(paddr, size, LIBDH_REMAP_DEVICE);
}

void* udk_ioremap_cache(unsigned long long paddr, unsigned long size)
{
	return __ioremap(paddr, size, LIBDH_REMAP_CACHE);
}

void* udk_ioremap_nocache(unsigned long long paddr, unsigned long size)
{
	return __ioremap(paddr, size, LIBDH_REMAP_NORMAL_NC);
}

void udk_iounmap2(const void *vaddr)
{
	unsigned long size = 0UL;

	if (vaddr == NULL) {
		return;
	}
	size = map_tree_delete((uintptr_t)vaddr);
	if (size == 0) {
		udk_debug("query iomap size failed\n");
		return;
	}
	libdh_iounmap(vaddr, size);
}

static inline bool mmap_need_retry(int ret, uint32_t flags, int tries)
{
	bool retry;

	retry = (((ret == E_HM_NOMEM) &&
		(flags & MFLAG_MAP_RETRY) != 0U) && (tries > 0));

	if (retry) {
		(void)hm_usleep(UDK_MMAP_SLEEP_TIME);
	}

	return retry;
}

static int __try_mmap(uintptr_t *rvaddr, struct udk_mmap_arg_s *args)
{
	int tries = UDK_MMAP_RETRY_NUMS;
	struct __actvret_hmcall_mem_mmap actv_ret;
	int ret;

	mem_zero_s(actv_ret);
	do {
		ret = actvxactcall_hmcall_mem_mmap(true, /* is_fwd */
						   true, /* allow_refwd */
						   args->addr, args->len, args->prot,
						   args->flags,
						   args->fd, args->offset,
						   &actv_ret);
	} while (mmap_need_retry(ret, args->flags, tries--));

	if (ret == E_HM_OK) {
		*rvaddr = (uintptr_t)actv_ret.rvaddr;
	}

	return ret;
}

int udk_mmap(unsigned long addr, unsigned long length,
	     unsigned long prot, unsigned int flags,
	     int fd, unsigned long offset, uintptr_t *rvaddr)
{
	int ret;
	struct udk_mmap_arg_s args = {
		.addr = ulong_to_ptr(addr, void),
		.len = length,
		.fd = fd,
		.prot = (uint32_t)prot,
		.flags = flags,
		.offset = offset,
	};
	ret = __try_mmap(rvaddr, &args);
	if (ret != E_HM_OK) {
		*rvaddr = ptr_to_type(NULL, uintptr_t);
	}

	return ret;
}
