/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 20 11:12:39 2021
 */
#ifndef AARCH64_ULIBS_ASM_LIBHMSRV_SYS_HMIPC_SHM_H
#define AARCH64_ULIBS_ASM_LIBHMSRV_SYS_HMIPC_SHM_H

/* Compatible with struct ipc_perm */
struct hm_ipc_perm_s {
	int __ipc_perm_key; /* key_t */
	unsigned int uid; /* uid_t */
	unsigned int gid; /* gid_t */
	unsigned int cuid; /* uid_t */
	unsigned int cgid; /* gid_t */
	unsigned int mode; /* mode_t */
	int __ipc_perm_seq;
	long __pad1;
	long __pad2;
};

/* Compatible with struct shmid_ds */
struct hm_shmid_ds_s {
	struct hm_ipc_perm_s shm_perm; /* struct ipc_perm */
	unsigned long shm_segsz;
	long shm_atime; /* time_t */
	long shm_dtime; /* time_t */
	long shm_ctime; /* time_t */
	int shm_cpid; /* pid_t */
	int shm_lpid; /* pid_t */
	unsigned long shm_nattch;
	unsigned long __pad1;
	unsigned long __pad2;
};

#endif
