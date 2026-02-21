/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 20 11:20:44 2021
 */
#ifndef AARCH64_ULIBS_ASM_LIBHMSRV_SYS_HMIPC_SHM_H
#define AARCH64_ULIBS_ASM_LIBHMSRV_SYS_HMIPC_SHM_H

#include <features.h>

/* Compatible with struct ipc_perm */
struct hm_ipc_perm_s {
	int __ipc_perm_key;
	unsigned int uid;
	unsigned int gid;
	unsigned int cuid;
	unsigned int cgid;
	unsigned int mode;
	int __ipc_perm_seq;
	long __pad1;
	long __pad2;
};

/* Compatible with struct shmid_ds */
struct hm_shmid_ds_s {
	struct hm_ipc_perm_s shm_perm;
	unsigned long shm_segsz;
	unsigned long __shm_atime_lo;
	unsigned long __shm_atime_hi;
	unsigned long __shm_dtime_lo;
	unsigned long __shm_dtime_hi;
	unsigned long __shm_ctime_lo;
	unsigned long __shm_ctime_hi;
	int shm_cpid;
	int shm_lpid;
	unsigned long shm_nattch;
	unsigned long __pad1;
	unsigned long __pad2;
	unsigned long __pad3;
#ifdef __MUSL_V120_TIME64
	long long shm_atime;
	long long shm_dtime;
	long long shm_ctime;
#else
	long shm_atime;
	long shm_dtime;
	long shm_ctime;
#endif
};

#endif
