/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmperf ng intapi in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 18 12:20:14 2023
 */
#ifndef SYSMGR_INTAPI_INSPECTOR_HMPERF_NG_PERF_H
#define SYSMGR_INTAPI_INSPECTOR_HMPERF_NG_PERF_H

#include <intapi/mem/vspace.h>

enum perf_event_paranoid_level {
	PERF_EVENT_NOT_PARANOID = -1,
	PERF_EVENT_PARANOID_TRACEPOINT = 0,
	PERF_EVENT_PARANOID_CPU = 1,
	PERF_EVENT_PARANOID_KERNEL = 2,
};

struct vspace_s;
struct perf_event;

#ifdef CONFIG_HMPERF_NG
int perf_event_get_paranoid(void);
int perf_event_set_paranoid(int p);
#else
static inline int perf_event_get_paranoid(void)
{
	return 0;
}
static inline int perf_event_set_paranoid(int p)
{
	UNUSED(p);
	return 0;
}
#endif

#ifdef CONFIG_HMPERF_NG
unsigned long get_perf_event_mlock_kb(void);
void set_perf_event_mlock_kb(unsigned long mlock_kb);

ssize_t perf_event_read(struct perf_event *event, void *buf, size_t count);
int sysmgr_perf_mmap(struct vspace_s *vspace, uintptr_t aligned_addr, size_t aligned_len,
		     uint32_t prot, uint32_t flags, int remote_fd, uint64_t offset, uintptr_t *rvaddr);
#else
static inline unsigned long get_perf_event_mlock_kb(void)
{
	return 0UL;
}
static inline void set_perf_event_mlock_kb(unsigned long mlock_kb)
{
	UNUSED(mlock_kb);
}
static inline int sysmgr_perf_mmap(struct vspace_s *vspace, uintptr_t aligned_addr, size_t aligned_len,
				   uint32_t prot, uint32_t flags, int remote_fd, uint64_t offset, uintptr_t *rvaddr)
{
	return E_HM_NOSYS;
}
#endif

struct thread_s;
struct vregion_s;

#if defined(CONFIG_HMPERF_NG) && !defined(__HOST_LLT__)
void perf_event_comm(struct thread_s *thread);
void perf_event_mmap(struct vregion_s *vr);
#else
static inline void perf_event_comm(struct thread_s *thread)
{
	UNUSED(thread);
}
static inline void perf_event_mmap(struct vregion_s *vr)
{
	UNUSED(vr);
}
#endif

#ifdef CONFIG_HMPERF_NG
int perf_init_sysfs(void);
#else
static inline int perf_init_sysfs(void)
{
	return 0;
}
#endif
#endif
