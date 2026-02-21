/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 06 22:00:34 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SHM_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SHM_H

#include <stddef.h>
#include <sys/shm.h>
#include "hm_ipc_comm.h"
#include "hm_ipc_shm_common.h"

int hm_ipc_shmget(int key, size_t size, int flags);

void *hm_ipc_shmat(int shmid, const void *addr, int flags, int *errn);

int hm_ipc_shmdt(const void *addr);

int hm_ipc_shmctl(int shmid, int cmd, struct shmid_ds *buf);

/*
 * populate the ipcshm memory and query the physical address of pages
 *
 * @shm_id: id of shared memory
 * @vaddr: the start virtual address from the result of hm_ipc_shmat
 * @offset: the offset of physical addresses
 * @dst: the buffer to whitch the physical address is writtrn
 * @size: the size(bytes) of dest buffer
 *
 * @ret: If the value is less than 0, an error is returned.
 *       Otherwise, page num is returned
 */
int hm_ipc_popluate_shm(int32_t shm_id, const void *vaddr,
			   unsigned long long offset,
			   unsigned long long *dst,
			   unsigned long size);

int hm_ipc_shm_setacl(int shmid, const char *attr, size_t size);

int hm_ipc_shm_getacl(int shmid, char *attr, size_t size);

int hm_ipc_shm_removeacl(int shmid);

#endif
