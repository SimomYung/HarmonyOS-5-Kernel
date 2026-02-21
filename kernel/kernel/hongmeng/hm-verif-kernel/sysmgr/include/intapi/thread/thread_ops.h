/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: define thread ops
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 27 16:10:35 2023
 */

#ifndef H_SYSMGR_INCLUDE_THREAD_OPS_H
#define H_SYSMGR_INCLUDE_THREAD_OPS_H

typedef struct thread_core_ops_s thread_core_ops_t;
typedef struct thread_sig_ops_s thread_sig_ops_t;

typedef struct thread_sched_ops_s thread_sched_ops_t;
#ifdef CONFIG_THREAD_FINE_LOAD
typedef struct thread_qos_ops_s thread_qos_ops_t;
#endif
typedef struct thread_soft_ex_ops_s thread_soft_ex_ops_t;
#ifdef CONFIG_PROCESS_COREDUMP
typedef struct thread_coredump_ops_s thread_coredump_ops_t;
#endif
typedef struct thread_procdump_ops_s thread_procdump_ops_t;
#ifdef CONFIG_RESMGR
typedef struct thread_resmgr_ops_s thread_resmgr_ops_t;
#endif
typedef struct thread_procfs_ops_s thread_procfs_ops_t;
#ifdef CONFIG_SYSMGR_PERF
typedef struct thread_perf_ops_s thread_perf_ops_t;
#endif
typedef struct thread_timemgr_ops_s thread_timemgr_ops_t;

#ifdef CONFIG_SECCOMP
typedef struct thread_sec_ops_s thread_sec_ops_t;
#endif
typedef struct thread_procmgr_ops_s thread_procmgr_ops_t;

typedef struct thread_ops_s {
	const thread_core_ops_t *core_ops;
	const thread_sig_ops_t *sig_ops;
	const thread_sched_ops_t *sched_ops;
#ifdef CONFIG_THREAD_FINE_LOAD
	const thread_qos_ops_t *qos_ops;
#endif
	const thread_soft_ex_ops_t *soft_ex_ops;
#ifdef CONFIG_PROCESS_COREDUMP
	const thread_coredump_ops_t *coredump_ops;
#endif
	const thread_procdump_ops_t *procdump_ops;
#ifdef CONFIG_RESMGR
	const thread_resmgr_ops_t *resmgr_ops;
#endif
	const thread_procfs_ops_t *procfs_ops;
#ifdef CONFIG_SYSMGR_PERF
	const thread_perf_ops_t *perf_ops;
#endif
	const thread_timemgr_ops_t *timemgr_ops;
#ifdef CONFIG_SECCOMP
	const thread_sec_ops_t *sec_ops;
#endif
	const thread_procmgr_ops_t *procmgr_ops;
} thread_ops_t;
#endif
