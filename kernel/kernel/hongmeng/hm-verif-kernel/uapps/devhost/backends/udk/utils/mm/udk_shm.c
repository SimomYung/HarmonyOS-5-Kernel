/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Shared memory interfaces of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:35:13 2019
 */
#include <udk/mm.h>

#include <limits.h>

#include <libmem/utils.h>
#include <libalgo/rbtree.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hmkernel/mm/pgtable.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>

#include <devhost/memory.h>

#include <udk/log.h>
#include <udk/cache.h>
#include <udk/iolib.h>
#include <udk/bitops.h>
#include <internal/init.h>

/* shared memory function */
int udk_alloc_shm_anon(size_t len, uint64_t flags, struct udk_shm_info *ret_info)
{
	uint32_t mflag = MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED;
	void *shm_vaddr = NULL;
	size_t shm_len = 0UL;
	int32_t shm_id = 0;
	int ret;
	int rc;

	if (ret_info == NULL) {
		return -EINVAL;
	}
	if ((flags & SHM_ALLOC_POPULATE) != 0ULL) {
		mflag |= MFLAG_MAP_POPULATE;
	}
	ret = hm_ashm_open_auth(0, 0U, len, SHM_O_CREAT | SHM_O_EXCL | SHM_O_RDWR,
				&shm_len);
	if (ret > 0) {
		shm_id = ret;
		shm_vaddr = hm_mem_mmap_posix(NULL, shm_len,
					      MPROT_READ | MPROT_WRITE,
					      mflag, shm_id, 0, &ret);
		if (ret != E_HM_OK) {
			rc = hm_ashm_close(shm_id);
			if (rc == E_HM_OK) {
				(void)hm_ashm_unlink(shm_id);
			}
		}
	}
	if (ret == E_HM_OK) {
		ret_info->id = shm_id;
		ret_info->len = shm_len;
		ret_info->vaddr = shm_vaddr;
	}

	return -hmerrno2posix(ret);
}

int udk_free_shm_anon(struct udk_shm_info *shm_info)
{
	int rc = E_HM_OK;

	if (shm_info == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = hm_mem_munmap(shm_info->vaddr, shm_info->len);
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_close(shm_info->id);
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_unlink(shm_info->id);
	}
	return -hmerrno2posix(rc);
}

int udk_grant_shm_to_caller(int32_t shmid, uint32_t prot, uint64_t *key)
{
	int rc = hm_ashm_grant_to_caller(shmid, prot, key);
	return -hmerrno2posix(rc);
}
