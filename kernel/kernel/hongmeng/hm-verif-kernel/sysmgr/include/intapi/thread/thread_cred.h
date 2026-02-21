/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Thread cred interface header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 06 14:35:21 2023
 */
#ifndef SYSMGR_THREAD_CRED_H
#define SYSMGR_THREAD_CRED_H

#include <intapi/process/process.h>
#include <intapi/thread/thread.h>
#include <libsec/cred.h>
#include <hmkernel/activation/local_area.h>
#include <lib/pac.h>

int thread_write_cred_miscdata(struct thread_s *new, struct process_s *proc);
struct base_cred *thread_cred_of(const struct thread_s *thread);
int thread_cred_copy(bool clone, struct thread_s *parent,
		     struct thread_s *new, struct process_s *proc);
void thread_cred_put(struct thread_s *thread);
int thread_cred_copy_from_process(struct thread_s *thread, struct base_cred *cred, struct process_s *proc);
#ifdef CONFIG_ENCAPS
void thread_encaps_copy_from_process(struct thread_s *thread, struct process_s *proc,
				   bool need_lock);
#endif
int thread_clone_cred(struct thread_s *new, struct thread_s *parent,
		      const struct thread_attr *attr,
		      struct process_s *process,
		      struct sysarg_thread_config *config);
struct base_cred *thread_cred_get(struct thread_s *thread);
void thread_cred_commit(struct thread_s *thread, struct base_cred *b_cred);
struct base_cred *thread_cred_dup(struct thread_s *thread);
int thread_cred_prepare(struct thread_s *thread, unsigned long clone_flags,
			struct base_cred **new_cred);

/* caller should guarantee thread and b_cred is not null */
static inline void thread_set_pre_actv_local_creds(const struct thread_s *thread)
{
	struct base_cred *b_cred = thread_cred_of(thread);
	size_t cred_addr = thread->local_creds_addr + offset_of(struct __cred_s, cred);
	size_t xact_cred_addr = thread->local_creds_addr + offset_of(struct __cred_s, xact_cred);
	size_t pac_cred_addr = thread->local_creds_addr + offset_of(struct __cred_s, pac_cred);
	size_t pac_xact_addr = thread->local_creds_addr + offset_of(struct __cred_s, pac_xact);
	*(cred_idx_t *)(cred_addr) = b_cred->idx;
	*(cred_idx_t *)(xact_cred_addr) = b_cred->idx;
	*(__u32 *)(pac_cred_addr) = __hm_pacdb(b_cred->idx, cred_addr);
	*(__u32 *)(pac_xact_addr) = __hm_pacdb(b_cred->idx, xact_cred_addr);
}

static inline void thread_assign_cred(struct thread_s *thread, struct base_cred *cred)
{
	thread->sec_thread.thread_cred.cred = cred;
	if ((thread->flags & THREAD_BIND_PRE_ACTV) != 0U) {
		/* init or update base cred idx in local area */
		threadmgr_ops_set_pre_actv_local_creds(thread);
	}
}
#endif
