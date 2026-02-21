/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>
#include <lnxbase/iofast.h>
#include <lnxbase/vmap.h>

#include <stdbool.h>
#include <lib/dlist.h>
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>

#include <devhost/log.h>
#include <devhost/cfg.h>
#include <devhost/memory.h>

#include "mempool.h"
#include "internal.h"
#include "page_pool.h"

#ifdef CONFIG_PAGE_ALLOC_VMAP
struct vmap_shm_node {
	struct vmap_node node;
	int shm_id;
};
#endif

static DLIST_HEAD(ldk_reclaim_func_list);

struct ldk_reclaim_func {
	struct dlist_node node;
	const char* name;
	void (*reclaim)(void);
};

#define LDK_MM_WATERMARK_DEFAULT_ORDER (15)
static unsigned long g_mm_max_watermark = 0;
static raw_atomic_ullong_t g_total_ldk_mm = RAW_ATOMIC_VAL_INIT(0);

static struct raw_mutex __populate_lock = RAW_MUTEX_INITIALIZER;

static struct liblinux_page_allocator *common_allocator = NULL;

static struct liblinux_kmap_info g_liblinux_kmap_info = {0};

static struct page_pool *pool_creat_default(unsigned int min_alloc_shift)
{
	struct page_pool *pool = NULL;
	struct pool_info *default_pool_info = NULL;
	int ret;

	default_pool_info = malloc(sizeof(struct pool_info));
	if (default_pool_info == NULL) {
		return pool;
	}
	ret = sprintf_s(default_pool_info->name, POOL_NAME_LEN, "%s", "common");
	if (ret < 0) {
		free(default_pool_info);
		return NULL;
	}
	default_pool_info->type = DEVHOST_POOL_TYPE_MEM;
	default_pool_info-> sizeorder = 0;
	default_pool_info->reclaimorder = LDK_MM_WATERMARK_DEFAULT_ORDER;
	pool = pool_creat_mem(default_pool_info, min_alloc_shift);
	if (pool == NULL) {
		free(default_pool_info);
	}

	return pool;
}

static void __kmap_info_init(const struct liblinux_kmap_info *info,
			     unsigned long long memstart)
{
	if (g_liblinux_kmap_info.kmap_virt_start != 0) {
		return;
	}

	g_liblinux_kmap_info = *info;
	g_liblinux_kmap_info.phys_offset = memstart;
}

struct liblinux_kmap_info lnxbase_get_kmap_info(void)
{
	return g_liblinux_kmap_info;
}

static int __kmap_init(unsigned long long memstart,
		       unsigned int min_alloc_shift,
		       const struct liblinux_kmap_info *info,
		       struct liblinux_page_allocator **allocator)
{
	struct page_pool *pool = NULL;
	struct page_pool *dma_pool = NULL;
	struct page_pool *slow_pool = NULL;
	struct page_pool *rsv_pool = NULL;
	int ret;

	/* init page pool first */
	ret = lnxbase_page_pool_init(min_alloc_shift);
	if (ret < 0) {
		return ret;
	}

