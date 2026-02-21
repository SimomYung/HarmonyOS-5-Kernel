/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: LNXBASE alloc memory APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 06 22:40:08 2023
 */

#include <sys/mman.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>
#include <hongmeng/types.h>
#include <libsysif/sysmgr/api.h>
#include <libsysif/devhost/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/raw_rwlock_guard.h>

#include "../../../devhost_backend.h"

#include <devhost/log.h>
#include "internal.h"

struct lnxbase_mem_info {
	struct mem_raw_ranges *ranges;
	unsigned int ranges_num;
	void *vaddr;
};

/* find the first order less than size */
static int __get_right_order(int order, unsigned long size)
{
	int cur_order = order;

	while ((MEM_ORDER_TO_SIZE((unsigned int)cur_order) > size) && (cur_order > 0)) {
		cur_order--;
	}

	return cur_order;
}

static unsigned int __mm_alloc_pfn(unsigned long size, struct mem_raw_ranges *ranges)
{
	void *vaddr = NULL;
	unsigned int num = 0;
	unsigned long long paddr;
	int order = (int)SIZE_TO_MEM_ORDER(size);
	unsigned long size_remaining = size;
	struct liblinux_kmap_info kmap_info = lnxbase_get_kmap_info();

	BUG_ON(kmap_info.kmap_virt_start == 0);

	while (size_remaining > 0) {
		order = __get_right_order(order, size_remaining);
		vaddr = devhost_backend_alloc_page((unsigned int)order);
		if (vaddr != NULL) {
			paddr = kmap_info.phys_offset + (ptr_to_ulong(vaddr) -
							 kmap_info.kmap_virt_start);
			ranges[num].start = paddr >> PAGE_SHIFT;
			ranges[num].end = ranges[num].start + MEM_ORDER_TO_COUNT((unsigned int)order);
			size_remaining -= MEM_ORDER_TO_SIZE((unsigned int)order);
			num++;
		} else {
			if (order > 0) {
				order--;
			}
		}
	}

	return num;
}

static void __mm_free_pfn(struct mem_raw_ranges *ranges, unsigned int ranges_num)
{
	unsigned int i;
	void *vaddr = NULL;
	unsigned int order;
	unsigned long long paddr;
	struct liblinux_kmap_info kmap_info = lnxbase_get_kmap_info();

	BUG_ON(kmap_info.kmap_virt_start == 0);

	for (i = 0; i < ranges_num; i++) {
		paddr = ranges[i].start << PAGE_SHIFT;
		vaddr = ulong_to_ptr((kmap_info.kmap_virt_start +
				      (paddr - kmap_info.phys_offset)), void);
		order = SIZE_TO_MEM_ORDER((ranges[i].end -
					   ranges[i].start) << PAGE_SHIFT);
		devhost_backend_free_page(vaddr, order);
	}
}

static void *__mm_vmap_batch(struct mem_raw_ranges *ranges,
			     unsigned int ranges_num, unsigned long size)
{
	int ret;
	void *vaddr = NULL;
	unsigned long prot = PROT_READ | PROT_WRITE;
	unsigned int flag = LIBLINUX_PAL_REMAP_CACHE;

	vaddr = liblinux_pal_vm_prepare(0, size, prot, flag);
	if (vaddr == NULL) {
		return NULL;
	}

	ret = liblinux_pal_vm_mmap_batch(ranges, ranges_num,
					 ptr_to_ulong(vaddr), prot, flag);
	if (ret < 0) {
		(void)liblinux_pal_vm_unmap(vaddr);
		return NULL;
	}

	return vaddr;
}

void *lnxbase_mm_alloc(unsigned long size, void **handle)
{
	void *vaddr = NULL;
	unsigned long page_num;
	unsigned long real_size;
	unsigned int ranges_num;
	struct lnxbase_mem_info *mem_info = NULL;
	struct mem_raw_ranges *ranges = NULL;

	if ((size == 0) || (handle == NULL)) {
		return NULL;
	}

	real_size = PAGE_ALIGN_UP(size);
	page_num = real_size >> PAGE_SHIFT;
	ranges = malloc(sizeof(*ranges) * page_num);
	if (ranges == NULL) {
		return NULL;
	}

	ranges_num = __mm_alloc_pfn(real_size, ranges);
	vaddr = __mm_vmap_batch(ranges, ranges_num, real_size);
	if (vaddr == NULL) {
		goto err_vmap;
	}

	mem_info = malloc(sizeof(*mem_info));
	if (mem_info == NULL) {
		goto err_info;
	}

	mem_info->ranges = ranges;
	mem_info->ranges_num = ranges_num;
	mem_info->vaddr = vaddr;
	*handle = (void *)mem_info;

	return vaddr;

err_info:
	(void)liblinux_pal_vm_unmap(vaddr);
err_vmap:
	__mm_free_pfn(ranges, ranges_num);
	free(ranges);
	return NULL;
}

