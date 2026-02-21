// SPDX-License-Identifier: GPL-2.0
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_path.h>
#include <libmem/umem.h>
#include <vfs.h>
#include "shm_trans.h"

static int shm_alloc(struct shmem_info *shm, size_t size)
{
	int shmid = 0;
	char *addr = NULL;
	int err = E_HM_OK;
	uint64_t flags = SHM_O_CREAT | SHM_O_EXCL | SHM_O_RDWR | SHM_O_POPULATE;

	shmid = hm_ashm_open_auth(0, 0U, size, flags, NULL);
	if (shmid < 0) {
		hm_error("open shm failed, err = %s\n", hmstrerror(shmid));
		err = shmid;
	}

	if (err == E_HM_OK) {
		shm->shmid = shmid;
		shm->len = size;
		addr = hm_mem_mmap(NULL, size, PROT_READ | PROT_WRITE,
				   MAP_SHARED | MAP_ANONYMOUS, shmid, 0);
		if (addr == MAP_FAILED) {
			(void)hm_ashm_close(shmid);
			(void)hm_ashm_unlink(shmid);
			hm_error("mmap shm failed\n");
			err = E_HM_RESFAULT;
		}
	}

	if (err == E_HM_OK) {
		shm->vaddr = addr;
	}

	return err;
}

void shm_clear(struct shmem_info *shm)
{
	shm->vaddr = NULL;
	shm->shmid = 0;
	shm->len = 0U;
}

int shm_alloc_and_grant(struct shmem_info *shm, size_t size, uint32_t flags,
			uint64_t rref, uint64_t *shm_key)
{
	int err = E_HM_OK;
	xref_t xref;

	if (shm == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = shm_alloc(shm, size);
		if (err != E_HM_OK)
			shm_clear(shm);
	}

	if (err == E_HM_OK) {
		xref.rref = (rref_t)rref;
		err = hm_ashm_grant(shm->shmid, flags, xref, shm_key);
		if (err != E_HM_OK) {
			shm_release(shm);
			shm_clear(shm);
			hm_error("shm grant failed, err = %s\n", hmstrerror(err));
		}
	}

	return err;
}

void shm_release(struct shmem_info *shm)
{
	int err;
	int shmid;

	if (shm != NULL) {
		shmid = shm->shmid;
		if (shm->vaddr != NULL) {
			err = hm_mem_munmap(shm->vaddr, shm->len);
			if (err != 0) {
				hm_warn("munmap shm failed, err = %s\n", hmstrerror(err));
			}
		}
		if (shmid != 0) {
			err = hm_ashm_close(shmid);
			if (err < 0) {
				hm_warn("close shm failed, shmid = %d, err = %s\n", shmid, hmstrerror(err));
			}
			err = hm_ashm_unlink(shmid);
			if (err < 0) {
				hm_warn("unlink shm failed, shmid = %d, err = %s\n", shmid, hmstrerror(err));
			}			
		}
	}
}
