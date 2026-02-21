/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: liblinux PAL implement for highmem
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 13 10:45:12 2023
 */
#include <liblinux/pal.h>

#include <libhmsrv_sys/hm_mem.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libmem/utils.h>

#include <devhost/log.h>
#include <devhost/memory.h>

#include <lnxbase/iofast.h>
#include <lnxbase/vmap.h>
#include "internal.h"

struct lnxbase_meminfo {
	struct liblinux_vmmemmap_info info;
	int shmid;
	bool flag;
};

static struct lnxbase_meminfo g_hmem = {
	.info = {0},
	.shmid = -1,
	.flag = false,
};

static DEFINE_VMAP_TREE(__vmemmap_tree);

static raw_atomic_ullong_t g_vmemmap_rsv_nr_pages = RAW_ATOMIC_VAL_INIT(0);
static unsigned long vmemmap_rsv_pages_stat(void)
{
	return (unsigned long)raw_atomic_ullong_read_relaxed(&g_vmemmap_rsv_nr_pages);
}

/* Only handle start and end is PAGE_ALIGNED */
bool lnxbase_is_vmemmap(const void *start, unsigned long size)
{
	struct liblinux_vmmemmap_info *info = &g_hmem.info;
	if ((!g_hmem.flag) || (size == 0)) {
		return false;
	}

	return (ptr_to_ulong(start) >= info->vmemmap_start) &&
	       ((ptr_to_ulong(start) + size -1) < (info->vmemmap_start + info->vmemmap_size));
}

bool lnxbase_is_vmemmap_populate_all(const void *start, unsigned long size)
{
	struct liblinux_vmmemmap_info *info = &g_hmem.info;

	if (lnxbase_is_vmemmap(start, size) && (____IS(info->flags & LIBLINUX_VMEMMAP_POPULATE_ALL))) {
		return true;
	}
	return false;
}

static void __highmem_page_init(void *addr, unsigned long size)
{
	size_t i;
	size_t page_size = (size_t)g_hmem.info.page_size;
	size_t page_cnt = (size_t)size / page_size;
	void *page_addr = ulong_to_ptr(g_hmem.info.page_addr, void);

	for(i = 0; i < page_cnt; i++) {
		void *tmp = ulong_to_ptr(ptr_to_ulong(addr) + i * page_size, void);
		NOFAIL(memcpy_s(tmp, page_size, page_addr, page_size));
	}
}

/* need to test no malloc performance */
static int __highmem_page_mapfix_rsv(const void *addr, int page_num)
{
	size_t i;
	int ret = 0;
	size_t step = PAGE_SIZE;
	size_t cnt = (size_t)(unsigned int)page_num / ((size_t)PAGE_SIZE / g_hmem.info.page_size);
	void *page_addr = NULL;

	page_addr = (void *)malloc(PAGE_SIZE);
	if (page_addr == NULL) {
		return -ENOMEM;
	}
	__highmem_page_init(page_addr, PAGE_SIZE);

	for (i = 0; i < cnt; i++) {
		void *tmp = ulong_to_ptr(ptr_to_ulong(addr) + i * step, void);
		NOFAIL(memcpy_s(tmp, step, page_addr, step));
	}

	free(page_addr);
	return ret;
}

static int __vmemmap_map_init(void *priv)
{
	struct liblinux_vmmemmap_info *info = (struct liblinux_vmmemmap_info *)priv;
	NOFAIL(memcpy_s(&g_hmem.info, sizeof(g_hmem.info), (void *)info, sizeof(*info)));
	lnxbase_stat_func_register(LNXBASE_VMEMMAP_RSV_PAGES, vmemmap_rsv_pages_stat);
	return 0;
}

static int __vmemmap_map_done(void *priv)
{
	UNUSED(priv);
	g_hmem.flag = true;
	return 0;
}

static int __vmemmap_populate_all(void *priv, void *addr, unsigned long size)
{
	struct vmap_node *node = NULL;
	int ret = 0;

	UNUSED(priv);

	/* reserve virtual address space for `page` with READ and WRITE */
	node = vmemmap_get_vnode(addr, size, PROT_READ | PROT_WRITE);
	if (node == NULL) {
		dh_error("create vmemmap node of size 0x%lx failed\n", size);
		return -ENOMEM;
	}

	ret = __vmap_tree_insert_merge(&__vmemmap_tree, node);
	if (ret < 0) {
		dh_error("insert vmemmap node of size 0x%lx failed, ret=%d\n",
			 size, ret);
		node->release(node);
	}

	/* optional: set anon name */
	(void)hm_mem_set_anon_name(addr, (size_t)size, "[liblinux/VMEMMAP]");
	(void)raw_atomic_ullong_add_relaxed(&g_vmemmap_rsv_nr_pages, (uint64_t)(size / PAGE_SIZE));

	return ret;
}

