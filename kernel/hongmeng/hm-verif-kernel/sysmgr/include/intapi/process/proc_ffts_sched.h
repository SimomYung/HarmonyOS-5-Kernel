/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Proc ffts sched
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 12 10:52:47 CST 2024
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROC_FFTS_SCHED_H
#define SYSMGR_INTAPI_PROCESS_PROC_FFTS_SCHED_H

#include "libhmsync/uspinlock.h"

#define FFTS_PROC_INIT 3
#define FFTS_WORKER_INIT 4
#define FFTS_SCHED_MAGIC 0XBF

#define FFRT_UNIFORM_ID_START 64
#define FFRT_UNIFORM_ID_NUM_MAX 960
#define FFRT_UNIFORM_ID_END (FFRT_UNIFORM_ID_START + FFRT_UNIFORM_ID_NUM_MAX - 1)

#define __FFTS_THD_SCHED_INFO_SYNC 621UL
#define FFTS_TIMEOUTUPPER_SEC 1000

struct sleeped_worker {
	int futex_val;
	struct uspinlock_s lock;
};

struct ffrt_tcb {
	uint16_t ffts_uid;
	uint16_t ffts_tid;
	int ffts_flag;
	unsigned long long cref;
};

struct ffrt_proc {
	uint16_t ffts_uid;
	uint16_t ffts_tid;
	int8_t qos_num;
	int8_t rsd;
	struct sleeped_worker *sworkers;
	unsigned long uaddr;
	unsigned long long kaddr;
};

struct ffrt_thd {
	uint16_t ffts_tid;
	int8_t qos;
	int8_t rsd;
};

struct sched_worker_s {
	pid_t pid;
	int qos;
};

struct ffts_sched_registry {
	int (*ffrt_put_uniform_id)(uint16_t uid);
};

int ffts_sched_proc_init(uint16_t ffts_tid, int8_t qos_num, unsigned long long futex_kaddr);
void ffts_sched_proc_exit(pid_t pid);
void ffts_sched_worker_wakeup(int tgid, int qos);
int ffrt_sched_uid_init(uint16_t uid, pid_t tgid);
int ffts_sched_ioctl(unsigned int cmd, unsigned long arg);
void ffts_sched_register_callback(struct ffts_sched_registry *fsreg);

#endif