int lnxbase_mm_query(void *handle, void *ranges, unsigned int ranges_num)
{
	unsigned long size;
	struct lnxbase_mem_info *mem_info = NULL;

	if (handle == NULL || ranges == NULL || ranges_num == 0) {
		return E_HM_INVAL;
	}

	mem_info = (struct lnxbase_mem_info *)handle;
	BUG_ON(mem_info->vaddr == NULL);
	if (ranges_num < mem_info->ranges_num) {
		return E_HM_INVAL;
	}

	size = mem_info->ranges_num * sizeof(struct mem_raw_ranges);
	NOFAIL(memcpy_s(ranges, size, mem_info->ranges, size));

	return (int)mem_info->ranges_num;
}

void lnxbase_mm_free(void *handle)
{
	struct lnxbase_mem_info *mem_info = NULL;

	if (handle == NULL) {
		return;
	}

	mem_info = (struct lnxbase_mem_info *)handle;
	BUG_ON(mem_info->vaddr == NULL);
	(void)liblinux_pal_vm_unmap(mem_info->vaddr);
	__mm_free_pfn(mem_info->ranges, mem_info->ranges_num);
	free(mem_info->ranges);
	free(mem_info);
}

#define MAX_PFN_RANGES	(32UL)
struct pfn_range {
	unsigned long long start;
	unsigned long long end;
};

struct vmemmap_range {
	void *start;
	void *end;
};

