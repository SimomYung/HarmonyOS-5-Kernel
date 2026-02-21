/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation for shmem alloc and free
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libmem/flags.h>
#include <libmem/utils.h>

#include "ux_event.h"
#include "ss_shmem.h"
#include "ss_channel.h"

static int free_ss_shmem(int32_t shm_id, size_t len, void *vaddr)
{
	int rc = E_HM_OK;

	if (vaddr == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (vaddr != ulong_to_ptr(MAP_FAILED, void)) {
			rc = hm_mem_munmap(vaddr, len);
			if (rc < 0) {
				hm_debug("munmap shm failed, err=%s\n", hmstrerror(rc));
			}
		}
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_close(shm_id);
		if (rc < 0) {
			hm_debug("close shm failed, err=%s\n", hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_unlink(shm_id);
		if (rc < 0) {
			hm_debug("unlink shm failed, err=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}

static int __ss_shmem_alloc(struct ss_shmem_info *shm, size_t size)
{
	void *shm_vaddr = NULL;
	int32_t shm_id;
	size_t shm_len;
	uint64_t shm_key;
	rref_t rref;
	int rc, err;

	if (shm == NULL) {
		hm_error("invalid para: shm is NULL\n");
		return E_HM_INVAL;
	}
	rc = hm_ashm_open_auth(0, 0, size,
				  SHM_O_CREAT | SHM_O_EXCL |
				  SHM_O_RDWR, &shm_len);
	if (rc < 0) {
		hm_debug("hm_ashm_open_auth failed, err=%s\n", hmstrerror(rc));
		return rc;
	}
	shm_id = rc;
	shm_vaddr = hm_mem_mmap_posix(NULL, shm_len, MPROT_READ | MPROT_WRITE,
				      MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
				      shm_id, 0, &rc);
	if (rc != E_HM_OK) {
		hm_error("hm_mem_mmap posix failed, err = %s\n", hmstrerror(rc));
		(void)free_ss_shmem(shm_id, shm_len, shm_vaddr);
		return rc;
	}

	rc = hm_net_ap_rref(&rref);
	if (rc < 0) {
		(void)free_ss_shmem(shm_id, shm_len, shm_vaddr);
		return rc;
	}
	xref_t xref = { .rref = rref };
	rc = hm_ashm_grant(shm_id, SHM_GRANT_RDWR, xref, &shm_key);
	if (rc < 0) {
		hm_error("grant shm to hmnet failed, shm id=%d, err=%s\n",
			 shm_id, hmstrerror(rc));
		err = free_ss_shmem(shm_id, shm_len, shm_vaddr);
		if (err != E_HM_OK) {
			hm_warn("free shm failed\n");
		}
		goto out;
	}
	shm->id = shm_id;
	shm->server_id = shm_key;
	shm->len = shm_len;
	shm->vaddr = shm_vaddr;
	rc = 0;

out:
	hm_net_put_rref(&rref);
	return rc;
}

int ss_shmem_alloc(struct ss_shmem_info *shm)
{
	return __ss_shmem_alloc(shm, HM_NET_SHMEM_SIZE);
}

void ss_shmem_free(struct ss_shmem_info *shm)
{
	int rc;

	/*
	 * If fork-socket fails before reallocating socket shm,
	 * the shm of the socket which comes from parent has been munmap,
	 * it is forbidden to release it again
	 */
	if (shm->vaddr == NULL) {
		return;
	}

	rc = free_ss_shmem(shm->id, shm->len, shm->vaddr);
	if (rc != E_HM_OK) {
		hm_warn("free ss shmem failed, err=%s, shm_id=%d\n",
			hmstrerror(rc), shm->id);
	}
}
