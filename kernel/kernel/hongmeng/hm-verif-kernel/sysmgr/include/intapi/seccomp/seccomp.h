/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition of security seccomp module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 13 16:55:31 2023
 */
#ifndef SYSMGR_SECURITY_SECCOMP_H
#define SYSMGR_SECURITY_SECCOMP_H

#include <hmkernel/errno.h>
#include <hmkernel/strict.h>
#include <libhmseccomp/seccomp.h>
#include <intapi/thread/thread.h>

struct cbpf_sock_fprog_s;

#ifdef CONFIG_SECCOMP
struct seccomp_metadata {
	__u64 filter_off;       /* Input: which filter */
	__u64 flags;             /* Output: filter's flags */
};

int seccomp_set_compat_mode_filter(struct thread_s *thread, unsigned int flags,
				   struct cbpf_sock_fprog_s *bpf_prog);
int seccomp_set_compat_mode_strict(struct thread_s *thread);
int seccomp_action_available(const uintptr_t ubuf);
long seccomp_get_filter(struct thread_s *curr_thread, struct thread_s *thread,
						unsigned long filter_off, void *data);
long seccomp_get_metadata(struct thread_s *curr_thread, struct thread_s *thread,
						unsigned long filter_off, void *data);

struct thread_sec_ops_s {
	void (*seccomp_cache_prepare)(struct thread_s *thread, struct seccomp_filter_sysmgr *sc_filter,
				      struct thread_s *thread_curr);
	int (*attach_filter)(struct thread_s *thread, unsigned int flags,
			     struct seccomp_filter_sysmgr *sc_filter, bool is_current);
};

extern const thread_sec_ops_t klt_sec_ops;

static inline const thread_sec_ops_t *sec_ops_of(const struct thread_s *thread)
{
	return thread_ops_of(thread)->sec_ops;
}

static inline void security_ops_seccomp_cache_prepare(struct thread_s *thread,
						 struct seccomp_filter_sysmgr *sc_filter,
						 struct thread_s *thread_curr)
{
	sec_ops_of(thread)->seccomp_cache_prepare(thread, sc_filter, thread_curr);
}

static inline int security_ops_attach_filter(struct thread_s *thread, unsigned int flags,
					     struct seccomp_filter_sysmgr *sc_filter, bool is_current)
{
	return sec_ops_of(thread)->attach_filter(thread, flags, sc_filter, is_current);
}
#else
static inline int seccomp_set_compat_mode_filter(struct thread_s *thread,
						 unsigned int flags,
						 struct cbpf_sock_fprog_s *bpf_prog)
{
	UNUSED(thread, flags, bpf_prog);
	return E_HM_NOSYS;
}
static inline int seccomp_set_compat_mode_strict(struct thread_s *thread)
{
	return E_HM_NOSYS;
}
static inline int seccomp_action_available(const uintptr_t ubuf)
{
	UNUSED(ubuf);
	return E_HM_OPNOTSUPP;
}
#endif /* CONFIG_SECCOMP */

#endif /* SYSMGR_SECURITY_SECCOMP_H */