static int __get_available_pfn(struct pfn_range *pfn_range,
			       unsigned long max_num,
			       unsigned long *pfn_range_num)
{
	int ret = E_HM_OK;
	struct __actvret_hmcall_mem_query_pfn_range func_ret;
	unsigned long mem_num = sizeof(struct pfn_range) / sizeof(unsigned long long);
	unsigned long length = mem_num * max_num;

	ret = actvcall_hmcall_mem_query_pfn_range(pfn_range, length, &func_ret);
	if ((ret != E_HM_OK)) {
		dh_error("get pfn ranges from sysmgr failed:%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	*pfn_range_num = func_ret.paddr_num / mem_num;
	return 0;
}

static void __pfn_to_vmemmap_range(struct pfn_range *pfn_range, void *vmemmap_start,
				  unsigned long page_size, unsigned long long pfn_offset,
				  struct vmemmap_range *vmemmap_range)
{
	void *start = NULL;
	void *end = NULL;
	void *start_align = NULL;
	void *end_align = NULL;

	if (vmemmap_range == NULL) {
		dh_error("vmemmap_range is NULL\n");
		return;
	}
	vmemmap_range->start = NULL;
	vmemmap_range->end = NULL;

	start = ulong_to_ptr((ptr_to_ulong(vmemmap_start) + ((pfn_range->start - pfn_offset) * page_size)), void);
	end = ulong_to_ptr((ptr_to_ulong(vmemmap_start) + ((pfn_range->end - pfn_offset) * page_size)), void);

	start_align = PAGE_ALIGN_DOWN(start);
	end_align = PAGE_ALIGN_UP(end);

	vmemmap_range->start = start_align;
	vmemmap_range->end = end_align;

	return;
}

static int __get_available_vmemmap(void *vmemmap_start, unsigned long page_size, struct vmemmap_range *vmemmap_ranges,
				   unsigned long max_num, unsigned long *vmemmap_range_num)
{
	int ret, i = 0;
	unsigned long j = 0;
	struct pfn_range *pfn_ranges = NULL;
	unsigned long pfn_range_num = 0;
	struct lnxbase_sysinfo info_ret = {0};
	struct vmemmap_range vmemmap_range = {NULL};

	if ((vmemmap_ranges == NULL) || (max_num == 0) || (vmemmap_range_num == NULL)) {
		return -EINVAL;
	}

	pfn_ranges = (struct pfn_range *)malloc(sizeof(struct pfn_range) * MAX_PFN_RANGES);
	if (pfn_ranges == NULL) {
		dh_error("get pfn_ranges error\n");
		return -ENOMEM;
	}
	/* get available pfn from sysmgr */
	ret = __get_available_pfn(pfn_ranges, MAX_PFN_RANGES, &pfn_range_num);
	if ((ret < 0) || (pfn_range_num == 0)) {
		dh_error("get available pfn pfn_range_num:%lu ret:%d\n", pfn_range_num, ret);
		goto out;
	}

	/* try to get memstart and total_ram from sysmgr */
	ret = lnxbase_sysinfo_get(&info_ret);
	if (ret < 0) {
		dh_error("query sysinfo failed, ret=%d\n", ret);
		goto out;
	}

	for (i = 0; i < (int)pfn_range_num; i++) {
		__pfn_to_vmemmap_range(&pfn_ranges[i], vmemmap_start, page_size,
				      (info_ret.memstart / PAGE_SIZE), &vmemmap_range);
		if ((vmemmap_range.start == NULL) || (vmemmap_range.end == NULL)) {
			dh_error("pfn:[0x%p-0x%p] convert failed\n",
				 ulong_to_ptr(pfn_ranges[i].start, void), ulong_to_ptr(pfn_ranges[i].end, void));
			ret = -EINVAL;
			goto out;
		}

		if ((j > 0) && (ptr_to_ulong(vmemmap_range.start) <= ptr_to_ulong(vmemmap_ranges[j - 1].end))) {
			vmemmap_ranges[j - 1].start = min(vmemmap_range.start, vmemmap_ranges[j - 1].start);
			vmemmap_ranges[j - 1].end = max(vmemmap_range.end, vmemmap_ranges[j - 1].end);
			continue;
		}

		if (j >= max_num) {
			dh_error("max_num:%lu is too small\n", max_num);
			ret = -EINVAL;
			goto out;
		}
		vmemmap_ranges[j].start = vmemmap_range.start;
		vmemmap_ranges[j].end = vmemmap_range.end;
		j++;
	}
	*vmemmap_range_num = j;

out:
	free(pfn_ranges);
	return ret;
}

static int __do_vmemmap_map(void *priv, struct vmemmap_init_ops *ops, void *addr,
			  unsigned long size, enum vmemmap_ops_type type)
{
	int ret = 0;
	int i = 0;
	struct vmemmap_ops *map_ops = ops->map_ops;
	struct vmemmap_ops *subcall = NULL;

	for (i = 0; i < (int)(ops->ops_num); i++) {
		subcall = map_ops++;
		if (!____IS((ops->ops_flags) & subcall->flags)) {
			continue;
		}

		switch (type) {
		case VMEMMAP_OPS_INIT:
			if (subcall->map_init) {
				ret = subcall->map_init(priv);
			}
		break;
		case VMEMMAP_OPS_MAP:
			if (subcall->map) {
				ret = subcall->map(priv, addr, size);
			}
		break;
		case VMEMMAP_OPS_DONE:
			if (subcall->map_done) {
				ret = subcall->map_done(priv);
			}
		break;
		default:
			ret = -EINVAL;
			dh_error("type(%u) is error\n", (unsigned int)type);
		break;
		}
	}

	return ret;
}

int lnxbase_vmemmap_init(void *vmemmap_start, unsigned long page_size, struct vmemmap_init_ops *ops, void *priv)
{
	int i, ret = 0;
	struct vmemmap_range *vmemmap_ranges = NULL;
	unsigned long vmemmap_range_num = 0;
	unsigned long size = 0;

	BUG_ON((PAGE_SIZE % page_size) != 0);

	vmemmap_ranges = (struct vmemmap_range *)malloc(sizeof(struct vmemmap_range) * MAX_PFN_RANGES);
	if (vmemmap_ranges == NULL) {
		dh_error("get vmemmap_ranges error\n");
		return -ENOMEM;
	}
	/* get available vmemmap from sysmgr */
	ret = __get_available_vmemmap(vmemmap_start, page_size, vmemmap_ranges, MAX_PFN_RANGES, &vmemmap_range_num);
	if ((ret < 0) || (vmemmap_range_num == 0)) {
		dh_error("get available vmemmap_range_num:%lu ret:%d\n", vmemmap_range_num, ret);
		goto out;
	}

	ret = __do_vmemmap_map(priv, ops, NULL, 0, VMEMMAP_OPS_INIT);
	if (ret < 0) {
		dh_error("do_vmemmap_map VMEMMAP_OPS_INIT ret:%d\n",ret);
		goto out;
	}

	/* map init for valid vmemmap */
	for (i = 0; i < (int)(vmemmap_range_num); i++) {
		size = ptr_to_ulong(vmemmap_ranges[i].end) - ptr_to_ulong(vmemmap_ranges[i].start);
		ret = __do_vmemmap_map(priv, ops, vmemmap_ranges[i].start, size, VMEMMAP_OPS_MAP);
		if (ret < 0) {
			dh_error("do_vmemmap_map VMEMMAP_OPS_MAP ret:%d\n",ret);
			goto out;
		}
	}

	ret = __do_vmemmap_map(priv, ops, NULL, 0, VMEMMAP_OPS_DONE);
	if (ret < 0) {
		dh_error("do_vmemmap_map VMEMMAP_OPS_DONE ret:%d\n",ret);
		goto out;
	}

out:
	free(vmemmap_ranges);
	return ret;
}

unsigned long long liblinux_pal_current_mm(void)
{
	struct __actvret_hmcall_mem_vspace_of_current func_ret;
	int ret;

	ret = actvcall_hmcall_mem_vspace_of_current(&func_ret);
	if (ret != E_HM_OK) {
		dh_error("query mm key failed %s\n", hmstrerror(ret));
		return 0;
	}
	return func_ret.key;
}

int liblinux_pal_vm_zap_ptes(unsigned long addr, unsigned long len, unsigned long long mm)
{
	int ret;

	ret = actvcall_hmcall_mem_proxy_munmap((void *)(uintptr_t)addr, len, mm, 1U);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

static lnxbase_pages_stat_p g_lnxbase_pages_stat[LNXBASE_PAGES_STAT_MAX] = {NULL};
DEFINE_RAW_RWLOCK(g_lnxbase_pages_stat_rwlock);
void lnxbase_stat_func_register(enum lnxbase_pages_stat_type type, lnxbase_pages_stat_p func)
{
	if (type >= LNXBASE_PAGES_STAT_MAX) {
		return;
	}
	RAW_RWLOCK_WR_GUARD(_, &g_lnxbase_pages_stat_rwlock);
	if (g_lnxbase_pages_stat[type] == NULL)
		g_lnxbase_pages_stat[type] = func;
	return;
}

static unsigned long lnxbase_pages_stat_get_nolock(enum lnxbase_pages_stat_type type)
{
	return (type < LNXBASE_PAGES_STAT_MAX) ? (g_lnxbase_pages_stat[type] ?
		g_lnxbase_pages_stat[type]() : 0) : 0;
}

void lnxbase_pages_stat(struct devhost_mem_info *mem_info)
{
	if (mem_info == NULL) {
		return;
	}

	RAW_RWLOCK_RD_GUARD(_, &g_lnxbase_pages_stat_rwlock);
	mem_info->discretepool_used = lnxbase_pages_stat_get_nolock(LNXBASE_DISCRETEPOOL_USED_PAGES);
	mem_info->discretepool_free = lnxbase_pages_stat_get_nolock(LNXBASE_DISCRETEPOOL_FREE_PAGES);
	mem_info->vmemmap_rsv_total = lnxbase_pages_stat_get_nolock(LNXBASE_VMEMMAP_RSV_PAGES);

	return;
}

void liblinux_pal_populate_size_per_page(void *rvaddr)
{
	int ret = E_HM_OK;
	struct pfn_range *pfn_ranges;
	unsigned long pfn_range_num = 0;
	
	pfn_ranges = (struct pfn_range *)malloc(sizeof(struct pfn_range) * MAX_PFN_RANGES);
	ret = __get_available_pfn(pfn_ranges, MAX_PFN_RANGES, &pfn_range_num);
	
	for (int i = 0; i < (int)pfn_range_num; i++) {
		if (pfn_ranges[i].end > pfn_ranges[i].start) {
			hm_mem_mlock((unsigned long*)rvaddr + pfn_ranges[i].start, (pfn_ranges[i].end - pfn_ranges[i].start) * sizeof(unsigned long));
		}
	}
	free(pfn_ranges);
}
