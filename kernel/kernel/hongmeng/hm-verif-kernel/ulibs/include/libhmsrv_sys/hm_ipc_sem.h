/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 2 15:11:34 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SEM_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IPC_SEM_H

#include <sys/sem.h>
#include "hm_ipc_comm.h"
#include <libhmsync/raw_mutex.h>
#include <lib/dlist.h>

/* sem_undo_list controls shared access to the list of sem_undo structures
 * that may be shared among all a CLONE_SYSVSEM task group.
 */
struct sem_undo_list {
	struct raw_mutex	mutex;
	struct dlist_node	list_proc;
};

int hm_ipc_semget(key_t key, int nsems, int semflg);

int hm_ipc_semop(int semid, struct sembuf *sops, size_t nsops);

int hm_ipc_semctl(int semid, int semnum, int cmd, unsigned long arg);

#endif
