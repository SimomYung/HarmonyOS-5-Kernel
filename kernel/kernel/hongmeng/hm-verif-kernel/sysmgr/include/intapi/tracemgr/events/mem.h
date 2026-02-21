/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: trace list for mem
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 21 04:16:58 2025
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(oom, oom_score_adj_update,
	TP_PROTO(struct process_s *, process, const char *, thread_name),
	TP_ARGS(process, thread_name),
	TP_FIELD(
		__field(int, pid)
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(short, oom_score_adj)
	),
	TP_ASSIGN(
		__entry->pid = process_pid_of(process);
		NOFAIL(strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name, HMCAP_THREAD_NAME_SIZE - 1));
		__entry->oom_score_adj = (short)process_oom_score_adj_of(process);
	),
	TP_PRINT("pid=%d comm=%s oom_score_adj=%d",
		__entry->pid, __entry->comm, __entry->oom_score_adj
	)
)

TRACE_EVENT(kmem, rss_stat,
	TP_PROTO(struct vspace_s *, vspace, bool, is_file),
	TP_ARGS(vspace, is_file),
	TP_FIELD(
		__field(unsigned int, mm_id)
		__field(unsigned int, curr)
		__field(int, member)
		__field(long, size)
	),
	TP_ASSIGN(
		__entry->mm_id = (unsigned int)vspace_rref_of(vspace);
		__entry->curr = 1;
		__entry->member = is_file ? (int)PAGE_FILE : (int)PAGE_ANON;
		__entry->size = (long)vspace_rss_of(vspace);
	),
	TP_PRINT("mm_id=%u curr=%d member=%d size=%ldB",
		__entry->mm_id,
		__entry->curr,
		__entry->member,
		__entry->size
	)
)

TRACE_EVENT(memmgr, sync_reclaim_begin,
	TP_PROTO(unsigned int, order, unsigned int, paf),
	TP_ARGS(order, paf),
	TP_FIELD(
		__field(unsigned int, order)
		__field(unsigned int, paf)
	),
	TP_ASSIGN(
		__entry->order = order;
		__entry->paf = paf;
	),
	TP_PRINT("order=%u paf=%u",
		__entry->order, __entry->paf
	)
)

TRACE_EVENT(memmgr, sync_reclaim_end,
	TP_PROTO(unsigned long, nr_reclaimed),
	TP_ARGS(nr_reclaimed),
	TP_FIELD(
		__field(unsigned long, nr_reclaimed)
	),
	TP_ASSIGN(
		__entry->nr_reclaimed = nr_reclaimed;
	),
	TP_PRINT("nr_reclaimed=%lu",
		__entry->nr_reclaimed
	)
)

TRACE_EVENT(memmgr, reclaim_thread_wake,
	TP_PROTO(unsigned long, target, unsigned long, memfree),
	TP_ARGS(target, memfree),
	TP_FIELD(
		__field(unsigned long, memfree)
		__field(unsigned long, target)
	),
	TP_ASSIGN(
		__entry->memfree = memfree;
		__entry->target = target;
	),
	TP_PRINT("memfree=%lu reclaim_target=%lu",
		__entry->memfree, __entry->target
	)
)

TRACE_EVENT(memmgr, reclaim_thread_sleep,
	TP_PROTO(unsigned long, nr_reclaimed, unsigned long, memfree),
	TP_ARGS(nr_reclaimed, memfree),
	TP_FIELD(
		__field(unsigned long, memfree)
		__field(unsigned long, nr_reclaimed)
	),
	TP_ASSIGN(
		__entry->memfree = memfree;
		__entry->nr_reclaimed = nr_reclaimed;
	),
	TP_PRINT("memfree=%lu nr_reclaimed=%lu",
		__entry->memfree, __entry->nr_reclaimed
	)
)

#define XACCT_MAX_TRACE_BUF 64
/* Zero is set so that it can follow the default PID of the trace log */
#define XACCT_MARK_WRITE_DEFAULT_PID 0
#define XACCT_TRACE_BEGIN true
#define XACCT_TRACE_END false

TRACE_EVENT(xacct, tracing_mark_write,
	TP_PROTO(pid_t, pid, const char *, name, bool, start),
	TP_ARGS(pid, name, start),
	TP_FIELD(
		__field(pid_t, pid)
		__array(char, name, XACCT_MAX_TRACE_BUF)
		__field(int, start)
	),
	TP_ASSIGN(
		__entry->pid = pid;
		__entry->start = start;
		if (start) {
			NOFAIL(strncpy_s(__entry->name, XACCT_MAX_TRACE_BUF, name, XACCT_MAX_TRACE_BUF - 1));
		}
	),
	TP_PRINT("%c|%d|%s",
		 "EB"[__entry->start], __entry->pid, __entry->start ? __entry->name : ""
	)
)