static int __highmem_page_mapfix(const void *addr, int page_num, uint32_t flags)
{
	size_t i;
	int ret = 0;
	size_t step = PAGE_SIZE;
	size_t cnt = (size_t)(unsigned int)page_num / ((size_t)PAGE_SIZE / g_hmem.info.page_size);

	for (i = 0; i < cnt; i++) {
		int err = E_HM_OK;
		void *tmp = ulong_to_ptr(ptr_to_ulong(addr) + i * step, void);
		(void)hm_mem_mmap_posix(tmp, step, PROT_READ, flags,
					g_hmem.shmid, 0, &err);
		/* When process return err,we try to set addr status which can set in loop. So we continue instead of return/break */
		if (err < 0) {
			hm_error_ratelimited("highmem page mapfix failed:%s\n", hmstrerror(err));
			ret = -EINVAL;
		}
	}

	return ret;
}

static int __prepare_highmem(void)
{
	int ret = 0;
	void *addr = NULL;
	unsigned long long highmem_page_paddr = 0;
	uint32_t flags = MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS;
	uint32_t oflags = SHM_O_CREAT | SHM_O_EXCL | SHM_O_CELL | SHM_O_RDWR | SHM_O_CELL_KEEP;

	/* Only support PAGE_SIZE is page_size aligned */
	if (PAGE_SIZE % g_hmem.info.page_size != 0) {
		return -EINVAL;
	}

	/* prepare struct page of one page */
	ret = __devhost_shm_mem_alloc(PAGE_SIZE, oflags, &highmem_page_paddr, &g_hmem.shmid);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	addr = hm_mem_mmap_posix(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, flags, g_hmem.shmid, 0, &ret);
	if (addr == MAP_FAILED) {
		return -hmerrno2posix(ret);
	}

	__highmem_page_init(addr, PAGE_SIZE);

	(void)hm_mem_munmap(addr, PAGE_SIZE);

	return ret;
}

static int __highmem_map_init(void *priv)
{
	int ret = 0;
	void *highmem_pg_template = NULL;
	struct liblinux_vmmemmap_info *info = (struct liblinux_vmmemmap_info *)priv;

	/* record highmem info */
	highmem_pg_template = malloc((size_t)info->page_size);
	if (highmem_pg_template == NULL) {
		return -ENOMEM;
	}
	NOFAIL(memcpy_s(highmem_pg_template, (size_t)info->page_size,
			(void *)(uintptr_t)info->page_addr, (size_t)info->page_size));
	g_hmem.info.page_addr = ptr_to_ulong(highmem_pg_template);

	if (!____IS(info->flags & LIBLINUX_VMEMMAP_POPULATE_ALL)) {
		/* prepare highmem page */
		ret = __prepare_highmem();
		if (ret < 0) {
			free(highmem_pg_template);
			g_hmem.info.page_addr = 0;
			dh_error("prepare highmem failed: %d\n", ret);
			return ret;
		}
	}

	return ret;
}

static int __highmem_map(void *priv, void *addr, unsigned long size)
{
	int ret = 0;
	uint32_t flags = MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS | MFLAG_MAP_POPULATE | MFLAG_MAP_FIXED;
	struct liblinux_vmmemmap_info *info = (struct liblinux_vmmemmap_info *)priv;

	if (____IS(info->flags & LIBLINUX_VMEMMAP_POPULATE_ALL)) {
		/* set highmem for all reserved vmemmap */
		ret = __highmem_page_mapfix_rsv(addr, (int)(size / g_hmem.info.page_size));
		if (ret < 0) {
			dh_error("highmem mapfix rsv size=0x%lx failed: %d\n",
				 size, ret);
			return ret;
		}
	} else {
		/* set highmem for all valid vmemmap */
		ret = __highmem_page_mapfix(addr, (int)(size / g_hmem.info.page_size), flags);
		if (ret < 0) {
			dh_error("highmem mapfix size=0x%lx failed: %d\n",
				 size, ret);
			return ret;
		}
	}

	return ret;
}

static int __vmemmap_populate_done(void *priv)
{
	UNUSED(priv);
	vmap_tree_set_readonly(&__vmemmap_tree);
	return E_HM_OK;
}