	/* try get and setup default pool */
	pool = lnxbase_page_pool_get("common");
	if (pool == NULL) {
		/* create one  if no default pool configured */
		pool = pool_creat_default(min_alloc_shift);
		BUG_ON(pool == NULL);
	}
	ret = lnxbase_page_pool_setup_linear(pool,
					     info->kmap_virt_start,
					     info->kmap_virt_end - info->kmap_virt_start,
					     (unsigned long)memstart);
	if (ret < 0) {
		dh_error("default page pool setup failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	dma_pool = lnxbase_page_pool_get("dma");
	if (dma_pool != NULL) {
		ret = lnxbase_page_pool_setup_default(dma_pool);
		if (ret < 0) {
			dh_error("default dma pool setup failed, ret=%s\n", hmstrerror(ret));
			return ret;
		}
	}

	slow_pool = lnxbase_page_pool_get("slow");
	if (slow_pool != NULL) {
		ret = lnxbase_page_pool_setup_default(slow_pool);
		if (ret < 0) {
			dh_error("default slow pool setup failed, ret=%s\n", hmstrerror(ret));
			return ret;
		}
	}

	rsv_pool = lnxbase_page_pool_get("rsv");
	if (rsv_pool != NULL) {
		ret = lnxbase_page_pool_setup_default(rsv_pool);
		if (ret < 0) {
			dh_error("default rsv pool setup failed, ret=%s\n", hmstrerror(ret));
			return ret;
		}
	}

	*allocator = pool->page_allocator;
	__kmap_info_init(info, memstart);

	return E_HM_OK;
}

int liblinux_pal_page_alloc_init(struct liblinux_pool_base *pool_base,
				 struct liblinux_ram_info *ram_info)
{
	struct liblinux_kmap_info info;
	int ret;
	void *start;
	unsigned long size;
	const char *range_name = "[liblinux/KMAP-RSVD]";

	BUG_ON(pool_base == NULL);
	BUG_ON(ram_info == NULL);

	ret = lnxbase_iofast_declare(range_name, pool_base);
	if (ret < 0) {
		start = ulong_to_ptr(pool_base->kmap_virt_start, void);
		size = pool_base->kmap_virt_end - pool_base->kmap_virt_start;
		/* reserve virtual address space for ldk kernel mapping with NONE */
		ret = liblinux_pal_reserve_range_name(start, size, PROT_NONE,
						      range_name);
		if (ret < 0) {
			dh_error("reserve range for ldk KMAPS failed: %d\n", ret);
			return ret;
		}
	}

	/* page alloc config */
	info.kmap_virt_start = pool_base->kmap_virt_start,
	info.kmap_virt_end = pool_base->kmap_virt_end,
	info.phys_offset = pool_base->phys_offset,
	info.page_sizeorder = pool_base->page_sizeorder,

	ret = liblinux_pal_page_alloc_init_ex(ram_info,
					      &common_allocator,
					      pool_base->page_sizeorder,
					      &info);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

#define SZ_1M	0x00100000
/* error need not rollback because it will panic */
int liblinux_pal_page_alloc_init_ex(struct liblinux_ram_info *ram_info,
				    struct liblinux_page_allocator **allocator,
				    unsigned int min_alloc_shift,
				    const struct liblinux_kmap_info *info)
{
	int ret;
	struct lnxbase_sysinfo info_ret;
	unsigned int cfg_total_watermark;

	BUG_ON(ram_info == NULL);
	BUG_ON(allocator == NULL);

	/* try to get memstart and total_ram from sysmgr */
	mem_zero_s(info_ret);
	ret = lnxbase_sysinfo_get(&info_ret);
	if (ret < 0) {
		dh_error("query sysinfo failed, ret=%d\n", ret);
		return ret;
	}

	ram_info->memstart = (unsigned long long)info_ret.memstart;
	ram_info->memend = (unsigned long long)info_ret.memend;
	ram_info->total_ram = (unsigned long long)info_ret.total_ram;

	cfg_total_watermark = devhost_get_poolcfg_total_watermark();
	if (cfg_total_watermark != 0) {
		g_mm_max_watermark = (unsigned long)cfg_total_watermark * SZ_1M;
	}

	/* configurable mempool */
	if ((info != NULL) && (info->kmap_virt_start != 0)) {
		ret = __kmap_init((info->phys_offset == 0) ? ram_info->memstart
							   : info->phys_offset,
				  min_alloc_shift, info,
				  allocator);
		return -hmerrno2posix(ret);
	}

	/* fallback to legacy default mempool */
	*allocator = mempool_allocator_init(min_alloc_shift);

	return 0;
}

struct liblinux_page_allocator *liblinux_pal_page_allocator_get(char *allocator_name)
{
	struct page_pool *pool = lnxbase_page_pool_get(allocator_name);
	if (pool == NULL) {
		return NULL;
	}
	return pool->page_allocator;
}

int liblinux_pal_reserve_range_name(void *start, unsigned long size, int prot,
				    const char *name)
{
	void *rvaddr = NULL;

	if (!PAGE_ALIGNED(start) || !PAGE_ALIGNED(size)) {
		return -EINVAL;
	}

	rvaddr = hm_mem_mmap(start, (size_t)size, (uint32_t)prot,
			     MAP_PRIVATE | MAP_ANONYMOUS | MFLAG_MAP_NOPOPULATE, 0, 0);
	if (rvaddr == MAP_FAILED) {
		return -ENOMEM;
	}
	if ((char *)rvaddr != start) {
		(void)hm_mem_munmap(rvaddr, (size_t)size);
		return -ENOMEM;
	}

	/* set optional anon memory name */
	if (name != NULL) {
		(void)hm_mem_set_anon_name(start, (size_t)size, name);
	}

	return 0;
}

#ifdef CONFIG_PAGE_ALLOC_VMAP
static int vmap_shm_node_release(struct vmap_node *node)
{
	int ret = E_HM_OK;
	void *addr = NULL;
	struct vmap_shm_node *vnode = container_of(node, struct vmap_shm_node, node);
	/* protect memory address for ldk */
	addr = hm_mem_mmap((void *)node->vaddr, (size_t)node->size, PROT_NONE,
			   MFLAG_MAP_PRIVATE | MFLAG_MAP_FIXED | MFLAG_MAP_ANONYMOUS,
			   -1, 0);
	if (addr == MAP_FAILED) {
		dh_warn("map memory address to PORT_NONE when release vmap_shm_node failed, shm_id=%d, size=0x%lx\n",
			vnode->shm_id, node->size);
		return -EFAULT;
	} else {
		(void)raw_atomic_ullong_sub_relaxed(&g_total_ldk_mm, node->size);
	}
	ret = __devhost_shm_mem_destroy(vnode->shm_id);
	if (ret != E_HM_OK) {
		dh_error("devhost shm_mem destory failed, shm_id=%d, err=%s\n",
			 vnode->shm_id, hmstrerror(ret));
	}
	free(vnode);

	return -hmerrno2posix(ret);
}

static int __add_shm_mem_to_vmap(const void *vaddr, unsigned long size, int shm_id)
{
	int ret = 0;

	/* freed in the vmap_shm_node_release */
	struct vmap_shm_node *node = (struct vmap_shm_node *)malloc(sizeof(struct vmap_shm_node));
	if (node == NULL) {
		dh_error("vmap_shm_node alloc failed\n");
		ret = -EINVAL;
	} else {
		mem_zero_s(*node);
		node->node.vaddr = vaddr;
		node->node.size = size;
		node->node.type = VMAP_TYPE_KMAP;
		node->shm_id = shm_id;
		node->node.release = vmap_shm_node_release;
		ret = vmap_tree_insert(&node->node);
		if (ret != 0) {
			free(node);
			dh_error("vmap_shm_node add to vmap tree failed, err=%d\n", ret);
		}
	}

	return ret;
}
#endif

static uint64_t __shm_oflags_from_pal_gfp_flags(unsigned int flags)
{
	uint64_t oflags = SHM_O_CREAT | SHM_O_EXCL | SHM_O_CELL | SHM_O_RDWR;

	if (____IS((unsigned int)flags & LIBLINUX_PAL_GFP_NORETRY)) {
		oflags |= SHM_O_NO_OOM;
	}

	if (____IS((unsigned int)flags & LIBLINUX_PAL_GFP_DMA)) {
		oflags |= SHM_O_DMA;
	}

	if (____IS((unsigned int)flags & LIBLINUX_PAL_GFP_NOFAIL)) {
		oflags |= SHM_O_NOFAIL;
	}

	return oflags;
}

struct __memrange {
	unsigned long start;
	unsigned long end;
};

#define MFLAG_EXT (MFLAG_MAP_PRIVATE | MFLAG_MAP_FIXED | MFLAG_MAP_ANONYMOUS)

static int __try_extend(struct __memrange range, unsigned long size, unsigned long *rvaddr,
			unsigned long long *rpaddr, int flags)
{
	void *vaddr = NULL;
	unsigned long long paddr = ~0ULL;
	int shm_id;
	int ret;
	uint64_t oflags = __shm_oflags_from_pal_gfp_flags((unsigned int)flags);
	unsigned long start = range.start;
	unsigned long end = range.end;
	unsigned int order = SIZE_TO_MEM_ORDER(size);

	if ((rvaddr == NULL) || (rpaddr == NULL)) {
		return -EINVAL;
	}

	/* the alloc does need order aligned, otherwise mmap will fail */
	ret = __devhost_shm_mem_alloc((size_t)MEM_ORDER_TO_SIZE(order), oflags, &paddr, &shm_id);
	if (ret < 0) {
		ret = -hmerrno2posix(ret);
		goto err_shm_alloc;
	}

	/* check overflow */
	/* `start + paddr` may overflow for arm32, but we need this overflow :) */
	if (unlikely((unsigned long)(start + paddr) > end)) {
		hm_panic("physical address out of range\n");
	}

	/* map shm mem to target vspace, size must be order aligned */
	vaddr = hm_mem_mmap(ulong_to_ptr(start + paddr, void), MEM_ORDER_TO_SIZE(order), PROT_READ | PROT_WRITE,
			    MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS | MFLAG_MAP_POPULATE | MFLAG_MAP_FIXED, shm_id, 0);
	if (vaddr == MAP_FAILED) {
		dh_error("shm mem map failed, shm_id=%d, size=0x%lx\n", shm_id, size);
		ret = -ENOMEM;
		goto err_shm_map;
	}

	if (size != MEM_ORDER_TO_SIZE(order)) {
		(void)hm_mem_mmap(ptr_add(vaddr, size), MEM_ORDER_TO_SIZE(order) - size, PROT_NONE, MFLAG_EXT, -1, 0);
	}
#ifdef CONFIG_PAGE_ALLOC_VMAP
	ret = __add_shm_mem_to_vmap(vaddr, size, shm_id);
	if (ret != E_HM_OK) {
		goto err_shm_add_vmap;
	}
#endif
	(void)hm_mem_set_anon_name(vaddr, (size_t)size, "[liblinux/KMAP]");

	/* ret values */
	*rvaddr = ptr_to_ulong(vaddr);
	*rpaddr = paddr;

	return 0;
#ifdef CONFIG_PAGE_ALLOC_VMAP
err_shm_add_vmap:
	(void)hm_mem_mmap(vaddr, (size_t)size, PROT_NONE, MFLAG_EXT, -1, 0);
#endif
err_shm_map:
	(void)__devhost_shm_mem_destroy(shm_id);
err_shm_alloc:
	return ret;
}

void lnxbase_register_reclaim_func(void (*func)(void), const char *name)
{
	struct ldk_reclaim_func* reclaim_func = (struct ldk_reclaim_func*)malloc(sizeof(struct ldk_reclaim_func));
	if (reclaim_func != NULL) {
		reclaim_func->reclaim = func;
		reclaim_func->name = name;
		dlist_insert_tail(&ldk_reclaim_func_list, &reclaim_func->node);
		dh_debug("shrinker %s registered\n", name);
	} else {
		dh_warn("ldk_reclaim for %s malloc failed\n", name);
	}
}

int lnxbase_page_pool_watermark_reached(void)
{
	if (g_mm_max_watermark == 0) {
		return 0;
	}
	return ((unsigned long)raw_atomic_ullong_read_relaxed(&g_total_ldk_mm) > g_mm_max_watermark);
}

int liblinux_pal_page_alloc_extend(unsigned long start, unsigned long end,
				   unsigned long size,
				   unsigned long *rvaddr,
				   unsigned long long *rpaddr,
				   int flags)
{
	int ret;
	struct __memrange range = {
		.start = start,
		.end = end,
	};

	ret = __try_extend(range, size, rvaddr, rpaddr, flags);
	if (ret == 0) {
		(void)raw_atomic_ullong_add_relaxed(&g_total_ldk_mm, size);
	}

	return ret;
}

static int vmap_kmap_release(struct vmap_node *vnode)
{
	/* vnode->vaddr and vnode->size must be PAGE_ALIGNED */
	BUG_ON((uintptr_t)vnode->vaddr % PAGE_SIZE != 0);
	BUG_ON(vnode->size % PAGE_SIZE != 0);

	if (raw_refcnt_nr_counter(&vnode->refcnt) == 0) {
		void *raddr = hm_mem_mmap((void *)vnode->vaddr, vnode->size, PROT_NONE,
				    MFLAG_MAP_PRIVATE | MFLAG_MAP_FIXED | MFLAG_MAP_ANONYMOUS,
				    -1, 0);
		if (raddr == MAP_FAILED) {
			dh_warn("populate release mmap node failed\n");
			return -EINVAL;
		}
	}

	free(vnode);
	return 0;
}

static struct vmap_node *kmap_get_vnode(void *addr, unsigned long size, uint32_t prot)
{
	struct vmap_node *node = NULL;
	int ret;

	ret = mprotect(addr, (size_t)size, (int)prot);
	if (ret < 0) {
		dh_error("page populate mprotect failed, ret=%d\n", ret);
		return NULL;
	}

	node = vmap_get_vnode(VMAP_TYPE_KMAP, FLAG_VMAP_MERGED);
	if (node == NULL) {
		dh_error("%s get vnode failed\n", __func__);
		return NULL;
	}

	node->vaddr = addr;
	node->size = size;
	node->release = vmap_kmap_release;

	return node;
}

static int populate_mem_add(unsigned long pop_start, unsigned long pop_end)
{
	int ret;
	void *start = ulong_to_ptr(pop_start, void);
	unsigned long size = pop_end - pop_start;
	uint32_t prot = PROT_READ | PROT_WRITE;
	struct vmap_node *node = NULL;

	if (lnxbase_is_vmemmap(start, size)) {
		node = vmemmap_get_vnode(start, size, prot);
	} else {
		node = kmap_get_vnode(start, size, prot);
	}
	if (node == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	ret = vmap_tree_insert_merge(node);
	if (ret < 0) {
		dh_error("insert vmap node failed, size=0x%lx, ret=%d\n",
			 size, ret);
		node->release(node);
	}

err:
	return ret;
}

int liblinux_pal_page_alloc_populate(const void *start, unsigned long size)
{
	unsigned long vaddr_start, vaddr_end;
	int ret;
	struct vmap_node *node;

	if (size == 0) {
		return -EINVAL;
	}
	if (lnxbase_is_vmemmap_populate_all(start, size) == true) {
		return 0;
	}

	vaddr_start = ALIGN_DOWN(ptr_to_ulong(start), PAGE_SIZE);
	vaddr_end = ALIGN_UP(ptr_to_ulong(start) + size, PAGE_SIZE);

	RAW_MUTEX_GUARD(_, &__populate_lock);

	for (unsigned long addr = vaddr_start; addr < vaddr_end; addr += PAGE_SIZE) {
		node = vmap_tree_find(ulong_to_ptr(addr, void));
		if (node == NULL) {
			ret = populate_mem_add(addr, addr + PAGE_SIZE);
			if (ret < 0) {
				dh_error("page populate mem add failed, ret=%d\n", ret);
				return ret;
			}
			continue;
		}
		if (node != NULL) {
			ret = vmap_tree_refcnt_split(ulong_to_ptr(addr, void), PAGE_SIZE, VMAP_REF_GET);
			if (ret < 0) {
				dh_error("page populate refcnt split failed, ret=%d\n", ret);
				return ret;
			}
		}
	}

	return 0;
}

int liblinux_pal_page_alloc_unpopulate(const void *start, unsigned long size)
{
	unsigned long vaddr_start;
	unsigned long vaddr_end;
	int ret;
	struct vmap_node *node;
	unsigned long unpop_start;
	unsigned long unpop_size;

	if (size == 0) {
		return -EINVAL;
	}
	if (lnxbase_is_vmemmap_populate_all(start, size) == true) {
		return 0;
	}

	vaddr_start = ALIGN_DOWN(ptr_to_ulong(start), PAGE_SIZE);
	vaddr_end = ALIGN_UP(ptr_to_ulong(start) + size, PAGE_SIZE);

	unpop_start = vaddr_start;

	RAW_MUTEX_GUARD(_, &__populate_lock);

	while (unpop_start < vaddr_end) {
		node = vmap_tree_find(ulong_to_ptr(unpop_start, void));
		if (node == NULL) {
			unpop_start += PAGE_SIZE;
			continue;
		}
		unpop_size = (ptr_to_ulong(node->vaddr) > (vaddr_end - node->size)) ?
			vaddr_end - unpop_start : ptr_to_ulong(node->vaddr) + node->size - unpop_start;
		ret = vmap_tree_refcnt_split(ulong_to_ptr(unpop_start, void), unpop_size, VMAP_REF_PUT);
		if (ret < 0) {
			dh_error("page unpopulate refcnt split failed, ret=%d\n", ret);
			return ret;
		}
		unpop_start += unpop_size;
	}

	return 0;
}
