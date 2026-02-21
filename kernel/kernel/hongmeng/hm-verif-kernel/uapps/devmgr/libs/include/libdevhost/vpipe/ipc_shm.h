/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Convert the retval of hm ipc_shm interfaces to posix errno
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 09 20:33:13 2021
 */
#ifndef LIBDEVHOST_VPIPE_IPC_SHM_H
#define LIBDEVHOST_VPIPE_IPC_SHM_H

/*
 * NOTE: Now the <libdevhost/vpipe/hm_ipc_shm.h> might be included
 * when compiling a liblinux module. To support this, cannot include
 * any unsupported-for-liblinux-module header files here.
 */
#include <libhmsrv_sys/hm_ipc_shm_common.h>

struct dh_shm_info {
	int id;
	unsigned long len;
	void *vaddr;
	void *paddr;
};

int libdh_ipc_shmget(int key, unsigned long size, int flags);
void *libdh_ipc_shmat(int shmid, const void *addr, int flags, int *errn);
int libdh_ipc_shmdt(const void *addr);
int libdh_ipc_shmctl(int shmid, int cmd, struct hm_shmid_ds_s *buf);

int libdh_alloc_static_shm(int id, unsigned int flags, struct dh_shm_info *ret_info);
int libdh_free_static_shm(int id, struct dh_shm_info *ret_info);

#endif /* !LIBDEVHOST_VPIPE_IPC_SHM_H */
