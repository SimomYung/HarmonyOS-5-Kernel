/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Feb 9 10:05:35 2020
 */
#include <sys/sem.h>
#include <errno.h>
#include <hmkernel/errno.h>
#include <libhmsrv_sys/hm_ipc_sem.h>
#include <libsysif/sysmgr/api.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libmem/utils.h>
#include <sys/syscall.h>
#include <hmasm/lsyscall.h>

#ifdef CONFIG_SYSVIPC
int hm_ipc_semget(key_t key, int nsems, int semflg)
{
	return lsyscall_syscall3(__NR_semget, key, nsems, semflg);
}

int hm_ipc_semop(int semid, struct sembuf *sops, size_t nsops)
{
	return lsyscall_syscall3(__NR_semop, semid, (long)sops, nsops);
}

int hm_ipc_semctl(int semid, int semnum, int cmd, unsigned long arg)
{
	return lsyscall_syscall4(__NR_semctl, semid, semnum, cmd, arg);
}
#else
int hm_ipc_semget(key_t key, int nsems, int semflg)
{
	UNUSED(key, nsems, semflg);

	return E_HM_NOSYS;
}

int hm_ipc_semop(int semid, struct sembuf *sops, size_t nsops)
{
	UNUSED(semid, sops, nsops);

	return E_HM_NOSYS;
}

int hm_ipc_semctl(int semid, int semnum, int cmd, unsigned long arg)
{
	UNUSED(semid, semnum, cmd, arg);

	return E_HM_NOSYS;
}
#endif