static int __vmemmap_iofast(struct liblinux_vmmemmap_info *info)
{
	int ret;
	struct __actvret_hmcall_mem_iomap_prepare ioprep_ret;

	mem_zero_s(ioprep_ret);
	ret = actvcall_hmcall_mem_iomap_prepare((void *)(uintptr_t)info->vmemmap_start, info->vmemmap_size, 0,
						(unsigned int)(PROT_READ | PROT_WRITE),
						MFLAG_MAP_PRIVATE | MFLAG_MAP_FIXED,
						&ioprep_ret);
	if (ret < 0) {
		dh_error("vmemmap iofast iomap prepare failed: %d\n", ret);
	}

	return ret;
}

static void __vmemmap_iofast_tree_insert(void *vaddr, unsigned long size)
{
	int ret = 0;
	struct vmap_node *node = NULL;

	node = vmap_get_vnode(VMAP_TYPE_VMEMMAP, FLAG_VMAP_MERGED);
	if (node == NULL) {
		dh_error("vmemmap get vnode failed\n");
		return;
	}
	node->vaddr = vaddr;
	node->size = size;

	ret = __vmap_tree_insert_merge(&__vmemmap_tree, node);
	if (ret < 0) {
		dh_error("insert vmemmap iofast node of size 0x%lx failed, ret=%d\n",
			 size, ret);
		free(node);
	}
}

static void __vmemmap_iofast_free_unused(unsigned long long pa, uint32_t order, unsigned long size)
{
	unsigned long long paddr;
	unsigned long long pa_end = pa + PAGE_ALIGN_UP(size);
	unsigned long long pa_free_end = pa + MEM_ORDER_TO_SIZE(order);
	int ret = 0;

	while (pa_end < pa_free_end) {
		order = SIZE_TO_MEM_ORDER(pa_free_end - pa_end);
		if (order) {
			order--;
		}

		paddr = pa_end | (order & (~PAGE_MASK));
		ret = lnxbase_iofast_free_pfn_v(&paddr, 1);
		if (ret == E_HM_OK) {
			(void)raw_atomic_ullong_sub_relaxed(&g_vmemmap_rsv_nr_pages, (1 << order));
		}
		pa_end += MEM_ORDER_TO_SIZE(order);
	}
}

static int __vmemmap_iofast_populate_all(void *priv, void *vaddr, unsigned long size)
{
	int ret;
	unsigned int pa_num = 0;
	unsigned long long paddr = 0;
	unsigned int order = SIZE_TO_MEM_ORDER(size);
	uint64_t hint = prot_to_hint(PROT_READ | PROT_WRITE, 0, 0);

	UNUSED(priv);
	ret = lnxbase_iofast_alloc_pfn_v(order, &paddr, 1UL,
		LIBLINUX_PAL_GFP_CONTINUOUS | LIBLINUX_PAL_GFP_NORETRY | LIBLINUX_PAL_GFP_DETACH | LIBLINUX_PAL_GFP_ZERO,
		&pa_num);
	if (ret < 0) {
		dh_error("vmemmap iofast iomap alloc size %lu failed: %d\n", size, ret);
		return ret;
	}

	ret = actvcall_hmcall_mem_iomap_populate(vaddr, size, paddr & PAGE_MASK, hint);
	if (ret < 0) {
		dh_error("vmemmap iofast iomap populate size %lu failed: %d\n", size, ret);
		(void)lnxbase_iofast_free_pfn_v(&paddr, 1);
		return ret;
	}

	(void)raw_atomic_ullong_add_relaxed(&g_vmemmap_rsv_nr_pages, (1 << order));

	__vmemmap_iofast_free_unused(paddr & PAGE_MASK, order, size);
	__vmemmap_iofast_tree_insert(vaddr, size);
	return ret;
}

static struct vmemmap_ops g_vmemmap_ops[] __read_mostly = {
	{__vmemmap_map_init, NULL, __vmemmap_map_done, ~0U},
	{NULL, __vmemmap_populate_all, __vmemmap_populate_done, LIBLINUX_VMEMMAP_POPULATE_ALL},
	{__highmem_map_init, __highmem_map, NULL, LIBLINUX_VMEMAP_HIGHMEM},
	{NULL, __vmemmap_iofast_populate_all, NULL, LIBLINUX_VMEMMAP_POPULATE_IOFAST},
};

