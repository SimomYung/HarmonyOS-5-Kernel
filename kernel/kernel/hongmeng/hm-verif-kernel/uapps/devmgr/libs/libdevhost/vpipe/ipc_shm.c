/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Convert the retval of hm ipc_shm interfaces to posix errno
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 10 22:24:03 2021
 */
#include <libdevhost/vpipe/ipc_shm.h>
#include <libhmsrv_sys/hm_ipc_shm.h>

#include <hongmeng/compiler.h>
#include <hongmeng/errno.h>

raw_static_assert(sizeof(struct hm_shmid_ds_s) == sizeof(struct shmid_ds),
		  hm_shmid_ds_s_compatible_with_shmid_ds);

int libdh_ipc_shmget(int key, unsigned long size, int flags)
{
	int err = hm_ipc_shmget(key, (size_t)size, flags);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

void *libdh_ipc_shmat(int shmid, const void *addr, int flags, int *errn)
{
	int err;
	void *result = hm_ipc_shmat(shmid, addr, flags, &err);
	if (errn != NULL) {
		*errn = (err < 0) ? -hmerrno2posix(err) : err;
	}
	return result;
}

int libdh_ipc_shmdt(const void *addr)
{
	int err = hm_ipc_shmdt(addr);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

int libdh_ipc_shmctl(int shmid, int cmd, struct hm_shmid_ds_s *buf)
{
	int err = hm_ipc_shmctl(shmid, cmd, (struct shmid_ds *)(void *)buf);
	return (err < 0) ? -hmerrno2posix(err) : err;
}
