/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: interfaces of proc prctl
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 21 09:26:57 2023
 */

#ifndef SYSMGR_INTAPI_PROCESS_PROC_PRCTL_H
#define SYSMGR_INTAPI_PROCESS_PROC_PRCTL_H

struct proc_prctl_args {
	int op;
	unsigned long arg2;
	unsigned long arg3;
	unsigned long arg4;
	unsigned long arg5;
};

struct thread_s;
int procmgr_prctl(unsigned long credential, struct proc_prctl_args args);
int prctl_posix_caps(struct thread_s *thread, struct proc_prctl_args args);

#endif /* SYSMGR_INTAPI_PROCESS_PROC_PRCTL_H */
