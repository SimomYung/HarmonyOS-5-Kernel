/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_HMBINDER_INFO_H__
#define __LIBDH_LINUX_HMBINDER_INFO_H__

#include "shm_trans.h"

struct binderfs_private_buffer {
	size_t size;
	char *data;
	struct shmem_info shm_info;
	uint64_t rref;
};

int hmbinder_info_read_remote(struct binderfs_private_buffer *pdata, int type);
void hmbinder_info_release(struct binderfs_private_buffer *pdata);
int hmbinder_info_acquire_fsmgr_rref(uint64_t *rref);
void hmbinder_info_release_fsmgr_rref(uint64_t rref);

#endif