/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define task stats
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 14 17:58:10 2019
 */
#ifndef ULIBS_LIBPROCFS_TASKSTAT_H
#define ULIBS_LIBPROCFS_TASKSTAT_H

#include <libhmucap/ucap.h>
#include <hmkernel/const.h>

enum task_flag {
	LIBPROCFS_FLAG_THREAD,
	LIBPROCFS_FLAG_PROCESS,
	LIBPROCFS_FLAG_MEM,
	LIBPROCFS_FLAG_TIME,
	LIBPROCFS_FLAG_REFRESH,
	LIBPROCFS_FLAG_HGUARD, /* thread scanned by hguard  */
};

#define task_flag_mask(n)	(__U(1) << (n))
#define TASK_STATE_LEN		16
#define TASK_COMM_LEN 		16
#define TASK_BLOCK_LEN		8
#define TASK_POLICY_LEN		16
#define TASK_NICE_LEN		8
#define TASK_PRIO_LEN		8

struct task_stats {
	pid_t tgid;
	pid_t pid;
	pid_t tid;
	pid_t ppid;

	uint32_t uid;
	uint32_t pgid;
	uint32_t rq;
	uint64_t cref;
	uint64_t sched_cnt;
	uint64_t total_time;
	uint64_t run_time;
	uint64_t ready_time;

	unsigned long vsize;
	unsigned long rwsize;
	unsigned long rss;
	unsigned long share;
	unsigned long stack;

	char comm[TASK_COMM_LEN];
	char state[TASK_STATE_LEN];
	char block[TASK_BLOCK_LEN];
	char policy[TASK_POLICY_LEN];
	char nice[TASK_NICE_LEN];
	char rt_prio[TASK_PRIO_LEN];
	char priority[TASK_PRIO_LEN];
	uint64_t prev_tcb_cref;
	uint64_t preempt_tcb_cref;
};

struct cpu_stat_entry {
	uint64_t run_time;
	uint64_t idle_time;
	bool online;
};

struct mem_stats {
	unsigned long total;
	unsigned long free;
	unsigned long share; /* size of unused share memory */
	unsigned long buff; /* size of used buff */
	unsigned long cache; /* size of used cache */
	unsigned long kobj;
	unsigned long cp_bitmap;
	unsigned long free_cache;
};

struct hm_procfs {
	void *task_seq;
	uint32_t cpu_nr;
	struct cpu_stat_entry *cpu_stats;
};

struct hm_procfs *hm_procfs_open(void);
void hm_procfs_close(struct hm_procfs *procfs);
int hm_procfs_register_rref(struct hm_procfs *procfs, rref_t rref);
int hm_procfs_inflate(struct hm_procfs *procfs);
int hm_procfs_taskstat(struct hm_procfs *procfs, struct task_stats *stats,
		       unsigned int flag);
int hm_procfs_cpustat(struct hm_procfs *procfs);
int hm_procfs_memstat(struct mem_stats *pmem);
int hm_server_memstat(rref_t server, struct mem_stats *pmem);
int hm_procstat_single(pid_t pid, unsigned int proc_flag, struct task_stats *stats);

#endif
