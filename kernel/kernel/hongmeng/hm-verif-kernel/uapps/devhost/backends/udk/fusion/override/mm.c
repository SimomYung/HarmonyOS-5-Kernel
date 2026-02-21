/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Override mm interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */

#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <intapi/mem/paf.h>
#include <intapi/mem/sysmem.h>

#include <devhost/backend.h>
#include <devhost/memory.h>

#include <udk/log.h>

/* Move following dma code to libdevhost in the future  */
static int __shm_mem_destroy(int shm_id)
{
	int ret = E_HM_OK;
	ret = hm_ashm_close(shm_id);
	if (ret < 0) {
		return ret;
	}
	return hm_ashm_unlink(shm_id);
}

static int __shm_mem_alloc(size_t size, uint64_t oflags,
			   unsigned long long *paddr, int *ret_id)
{
	int err = E_HM_OK;
	int shm_id;
	struct shm_node_info shm_info;

	BUG_ON(paddr == NULL);

	shm_id = hm_ashm_open_auth(0, 0U, size, oflags, NULL);
	if (shm_id < 0) {
		return shm_id;
	}

	err = hm_mem_query_shm_info(shm_id, &shm_info);
	if (err != E_HM_OK) {
		udk_error("query shm info failed, err=%s\n", hmstrerror(err));
		(void)__shm_mem_destroy(shm_id);
		return err;
	}

	*paddr = shm_info.paddr;
	if (ret_id != NULL) {
		*ret_id = shm_id;
	}

	return E_HM_OK;
}

void *devhost_backend_mem_alloc(size_t size, uint32_t mprot,
				unsigned long long *rpaddr,
				int *rshmid)
{
	int err = E_HM_OK;
	int shm_id = 0;
	off_t offset = 0;
	void *rvaddr = NULL;
	void *backend_addr = NULL;
	unsigned long long paddr = 0;
	uint32_t flags =  MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS |
			  MFLAG_MAP_POPULATE;
	uint64_t oflags = SHM_O_CREAT | SHM_O_EXCL | SHM_O_CELL | SHM_O_RDWR | SHM_O_NO_IO;

	if (mprot & MPROT_DMA) {
		oflags |= SHM_O_DMA;
	}

	err = __shm_mem_alloc(size, oflags, &paddr, &shm_id);
	if (err < 0) {
		return MAP_FAILED;
	}

	/* The map operation does not require MPROT_DMA */
	mprot &= (~MPROT_DMA);
	rvaddr = hm_mem_mmap(backend_addr, size, (uint32_t)mprot,
			     flags, shm_id, offset);
	if (rvaddr == MAP_FAILED) {
		(void)__shm_mem_destroy(shm_id);
		return MAP_FAILED;
	}

	if (rpaddr != NULL) {
		*rpaddr = paddr;
	}
	if (rshmid != NULL) {
		*rshmid = shm_id;
	}

	return rvaddr;
}

void devhost_backend_mem_free(int shmid, void *addr, size_t size)
{
	(void)hm_mem_munmap(addr, size);
	(void)__shm_mem_destroy(shmid);
}

void *malloc(size_t size)
{
	return sysmgr_internal_malloc(size, PAF_NO_IO);
}
