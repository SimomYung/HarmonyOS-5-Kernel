/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_SHM_TRANS_H__
#define __LIBDH_LINUX_SHM_TRANS_H__

struct shmem_info {
	int32_t shmid;
	size_t len;
	void *vaddr;
};

int shm_alloc_and_grant(struct shmem_info *shm, size_t size, uint32_t flags,
			uint64_t cref, uint64_t *shm_key);
void shm_release(struct shmem_info *shm);
void shm_clear(struct shmem_info *shm);

#endif
