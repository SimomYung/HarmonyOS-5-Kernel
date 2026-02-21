/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: CMA pal APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 21 21:22:27 2022
 */
#include <liblinux/pal.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libmem/flags.h>
#include <libmem/utils.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

#include <devhost/cma.h>
#include <devhost/log.h>
#include <devhost/memory.h>
#include "../../../../devhost_backend.h"

int liblinux_pal_cma_area_init(unsigned long paddr, unsigned long vaddr, unsigned long size,
			       unsigned int cma_id, const char *name)
{
	int ret;
	struct __actvret_hmcall_mem_cma_id_fetch fetch_ret;
	struct __actvret_hmcall_mem_iomap_prepare ioprep_ret;

	if (name == NULL) {
		return -EINVAL;
	}

	mem_zero_s(fetch_ret);
	ret = actvcall_hmcall_mem_cma_id_fetch(paddr, (void *)name, strlen(name) + 1U,
					       &fetch_ret);
	if ((ret < 0) || (fetch_ret.cma_id != cma_id))
		dh_panic("liblinux fetch cma_id%u failed, fetch_id=%u ret=%s\n",
			 cma_id, fetch_ret.cma_id, hmstrerror(ret));

	mem_zero_s(ioprep_ret);
	dh_info("mem ioremap cma_id %u va 0x%p size %lu\n", fetch_ret.cma_id, ulong_to_ptr(vaddr, void), size);
	ret = actvcall_hmcall_mem_iomap_prepare((void *)vaddr, size, UINT_MAX,
						(unsigned int)(PROT_READ | PROT_WRITE),
						MFLAG_MAP_FIXED | MFLAG_MAP_SHARED,
						&ioprep_ret);
	if (ret < 0) {
		dh_error("liblinux iomap for cma failed, ret=%s\n", hmstrerror(ret));
		(void)liblinux_pal_reserve_range_name((void *)vaddr, size, PROT_NONE,
						      "[liblinux/KMAP-RSVD]");
		return -hmerrno2posix(ret);
	}

	return 0;
}

int liblinux_pal_cma_alloc(unsigned int cma_id, unsigned long size, unsigned int align_order,
			   unsigned long *paddr)
{
	int ret;
	void *vaddr;
	uint64_t hint;
	unsigned long prot = PROT_READ | PROT_WRITE;
	struct __actvret_hmcall_mem_cma_alloc actv_ret;

	mem_zero_s(actv_ret);
	ret = actvcall_hmcall_mem_cma_alloc(cma_id, size, align_order, &actv_ret);
	if (ret < 0) {
		dh_error("liblinux cma alloc failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	*paddr = actv_ret.paddr;
	vaddr = devhost_backend_prepare_map(*paddr, size);
	if (vaddr == NULL) {
		dh_error("liblinux cma prepare map failed\n");
		(void)actvcall_hmcall_mem_cma_free(cma_id, *paddr, size);
		return -EINVAL;
	}

	hint = prot_to_hint((uint32_t)prot, 0, 0);
	ret = actvcall_hmcall_mem_iomap_populate(vaddr, size, *paddr, hint);
	if (ret < 0) {
		dh_error("liblinux mapping on cma failed, ret=%s\n", hmstrerror(ret));
		devhost_backend_prepare_unmap(vaddr, size);
		(void)actvcall_hmcall_mem_cma_free(cma_id, *paddr, size);
		return -hmerrno2posix(ret);
	}

	return 0;
}

int liblinux_pal_cma_release(unsigned int cma_id, unsigned long vaddr,
			     unsigned long paddr, unsigned long size)
{
	int ret;

	ret = actvcall_hmcall_mem_iomap_unpopulate((void *)vaddr, size);
	if (ret < 0) {
		dh_error("liblinux unmapping cma failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	devhost_backend_prepare_unmap((void *)vaddr, size);

	ret = actvcall_hmcall_mem_cma_free(cma_id, paddr, size);
	if (ret < 0) {
		dh_error("liblinux cma free failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}
