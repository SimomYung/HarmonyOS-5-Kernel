/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Callback spincall for spinhdlr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 20 13:10:24 2023
 */

#ifndef INTAPI_CTRLMEM_SPINHDLR_H
#define INTAPI_CTRLMEM_SPINHDLR_H

#include <stddef.h>
#include <stdbool.h>
#include <hmasm/ucontext.h>
#include <hmkernel/ctrlmem/sysconf.h>
#include <hmkernel/compiler.h>
#include <hmkernel/capability.h>

extern bool
ctrlmem_sysconf_spinhdlr_ptlwfe_check(struct __ctrlmem_sysconf_spinhdlr_ptlwfe_state_s *state, long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_commit(const struct __ctrlmem_sysconf_spinhdlr_ptlwfe_state_s *state,
				       bool cancel, long ret, void *sp, void *fallback_entry);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_exphdlr(void);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall(long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall_proc_getppid(long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall_fcntl(long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall_mem_mprotect_lsyscall(long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall_mem_madvise_lsyscall(long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall_close(long ret);

extern void
ctrlmem_sysconf_spinhdlr_ptlwfe_lsyscall_dup(long ret);

extern void ctrlmem_sysconf_spinhdlr_dfrwfe(void *fallback_sp, void *fallback_entry);
extern void ctrlmem_sysconf_spinhdlr_fallback(void);

extern int
ctrlmem_sysconf_spinhdlr_sched_trans(__u64 src_cref, __u64 dst_cref, __u32 __restore);

extern int
ctrlmem_sysconf_spinhdlr_trace_shm_set_addr(uintptr_t addr);

extern int
ctrlmem_sysconf_spinhdlr_trace_shm_set_futex_addr(uintptr_t addr);

extern int
ctrlmem_sysconf_spinhdlr_trace_shm_set_snapshot_addr(uintptr_t addr);

extern int
ctrlmem_sysconf_spinhdlr_trace_shm_set_saved_taskinfo_addr(uintptr_t addr);

extern void
ctrlmem_sysconf_spinhdlr_set_uentry(void);

#ifdef CONFIG_KOBJ_THREAD_SPINCALL
void ctrlmem_sysconf_spinhdlr_resume_thread(void *kobj_thread, cref_t tcb_cref);
void ctrlmem_sysconf_spinhdlr_wake_thread(void *kobj_thread, cref_t tcb_cref);
extern int
ctrlmem_sysconf_spinhdlr_thread_sched_setattr(void *kobj_thread, cref_t tcb_cref,
					      void *args, unsigned long args_size);
struct sysarg_thread_stat;
extern int
ctrlmem_sysconf_spinhdlr_thread_stat(void *kobj_thread, cref_t tcb_cref,
				     unsigned long flag,
				     struct sysarg_thread_stat *stat);
extern int
ctrlmem_sysconf_spinhdlr_thread_lt_set_conf(void *args, unsigned long args_size);
#endif

extern int
ctrlmem_sysconf_spinhdlr_secmem_change_klinear(__u64 pstart, __u64 pend,
						__u64 pgt_pa, unsigned long modelid);

extern __u64 ctrlmem_sysconf_spinhdlr_fdtable_lsyscall_fd(const struct arch_uctx *uctx);

struct sysarg_fdtable_fd_install;
extern int
ctrlmem_sysconf_spinhdlr_fdtable_install_fd(__u32 cnode_idx, const struct sysarg_fdtable_fd_install *fd_install_args);

struct sysarg_fdtable_fd_stat;
extern int
ctrlmem_sysconf_spinhdlr_fdtable_close_fd(__u32 cnode_idx, unsigned int fd, int fd_type,
					  struct sysarg_fdtable_fd_stat *fd_stat);

struct sysarg_fdtable_fd_config;
extern int
ctrlmem_sysconf_spinhdlr_fdtable_alloc_fd(__u32 cnode_idx, struct sysarg_fdtable_fd_config *fd_args);

struct sysarg_fdtable_fd_dup_req;
struct sysarg_fdtable_fd_dup_res;
extern int
ctrlmem_sysconf_spinhdlr_fdtable_dup_fd(__u32 cnode_idx, const struct sysarg_fdtable_fd_dup_req *dup_req,
					struct sysarg_fdtable_fd_dup_res *dup_res);

extern int
ctrlmem_sysconf_spinhdlr_fdtable_free_fd(__u32 cnode_idx, unsigned int fd, int fd_type, __u64 *pfn);
extern int
ctrlmem_sysconf_spinhdlr_fdtable_alloc_install_fd(__u32 cnode_idx, struct sysarg_fdtable_fd_install *fd_args);

extern void
ctrlmem_sysconf_spinhdlr_set_caller_compat_state(bool fake_compat);
#endif