/* error need not rollback because it will panic */
int liblinux_pal_vmemmap_init(struct liblinux_vmmemmap_info *info)
{
	int ret = 0;
	struct vmemmap_init_ops vmemmap_init_ops = {
			.map_ops = g_vmemmap_ops,
			.ops_num = ARRAY_SIZE(g_vmemmap_ops),
			.ops_flags = info->flags,
	};

	/* can not handle `struct page` cross page */
	BUG_ON((PAGE_SIZE % info->page_size) != 0);
	if (info->flags == LIBLINUX_VMEMMAP_POPULATE_IOFAST) {
		ret = __vmemmap_iofast(info);
	} else {
		ret = liblinux_pal_reserve_range_name((void *)(uintptr_t)info->vmemmap_start,
				(unsigned long)info->vmemmap_size, PROT_READ | PROT_WRITE, "[liblinux/VMEMMAP]");
	}
	if (ret < 0) {
		dh_error("reserve range for VMEMMAP (size=0x%lx) failed: %d\n",
			 (unsigned long)info->vmemmap_size, ret);
		return ret;
	}

	ret = lnxbase_vmemmap_init(ulong_to_ptr(info->vmemmap_start, void),
				   (unsigned long)(info->page_size),
				   &vmemmap_init_ops, (void *)info);
	if (ret < 0) {
		dh_error("vmemmap map failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int __vmap_vmemmap_release(struct vmap_node *vnode)
{
	int ret = 0;
	uint32_t flags = MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS |  MFLAG_MAP_FIXED;

	/* vnode->vaddr and vnode->size must be PAGE_ALIGNED */
	BUG_ON((uintptr_t)vnode->vaddr % PAGE_SIZE != 0);
	BUG_ON(vnode->size % PAGE_SIZE != 0);

	if (raw_refcnt_nr_counter(&vnode->refcnt) == 0) {
		ret = __highmem_page_mapfix(vnode->vaddr, (int)(vnode->size / PAGE_SIZE), flags);
		if (ret < 0) {
			ret = -EINVAL;
		}
	}

	free(vnode);
	return ret;
}

struct vmap_node *vmemmap_get_vnode(void *start, unsigned long size, uint32_t prot)
{
	void *ptr = NULL;
	struct vmap_node *node = NULL;
	int ret = 0;

	if (!IS_ALIGNED(size, PAGE_SIZE)) {
		return NULL;
	}

	node = vmap_get_vnode(VMAP_TYPE_VMEMMAP, FLAG_VMAP_MERGED);
	if (node == NULL) {
		dh_error("vmemmap get vnode failed\n");
		return NULL;
	}

	ptr = hm_mem_mmap_posix(start, size, prot,
				MAP_PRIVATE | MFLAG_MAP_FIXED | MAP_ANONYMOUS,
				-1, 0, &ret);
	if (ptr == MAP_FAILED) {
		dh_error("page mmapfix failed, ret=%s\n", hmstrerror(ret));
		goto err;
	}

	__highmem_page_init(start, size);

	node->vaddr = start;
	node->size = size;
	node->release = __vmap_vmemmap_release;
	return node;
err:
	free(node);
	return NULL;
}

int liblinux_pal_vmemmap_is_exist(const void *start, unsigned long size)
{
	struct vmap_node *node = NULL;

	if (!lnxbase_is_vmemmap_populate_all(start, size)) {
		return -EINVAL;
	}

	node = __vmap_tree_find(&__vmemmap_tree, start);
	if (node == NULL) {
		return -ENODATA;
	}
	BUG_ON(start < node->vaddr);

	if ((ptr_to_ulong(start) + size) >
	    (ptr_to_ulong(node->vaddr) + node->size)) {
		return -ERANGE;
	}

	return 0;
}

int liblinux_pal_vmemmap_is_populated(const void *start, unsigned long size)
{
	if (!lnxbase_is_vmemmap_populate_all(start, size)) {
		return -EINVAL;
	}

	return liblinux_pal_vmemmap_is_exist(start, size);
}

int liblinux_pal_vmemmap_dump(char *info, unsigned long size)
{
	int ret;
	int ret_len = 0;
	struct rbt_node *node = NULL;
	struct vmap_node *cur = NULL;

	ret = sprintf_s(info, size, "%s%-16s %-16s\n",
			"vmemmap tree node info:\n", "vaddr", "size");
	if (ret < 0)
		return ret;
	ret_len += ret;

	RAW_MUTEX_GUARD(_, &__vmemmap_tree.lock);
	rbt_for_each_entry_safe(cur, node, &__vmemmap_tree.base, struct vmap_node, node) {
		ret = sprintf_s(info + ret_len, size - ret_len, "%#-16lx %#-16x\n",
				(unsigned long)cur->vaddr, cur->size);
		if (ret < 0)
			return ret;
		ret_len += ret;
	}

	return ret_len;
}
