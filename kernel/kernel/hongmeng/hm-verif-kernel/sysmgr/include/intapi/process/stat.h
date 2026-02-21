/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for process information statistics interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 16:32:36 2019
 */
#ifndef SYSMGR_INTAPI_PROCESS_STAT_H
#define SYSMGR_INTAPI_PROCESS_STAT_H

#include <unistd.h>
#include <intapi/thread/thread_ops.h>
#include <intapi/thread/thread.h>
#include <intapi/process/process.h>
#include <libbunch/bunch_ipc.h>
#include <libhmsrv_sys/hm_guard.h>
#include <hmkernel/sched_module.h>

struct cpu_energy_stat {
	struct __tcb_time time;
	unsigned long long energy;
};
int process_cpu_energy_stat(struct process_s *process,
		     struct cpu_energy_stat *stat);
void thread_cpu_energy_stat(struct thread_s *thread,
			   struct cpu_energy_stat *stat);
enum process_state process_get_state(struct process_s *process);

const char *get_process_state(enum process_state state);

#ifdef CONFIG_MODULE_SCHED_CLASS
int thread_sched_attr(const struct thread_s *thread,
		      struct sysarg_thread_stat *stat);
void thread_sched_attr_fill(unsigned int flag, struct sched_module_args_s sched_args,
			    struct sysarg_thread_stat *stat);
#endif
int process_stat_to_bunch(pid_t pid, unsigned int flags,
			  struct bunch *bunch);
int process_stat(pid_t pid, unsigned int flags, struct bunch_ipc_attr *attr);
int thread_rusage_stat(const struct thread_s *thread, struct sysarg_thread_stat *stat);

const char *get_tcb_state_from_stat(const struct sysarg_thread_stat *stat);
const char *get_tcb_block_reason_from_stat(const struct sysarg_thread_stat *stat);

int process_elf_load_info(pid_t pid, struct bunch_ipc_attr *attr);

int process_check_regs(struct process_s *process, unsigned int which, unsigned long addr, size_t size);
void hangtask_info_write(struct thread_s *thread,
			 bool in_monitor_list,
			 struct sysarg_thread_stat *stat,
			 struct guard_hangtask_stat *info);
bool is_rt_sched_class(int policy);

struct procmgr_stat_ops_s {
	void (*bunch_append_ex_info)(struct thread_s *thread, struct bunch *bunch);
};

#endif
