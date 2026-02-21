/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: LNXBASE iofast alloc/free APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 28 20:48:03 2023
 */

#include <liblinux/pal.h>
#include <lnxbase/iofast.h>

#include <libmem/flags.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libkasan/kasan.h>
#include <libsysif/sysmgr/api.h>

#include <devhost/log.h>

static bool use_iofast = false;

static struct lnxbase_iofast_region lnxbase_iofast_regions[IOFAST_NR_TYPES];

bool lnxbase_use_iofast(void)
{
	return use_iofast;
}

int lnxbase_iofast_acquire_regions(struct lnxbase_iofast_region **regions)
{
	if (regions == NULL) {
		return E_HM_INVAL;
	}

	*regions = lnxbase_iofast_regions;
	return (int)IOFAST_NR_TYPES;
}

int lnxbase_iofast_declare(const char *range_name, struct liblinux_pool_base *pool_base)
{
	int ret;
	struct __actvret_hmcall_mem_iofast_declare func_ret;

	if (pool_base == NULL) {
		return E_HM_INVAL;
	}

	ret = actvcall_hmcall_mem_iofast_declare(range_name, pool_base->kmap_virt_start,
			pool_base->phys_offset, pool_base->kmap_virt_end - pool_base->kmap_virt_start,
			&func_ret);
	if (ret < 0) {
		dh_debug("iofast declare failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}

	pool_base->kmap_virt_start = func_ret.desc.vstart;
	pool_base->kmap_virt_end = func_ret.desc.vstart + func_ret.desc.length;
	pool_base->phys_offset = func_ret.desc.pstart;
	NOFAIL(memcpy_s(lnxbase_iofast_regions, sizeof(lnxbase_iofast_regions),
			func_ret.desc.regions, sizeof(lnxbase_iofast_regions)));

	use_iofast = true;

	return 0;
}

static unsigned int lnxbase_iofast_flags_from_gfp(unsigned int gfp_flags)
{
	unsigned int flags = 0U;

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_DMA)) {
		flags |= IOFAST_DMA;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_DETACH)) {
		flags |= IOFAST_DETACH;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_ZERO)) {
		flags |= IOFAST_ZERO;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_KSWAPD_SELF)) {
		flags |= IOFAST_NO_SLOW;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_NORETRY)) {
		flags |= IOFAST_NO_IO;
	}

	/* do not use reserved memory for GFP_NORETRY, GFP_NOWARN,
	 * GFP_NOWAIT, GFP_MAYFAIL, GFP_RETRY_MAYFAIL */
	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_NORETRY)) {
		flags |= IOFAST_NO_RSVCTG;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_NO_RESVMEM)) {
		flags |= IOFAST_NO_RSVCTG;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_NODOWNGRADE)) {
		flags |= IOFAST_NODOWNGRADE;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_MAYSLOW)) {
		flags |= IOFAST_MAYSLOW;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_PGTBL)) {
		flags |= IOFAST_PGTBL;
	}

	if (____IS(gfp_flags & LIBLINUX_PAL_GFP_PGD)) {
		flags |= IOFAST_PGD;
	}

	return flags;
}

/*
 * Alloc memory that meets the order size or more from sysmgr.
 *
 * @order: page order.
 * @pa_array: An array to store the requested pa information, the lower
 *	      12 bits store the order, and the other high bits are paddr.
 * @num: pa_array length.
 *
 * @flags: Control the specific behavior of allocation memory.
 *	LIBLINUX_PAL_GFP_DMA <--> IOFAST_DMA:
 *	  alloc memory from dma zone.
 *	LIBLINUX_PAL_GFP_DETACH <--> IOFAST_DETACH:
 *	  alloc pages not map to vregion.
 *	LIBLINUX_PAL_GFP_ZERO <--> IOFAST_ZERO:
 *	  alloc zero pages.
 *	LIBLINUX_PAL_GFP_NORETRY <--> IOFAST_NO_IO:
 *	  Do not retry when allocation fails.
 *	LIBLINUX_PAL_GFP_NORETRY <--> IOFAST_NO_RSVCTG:
 *	  Do not use reserved contiguous memory when page allocation fails.
 *	LIBLINUX_PAL_GFP_NODOWNGRADE <--> IOFAST_NODOWNGRADE:
 *	  The allocated pa must be the size of order.
 *
 * @pa_num: The number of paddr actually returned by sysmgr.
 *
 * `length = (num << (PAGE_SHIFT + order)) + num` is used to pass the
 * maximum allocable size to sysmgr.
 *
 * The implementation on the sysmgr side meets the following conditions:
 *   1. The sysmgr side needs to ensure that the size of the pa_array[0]
 *      allocated is order, otherwise the allocation will fail.
 *   2. If IOFAST_NODOWNGRADE is set, pat_array[1..num-1] must meet the
 *      order size, otherwise the allocation will be terminated and the
 *      number of pa actually allocated will be returned.
 *   3. If IOFAST_NODOWNGRADE is not set, pa_array[1..num-1] can be smaller
 *      than the order size.
 */
int lnxbase_iofast_alloc_pfn_v(unsigned int order, unsigned long long *pa_array,
			       unsigned long num, unsigned int flags, unsigned int *pa_num)
{
	int ret;
	unsigned int _flags = 0U;
	unsigned long length;
	struct __actvret_hmcall_mem_iofast_alloc_pfn_v func_ret;

	if (pa_array == NULL || pa_num == NULL) {
		return E_HM_INVAL;
	}

	_flags = lnxbase_iofast_flags_from_gfp(flags);

	/*
	 * length consists of two parts, the lower 12 bits store pa_num,
	 * and the higher bits stores page_num(real size).
	 */
	length = (num << (PAGE_SHIFT + order)) | num;
	ret = actvcall_hmcall_mem_iofast_alloc_pfn_v(order, _flags, pa_array,
						     length, &func_ret);
	if (ret < 0) {
		dh_debug("iofast alloc pfn failed, ret=%s order=%u\n",
			 hmstrerror(ret), order);
		return ret;
	}

	*pa_num = (unsigned int)func_ret.paddr_num;

	return 0;
}

int lnxbase_iofast_update_dev_reserve(unsigned long long size)
{
	int ret = 0;

	if (size == 0) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	ret = actvcall_hmcall_mem_iofast_update_dev_reserve_v(size);
	return -hmerrno2posix(ret);
}

int lnxbase_iofast_free_pfn_v(unsigned long long *pa_array, unsigned int pa_num)
{
	int ret;

	if (pa_array == NULL) {
		return E_HM_INVAL;
	}

	if (pa_num == 0) {
		return 0;
	}

	ret = actvcall_hmcall_mem_iofast_free_pfn_v(pa_array, pa_num);
	/*
	 * this ret is not normal return value,
	 * it means the count of memory which is failed to be freed.
	 */
	if (ret != 0) {
		dh_debug("iofast free pfn failed\n");
		return ret;
	}

	return 0;
}
