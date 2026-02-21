// SPDX-License-Identifier: GPL-2.0
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_path.h>
#include <libmem/umem.h>
#include <vfs.h>
#include "hmbinder_info.h"

#define DEFAULT_PAGE_NUM (1U)

int hmbinder_info_acquire_fsmgr_rref(uint64_t *rref)
{
	int err = E_HM_OK;

	if (rref == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = hm_path_acquire(FSMGR_PATH_ACTV, rref);
		if (err != E_HM_OK) {
			hm_error("acquire fsmgr rref failed, err = %s\n", hmstrerror(err));
		}
	}

	return -hmerrno2posix(err);
}

void hmbinder_info_release_fsmgr_rref(uint64_t rref)
{
	(void)hm_path_release(FSMGR_PATH_ACTV, (rref_t)rref);
}

int hmbinder_info_read_remote(struct binderfs_private_buffer *pdata, int type, int pid)
{
	int rc = E_HM_OK;
	unsigned int npage = DEFAULT_PAGE_NUM;
	struct shmem_info *shm;
	uint64_t shm_key;
	rref_t rref;

	if (pdata == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rref = (rref_t)pdata->rref;
		shm = &pdata->shm_info;
		do {
			rc = shm_alloc_and_grant(shm, npage << PAGE_SHIFT, SHM_GRANT_RDWR, rref, &shm_key);
			if (rc < 0) {
				hm_error("shm alloc and grant failed, rc = %s\n", hmstrerror(rc));
				break;
			}

			rc = vfs_binder_read_debugfs_info(shm_key, shm->len, type, pid);
			if (rc == E_HM_FBIG) {
				shm_release(shm);
				npage = npage << 1;
			} else if (rc < 0) {
				shm_release(shm);
				shm_clear(shm);
				break;
			} else {
				pdata->data = shm->vaddr;
				pdata->size = (size_t)(unsigned int)rc;
			}
		} while (rc == E_HM_FBIG);
	}

	return rc < 0 ? -hmerrno2posix(rc) : 0;
}

void hmbinder_info_release(struct binderfs_private_buffer *pdata)
{
	if (pdata != NULL) {
		shm_release(&pdata->shm_info);
	}
}
