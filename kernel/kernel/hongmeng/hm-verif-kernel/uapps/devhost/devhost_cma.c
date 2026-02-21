/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Exported APIs of Devhost Cma
 * Author: Huawei OS Kernel Lab
 * Create: Tues May 10 19:09:12 2022
 */
#include <devhost/cma.h>

#include <devhost_api.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libdevhost/mm.h>
#include <libmem/flags.h>
#include <libmem/utils.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>

#include <devhost/log.h>
#include "devhost.h"
#include "devhost_backend.h"

static int devhost_cma_alloc(unsigned int cma_id, size_t size, unsigned long long *paddr)
{
	struct __actvret_hmcall_mem_cma_alloc actv_ret;
	int ret;

	if (paddr == NULL || size == 0) {
		return E_HM_INVAL;
	}

	mem_zero_s(actv_ret);
	ret = actvcall_hmcall_mem_cma_alloc(cma_id, size, 0, &actv_ret);
	if (ret != E_HM_OK) {
		dh_error("cma alloc(cma_id=0x%x, size=%#zx) failed, err=%s\n",
					cma_id, size, hmstrerror(ret));
		return ret;
	}

	*paddr = actv_ret.paddr;
	return ret;
}

static int devhost_cma_free(unsigned int cma_id, unsigned long long paddr, size_t size)
{
	if (size == 0) {
		return E_HM_INVAL;
	}
	return actvcall_hmcall_mem_cma_free(cma_id, paddr, size);
}

static int devhost_cma_raw_map(const void *vaddr, unsigned long long paddr, unsigned long len,
				unsigned long prot, unsigned int flags)
{
	int ret;
	uint64_t hint;

	if (len == 0) {
		return E_HM_INVAL;
	}

	hint = prot_to_hint((uint32_t)prot, 0, 0);
	ret = actvcall_hmcall_mem_raw_map(vaddr, paddr, len, flags, hint);
	if (ret != E_HM_OK) {
		dh_error("devhost cma raw map(len=0x%lx, hint=%"PRIx64") failed, err=%s\n",
					len, hint, hmstrerror(ret));
	}

	return ret;
}

static int __devhost_cma_query_paddr(const struct devhost_cma *cma, unsigned long long *rpaddr)
{
	size_t size = cma->size;
	unsigned long long paddr = 0ULL;
	int ret;

	ret = devhost_cma_alloc(cma->cma_id, size, &paddr);
	if (ret < 0) {
		dh_error("devhost cma dyn init alloc(cma id=0x%x, size=%#zx) failed, err=%s\n",
						cma->cma_id, size, hmstrerror(ret));
		return ret;
	}

	*rpaddr = paddr;

	return devhost_cma_free(cma->cma_id, paddr, size);
}

