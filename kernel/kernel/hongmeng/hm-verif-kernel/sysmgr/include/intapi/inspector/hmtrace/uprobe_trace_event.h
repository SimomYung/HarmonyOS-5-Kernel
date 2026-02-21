/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Frontend operations of uprobe
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 7 15:50:16 2020
 */

#ifndef SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_TRACE_EVENT_H
#define SYSMGR_INTAPI_INSPECTOR_HMTRACE_UPROBE_TRACE_EVENT_H

#include <stdint.h>
#include <unistd.h>

#include <intapi/process/process.h>
#include <intapi/thread/thread.h>
#include <intapi/inspector/hmtrace/uprobe.h>
#include <intapi/mem/file.h>

struct uprobe_trace_event;

#ifdef CONFIG_UPROBE_TRACE
int register_uprobe_trace_event(char *file_path, char *symbol_name,
				unsigned long file_offset, pid_t pid,
				struct uprobe_trace_event **pevent);
int register_uprobe_for_process(struct uprobe_trace_event *event,
			struct process_s *proc, struct vspace_s *vspace);

void unregister_uprobe_trace_event(struct uprobe_trace_event *event);

int uprobe_trace_event_attach_handlers(struct uprobe_trace_event *event,
					struct uprobe_handlers *handlers);

void uprobe_trace_event_detach_handlers(struct uprobe_trace_event *event,
					struct uprobe_handlers *handlers);

void uprobe_mmap(struct vspace_s *vspace, unsigned long fd_file);
#else
static inline int register_uprobe_trace_event(char *file_path, char *symbol_name,
			unsigned long file_offset, pid_t pid,
			struct uprobe_trace_event **pevent)
{
	UNUSED(file_path, symbol_name, file_offset, pid, pevent);
	return E_HM_NOSYS;
}

static inline int register_uprobe_for_process(struct uprobe_trace_event *event,
					      struct process_s *proc, struct vspace_s *vspace)
{
	UNUSED(event, proc, vspace);
	return E_HM_NOSYS;
}

static inline void unregister_uprobe_trace_event(struct uprobe_trace_event *event)
{
	UNUSED(event);
}

static inline int uprobe_trace_event_attach_handlers(struct uprobe_trace_event *event,
							struct uprobe_handlers *handlers)
{
	UNUSED(event, handlers);
	return E_HM_NOSYS;
}

static inline void uprobe_trace_event_detach_handlers(struct uprobe_trace_event *event,
							struct uprobe_handlers *handlers)
{
	UNUSED(event, handlers);
}

static inline void uprobe_mmap(struct vspace_s *vspace, unsigned long fd_file)
{
	UNUSED(vspace, fd_file);
}
#endif

void **rasfile_probe_args(struct uprobe_context *context);
int rasfile_probe_pre_handler(struct uprobe_context *context, void *arg);
void rasfile_probe_set_ret(struct uprobe_context *context, long retval);
long regs_return_value(struct uprobe_context *context);
int rasfile_probe_init(char *file_path, char *symbol_name, uprobe_handler_t pre_handler, uprobe_handler_t ret_handler,
		       void *args, struct uprobe_trace_event **evt);
void rasfile_probe_destroy(struct uprobe_trace_event *evt, uprobe_handler_t pre_handler, uprobe_handler_t ret_handler,
			   void *args);

#endif
