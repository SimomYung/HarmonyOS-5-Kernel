/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of hmtrace control API
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#ifndef ULIBS_LIBHMTRACE_HMTRACECTRL_H
#define ULIBS_LIBHMTRACE_HMTRACECTRL_H

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <libstrict/strict.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>

/* get registered trace_class list name */
int hm_trace_get_registered_class_list_by_pid(pid_t pid, const char **buffer);

/* get registered events list name in specific class */
int hm_trace_get_registered_events_list_by_pid(pid_t pid, const char *class_name,
					       const char **buffer);

/* get available events list name in specific class */
int hm_trace_get_available_events_list_by_pid(pid_t pid, const char *class_name,
					      const char **buffer);

/* register event dynamicly by hmtrace.elf */
int hm_trace_register_event_by_pid(pid_t pid, const char *class_name,
				   const void *buffer, unsigned int buffer_size);

/* unregister event dynamicly by hmtrace.elf */
int hm_trace_unregister_event_by_pid(pid_t pid, const char *class_name,
				     const void *buffer, unsigned int buffer_size);

/* set event tracing on by client */
int hm_trace_set_event_tracing_on_by_pid(pid_t pid, const char *class_name,
					 const char *event_name);

/* set event tracing off by client */
int hm_trace_set_event_tracing_off_by_pid(pid_t pid, const char *class_name,
					  const char *event_name);

/* get event tracing status by client */
int hm_trace_get_event_tracing_status_by_pid(pid_t pid,
					     const char *class_name,
					     const char *event_name,
					     int *status);

/* get event tracing eid by client */
int hm_trace_get_event_tracing_eid_by_pid(pid_t pid,
					  const char *class_name,
					  const char *event_name,
					  int *eid);

/*
 * get the ring buffer information of the process specified by pid
 *
 * @param	pid		pid of tracemgr process
 * @param	class_name	[I] trace class's name
 * @param	rb		[O] pointer to ring buffer
 *
 * @return	return 0 and rb->shm_id/rb_sz/blk_num will be filled if success,
 * or return negative if fail
 * @note	this api is meant to be used by client
 */
int hm_trace_get_rb_info_by_pid(pid_t pid, const char *class_name,
				struct rb *rb);

/*
 * Open IPC buffer, used by client
 *
 * @param	pid	[I]	pid of the IPC buffer server
 * @param	name	[I]	IPC buffer name
 *
 * @return	handle of ipc buffer if success, or error number if fail
 */
int hm_trace_ipc_buf_open(pid_t pid, const char *name);

/*
 * Read item of IPC buffer, used by client
 *
 * @param	pid	[I]	pid of IPC buffer server
 * @param	handle	[I]	IPC buffer handle
 * @item	item	[O]	the buffer to store the data from IPC buffer
 *
 * @retuen	read data size from server if success, or error number if fail
 */
int hm_trace_ipc_buf_read_item(pid_t pid, int handle, void **item);

/*
 * Close the IPC buffer, used by client
 *
 * @param	pid	[I]	pid of IPC buffer server
 * @param	handle	[I]	IPC buffer handle
 *
 * @return	E_HM_OK if success, or error number if fail
 */
int hm_trace_ipc_buf_close(pid_t pid, int handle);

/*
 * Turn on/off all trace events
 * @param	pid [I]		pid of IPC buffer server
 * @param	status [I]	trace event status
 *
 * @return	E_HM_OK if success, or error number if fail
 */
int hm_trace_tracing_on_all_events(pid_t pid, unsigned int status);

/*
 * Allocate a new trace buffer
 * @param	pid		[I] pid of IPC buffer server
 * @param	class_name	[I] trace class's name
 * @param	buffer_size	[I] buffer size
 *
 * @return	E_HM_OK if success, or error number if fail
 */
int hm_trace_realloc_trace_buffer_by_pid(pid_t pid, const char *class_name, unsigned int buffer_size);

/*
 * Obtain the size of trace buffer
 * @param	pid		[I] pid of IPC buffer server
 * @param	class_name	[I] trace class's name
 * @param	buffer_size	[O] trace buffer size
 *
 * @return	E_HM_OK if success, or error number if fail
 */
int hm_trace_buffer_size_of_pid(pid_t pid, const char *class_name, size_t *buffer_size);
#endif