int devhost_backend_cma_setup(struct devhost_cma *cma, void *vaddr)
{
	int ret;

	ret = hm_mem_munmap(vaddr, cma->size);
	if (ret < 0) {
		dh_error("devhost cma dyn init munmap failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = devhost_mem_resv_mem(vaddr, cma->size);
	if (ret < 0) {
		dh_error("devhost cma dyn init resv mem failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	cma->svaddr = ptr_to_ulong(vaddr);

	return E_HM_OK;
}

int devhost_backend_cma_st_init(size_t size,
				struct devhost_cma **rtcma)
{
	struct devhost_cma *cma = NULL;

	if (rtcma == NULL) {
		return E_HM_INVAL;
	}

	cma = (struct devhost_cma *)malloc(sizeof(struct devhost_cma));
	if (cma == NULL) {
		return -ENOMEM;
	}
	raw_mutex_init(&cma->mutex);
	cma->cma_id = 0;
	cma->size = size;

	*rtcma = cma;
	dh_info("cma st init(cma id=%#x,size=%#zx) done\n", cma->cma_id, size);
	return 0;
}
int devhost_backend_cma_dyn_init(size_t size,
				 struct devhost_cma **rtcma)
{
	struct __actvret_hmcall_mem_cma_dyn_init actv_ret;
	struct devhost_cma *cma = NULL;
	int ret;

	if (rtcma == NULL) {
		return E_HM_INVAL;
	}

	cma = (struct devhost_cma *)malloc(sizeof(struct devhost_cma));
	if (cma == NULL) {
		return -ENOMEM;
	}
	raw_mutex_init(&cma->mutex);
	mem_zero_s(actv_ret);
	ret = actvcall_hmcall_mem_cma_dyn_init(size, &actv_ret);
	if (ret != E_HM_OK) {
		dh_error("cma dyn init(size=%#zx) failed, err=%s\n", size, hmstrerror(ret));
		goto err_init_free;
	}
	cma->cma_id = actv_ret.cma_id;
	cma->size = size;

	ret = __devhost_cma_query_paddr(cma, &cma->spaddr);
	if (ret < 0) {
		dh_error("cma dyn init resv vaddr failed, err=%s\n", hmstrerror(ret));
		goto err_init_resv;
	}

	*rtcma = cma;
	dh_info("cma dyn init(cma id=%#x,size=%#zx) done\n", cma->cma_id, size);
	return 0;

err_init_resv:
	/*
	 * because CMA dyn destroy is required here,
	 * but sysmgr has not provided this sysif,
	 * so the BUG_ON, otherwise it will cause resource leakage
	 */
	BUG_ON(ret != 0);
err_init_free:
	free(cma);
	return ret;
}

int devhost_backend_cma_dyn_destroy(const struct devhost_cma * cma)
{
	/*
	 * because CMA dyn destroy is required here,
	 * but sysmgr has not provided this sysif,
	 */
	UNUSED(cma);
	return 0;
}

void *devhost_backend_cma_alloc(struct devhost_cma *cma,
				size_t size,
				uint32_t mprot,
				unsigned long long *rpaddr)
{
	unsigned long long paddr = 0;
	unsigned int flags = LIBDH_REMAP_CACHE;
	void *vaddr = NULL;
	int ret;

	if (cma == NULL || rpaddr == NULL || size == 0) {
		return NULL;
	}

	RAW_MUTEX_GUARD(_, &cma->mutex);
	ret = devhost_cma_alloc(cma->cma_id, size, &paddr);
	if (ret < 0) {
		dh_error("devhost backend cma alloc(cma id=0x%x, size=%#zx) failed, err=%s\n",
					cma->cma_id, size, hmstrerror(ret));
		goto err_alloc;
	}

	if (paddr < cma->spaddr || paddr >= cma->spaddr + cma->size) {
		dh_error("devhost backend cma alloc paddr overflow\n");
		goto err_reserve;
	}
	vaddr = devhost_cma_phys_to_virt(cma, paddr);
	ret = devhost_cma_raw_map(vaddr, paddr, size, mprot, flags);
	if (ret < 0) {
		dh_error("devhost backend cma raw map(mprot=0x%x, flags=0x%x) failed, err=%s\n",
								mprot, flags, hmstrerror(ret));
		goto err_reserve;
	}
	*rpaddr = paddr;

	return vaddr;

err_reserve:
	(void)devhost_cma_free(cma->cma_id, paddr, size);
err_alloc:
	return NULL;
}

int devhost_backend_cma_free(struct devhost_cma *cma, void *addr, size_t size)
{
	int ret;
	int ret_resv;

	if (cma == NULL || size == 0) {
		return -EINVAL;
	}
	if (ptr_to_ulong(addr) < cma->svaddr || ptr_to_ulong(addr) >= cma->svaddr + cma->size) {
		dh_error("devhost backend cma free overflow\n");
		return -EINVAL;
	}

	RAW_MUTEX_GUARD(_, &cma->mutex);
	ret = hm_mem_munmap(addr, size);
	if (ret < 0) {
		dh_error("devhost backend cma free munmap failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = devhost_cma_free(cma->cma_id,
			       devhost_cma_virt_to_phys(cma, addr),
			       size);
	ret_resv = devhost_mem_resv_mem(addr, size);
	if (ret < 0) {
		dh_error("devhost backend cma free failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}
	if (ret_resv < 0) {
		dh_error("devhost mem resv failed, ret=%s\n", hmstrerror(ret_resv));
		return ret_resv;
	}

	return 0;
}
