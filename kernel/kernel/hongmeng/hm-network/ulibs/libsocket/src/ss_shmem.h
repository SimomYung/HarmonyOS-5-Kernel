/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file of shmem alloc and free
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#ifndef __LIBSOCKET_SS_SHMEM_H_
#define __LIBSOCKET_SS_SHMEM_H_

#include <libhmsrv_sys/hm_mem.h>

struct ss_shmem_info {
	int32_t id;
	uint64_t server_id;
	size_t len;
	void *vaddr;
};

int ss_shmem_alloc(struct ss_shmem_info *shm);
void ss_shmem_free(struct ss_shmem_info *shm);

#endif
