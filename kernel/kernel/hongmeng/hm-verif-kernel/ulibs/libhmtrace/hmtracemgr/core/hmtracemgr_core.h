/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of hmtracemgr inner interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#ifndef ULIBS_LIBHMTRACE_HMTRACEMGR_UTILS_H
#define ULIBS_LIBHMTRACE_HMTRACEMGR_UTILS_H

#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libbunch/bunch_ipc.h>

/* hmtracemgr functions */
int hmtracemgr_init(pid_t pid);
int hmtracemgr_get_registered_class_list(struct bunch_ipc_attr *attr);
int hmtracemgr_get_registered_events_list(const char *class_name, struct bunch_ipc_attr *attr);
int hmtracemgr_get_available_events_list(const char *class_name, struct bunch_ipc_attr *attr);
int hmtracemgr_register_event(const char *class_name, const void *buffer,
			      unsigned int buffer_size);
int hmtracemgr_unregister_event(const char *class_name, const void *buffer,
				unsigned int buffer_size);
int hmtracemgr_set_event_on(const char *class_name, const char *event_name);
int hmtracemgr_set_event_off(const char *class_name, const char *event_name);
int hmtracemgr_get_event_status(const char *class_name, const char *event_name,
				int *status);
int hmtracemgr_get_event_eid(const char *class_name, const char *event_name,
			     int *eid);
int hmtracemgr_get_rb_info(const char *class_name, struct rb *rb);
int hmtracemgr_trace_buffer_size(const char *class_name, size_t *buffer_size);
int hmtracemgr_realloc_trace_buffer(const char *class_name, size_t size);
#endif
