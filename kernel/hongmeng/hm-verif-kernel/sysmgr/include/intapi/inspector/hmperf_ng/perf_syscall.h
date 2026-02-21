/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmperf ng syscall intapi in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 24 11:43:10 2023
 */
#ifndef SYSMGR_INTAPI_INSPECTOR_HMPERF_NG_PERF_SYSCALL_H
#define SYSMGR_INTAPI_INSPECTOR_HMPERF_NG_PERF_SYSCALL_H

#include <hongmeng/errno.h>
#include <hongmeng/types.h>

struct perf_event;
struct perf_event_arg;

#ifndef PERF_FLAG_FD_CLOEXEC
#define PERF_FLAG_FD_CLOEXEC		(1UL << 3)
#endif

#ifdef CONFIG_HMPERF_NG
int sysmgr_perf_close(struct perf_event *event);
ssize_t sysmgr_perf_read(struct perf_event *event, void *buf, size_t count);
int sysmgr_perf_ioctl(struct perf_event *event, unsigned int cmd, unsigned long arg);
int sysmgr_perf_event_open(struct perf_event_arg *arg, struct perf_event **pevent);
#else
static inline int sysmgr_perf_close(struct perf_event *event)
{
	UNUSED(event);
	return E_HM_NOSYS;
}

static inline ssize_t sysmgr_perf_read(struct perf_event *event, void *buf, size_t count)
{
	UNUSED(event, buf, count);
	return E_HM_NOSYS;
}

static inline int sysmgr_perf_ioctl(struct perf_event *event, unsigned int cmd, unsigned long arg)
{
	UNUSED(event, cmd, arg);
	return E_HM_NOSYS;
}

static inline int sysmgr_perf_event_open(struct perf_event_arg *arg, struct perf_event **pevent)
{
	UNUSED(arg, event);
	return E_HM_NOSYS;
}

#endif

#endif
