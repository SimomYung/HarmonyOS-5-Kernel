/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Perf intapi in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 30 10:05:16 2020
 */

#ifndef SYSMGR_INTAPI_INSPECTOR_PERF_H
#define SYSMGR_INTAPI_INSPECTOR_PERF_H

#include <unistd.h>
#include <intapi/thread/thread_ops.h>
#include <lib/dlist.h>
#include <libperf/perf_event.h>
#include <hmkernel/drivers/hypervisor.h>

/*
 * perf session state transition:
 *             init             start
 *  UNINITED --------> INITED ---------> STARTED
 *  (invisible)         |                  ^|
 *                      |release           ||
 *                      |             start|| stop
 *                      v                  ||
 *                (invisible)  release     |v
 *                 RELEASED <----------- STOPPED
 */
enum perf_session_state {
	SESSION_STATE_INITED,
	SESSION_STATE_STARTED,
	SESSION_STATE_STOPPED,
};

/*
 * buffer page state transition:
 *               insufficient space
 *   IDLE(init) ----------------------> FULL
 *       ^         fulsh buffer          |
 *       |                               | all data is written
 *       | data read complete            |
 *       |                               v
 *       ---------------------------- READABLE
 */
enum perf_buffer_page_state {
	PERF_PAGE_STATE_IDLE,
	PERF_PAGE_STATE_FULL,
	PERF_PAGE_STATE_READABLE,
};

struct perf_page_s {
	void *vaddr;
	bool last;
	uint32_t write_size;
	int ref;
	enum perf_buffer_page_state state;
};

struct perf_buffer_s {
	int futex_waiter;
	uint32_t page_nr;

	uint32_t read_index;
	uint32_t write_head;

	uint64_t lost_events;
	uint64_t total_events;

	void *vaddr;
	struct perf_page_s *pages;
};

struct perf_session_s {
	struct dlist_node node;

	/* protect insert/delete event_list */
	struct raw_mutex event_mutex;
	struct dlist_node event_list;

	/* pid of perf process, used to distinguish different sessions */
	pid_t pid;

	/* for details, see description of perf_session_state */
	enum perf_session_state state;

	/* reader reference count, session can be released only when value is 0 */
	int ref;

	/*
	 * different uev need to be registered for different event modes,
	 * register uev at a time when session start, and unregister when stop
	 */
	unsigned int notify_flag;

	/*
	 * all events in a session share a buffer, buffer is managed by pages,
	 * page data is sent to perf process through the sysif interface
	 */
	struct perf_buffer_s buffer;
};

struct perf_event_s;
struct perf_event_ops_s {
	int (*init)(struct perf_event_s *event);
	int (*start)(struct perf_event_s *event, struct process_s *session);
	int (*stop)(const struct perf_event_s *event,
		    const struct process_s *session, bool write);
	void (*release)(const struct perf_event_s *event);
};

struct perf_event_s {
	struct dlist_node node;

	/* indicates the session to which an event belongs */
	struct perf_session_s *session;

	/* each type of perf_event needs to implement all interfaces in ops */
	const struct perf_event_ops_s *ops;

	/* perf event type */
	enum _perf_event_type type;

	/* for per-process indicates pid, and for perf-thread indicates tid */
	pid_t owner;

	/* used for per-cpu */
	unsigned int cpu;

	/* used for per-cluster */
	unsigned int cluster;

	/* pmu event config */
	struct __perf_event_config_s config;
};

struct vperfctx_s {
	/* The perf event */
	struct perf_event_s *event;

	/* The number of vcpu */
	uint32_t vcpu_nr;

	/* The state of vcpu */
	uint32_t vcpu_status[MAX_VCPUS];
};

struct perf_cpuctx_s {
	/* cpucontext cref */
	cref_t cref;

	/* pmem alloc for cpucontext */
	struct pmem_area_s *mem;
};

struct perf_dsuctx_s {
	struct dlist_node node;
	uint32_t src_cnode_id_key;

	/* PMU cpucontext cref */
	cref_t cref;

	/* pmem alloc for PMU cpucontext */
	struct pmem_area_s *mem;
};
#ifdef CONFIG_SYSMGR_PERF
struct thread_perf_ops_s {
	int (*cpuctx_attach_tcb)(struct perf_event_s *event,
			  struct process_s *process, struct thread_s *thread);
	int (*cpuctx_detach_tcb)(const struct perf_event_s *event,
			  struct process_s *process, struct thread_s *thread, bool write);
};
extern const thread_perf_ops_t klt_perf_ops;
#endif
#endif
