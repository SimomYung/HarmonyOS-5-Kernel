/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: intapi for kcov
 * Author: Huawei OS Kernel Lab
 * Create:  Thu Jan 4 20:57:52 2024
 */

#ifndef SYSMGR_KCOV_KCOV_H
#define SYSMGR_KCOV_KCOV_H

#include <hongmeng/errno.h>
struct vspace_s;
struct thread_s;

#ifdef CONFIG_KCOV
int kcov_sysmgr_mmap(struct vspace_s *vspace, int remote_fd, uintptr_t *rvaddr);
__u32 kcov_sysmgr_kcov_idx_state_of(struct thread_s *thread);
#else
static inline int kcov_sysmgr_mmap(struct vspace_s *vspace, int remote_fd, uintptr_t *rvaddr)
{
	UNUSED(vspace, remote_fd, rvaddr);
	return E_HM_NOSYS;
}

static inline __u32 kcov_sysmgr_kcov_idx_state_of(struct thread_s *thread)
{
	UNUSED(thread);
	return 0U;
}
#endif

#endif
