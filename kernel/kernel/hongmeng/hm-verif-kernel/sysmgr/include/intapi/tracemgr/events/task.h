/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: trace list for task
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 21 04:16:58 2025
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */
TRACE_EVENT(sched, sched_process_exit,
	TP_PROTO(struct thread_s *, thread, pid_t, pid, int, prio),
	TP_ARGS(thread, pid, prio),
	TP_FIELD(
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(int, prio)
	),
	TP_ASSIGN(
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = pid;
		__entry->prio = prio;
	),
	TP_PRINT("comm=%s pid=%d prio=%d",
		__entry->comm, __entry->pid, __entry->prio
	)
)

TRACE_EVENT(sched, sched_process_free,
	TP_PROTO(struct thread_s *, thread, pid_t, pid, int, prio),
	TP_ARGS(thread, pid, prio),
	TP_FIELD(
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(int, prio)
	),
	TP_ASSIGN(
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = pid;
		__entry->prio = prio;
	),
	TP_PRINT("comm=%s pid=%d prio=%d",
		__entry->comm, __entry->pid, __entry->prio
	)
)

#ifdef CONFIG_THREAD_EXIT_EVENT
#define THREAD_EXIT_EVENT_MAX_STR 32
#define ONLINE_TIMESTAMP_FMT "%ld.%09lds"

TRACE_EVENT(sched, sched_thread_exit_event,
	TP_PROTO(struct thread_s *, thread, pid_t, pid, pid_t, ppid, __u64, total, __u64, ready, __u64, running,
		struct timespec *, total_ts, struct timespec *, ready_ts, struct timespec *, run_ts),
	TP_ARGS(thread, pid, ppid, total, ready, running, total_ts, ready_ts, run_ts),
	TP_FIELD(
		__array(char, name, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(pid_t, ppid)
		__field(pid_t, tid)
		__field(unsigned long, total)
		__field(unsigned long, ready)
		__field(unsigned long, running)
		__array(char, total_time, THREAD_EXIT_EVENT_MAX_STR)
		__array(char, ready_time, THREAD_EXIT_EVENT_MAX_STR)
		__array(char, run_time, THREAD_EXIT_EVENT_MAX_STR)
	),
	TP_ASSIGN(
		strncpy_s(__entry->name, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = pid;
		__entry->ppid = ppid;
		__entry->tid = thread_tid_of(thread);
		__entry->total = total;
		__entry->ready = ready;
		__entry->running = running;
		snprintf_s(__entry->total_time, THREAD_EXIT_EVENT_MAX_STR, THREAD_EXIT_EVENT_MAX_STR - 1,
			   ONLINE_TIMESTAMP_FMT, total_ts->tv_sec, total_ts->tv_nsec);
		snprintf_s(__entry->ready_time, THREAD_EXIT_EVENT_MAX_STR, THREAD_EXIT_EVENT_MAX_STR - 1,
			   ONLINE_TIMESTAMP_FMT, ready_ts->tv_sec, ready_ts->tv_nsec);
		snprintf_s(__entry->run_time, THREAD_EXIT_EVENT_MAX_STR, THREAD_EXIT_EVENT_MAX_STR - 1,
			   ONLINE_TIMESTAMP_FMT, run_ts->tv_sec, run_ts->tv_nsec);
	),
	TP_PRINT("name=%s, pid=%d, ppid=%d, tid=%d, total=%lu, ready=%lu, running=%lu, "
		 "total time=%s, ready=%s, run=%s",
		__entry->name, __entry->pid, __entry->ppid, __entry->tid, __entry->total, __entry->ready, __entry->running,
		__entry->total_time, __entry->ready_time, __entry->run_time
	)
)
#endif

TRACE_EVENT(sched, sched_radical_user_qos,
	TP_PROTO(struct thread_s *, thread, pid_t, pid, int, prio),
	TP_ARGS(thread, pid, prio),
	TP_FIELD(
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(int, prio)
	),
	TP_ASSIGN(
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = pid;
		__entry->prio = prio;
	),
	TP_PRINT("comm=%s pid=%d prio=%d",
		__entry->comm, __entry->pid, __entry->prio
	)
)

TRACE_EVENT(task, task_newtask,
	TP_PROTO(struct thread_s *, thread, __u64, clone_flags),
	TP_ARGS(thread, clone_flags),
	TP_FIELD(
		__field(pid_t, pid)
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(unsigned long, clone_flags)
		__field(short, oom_score_adj)
	),
	TP_ASSIGN(
		struct process_s *process = thread->process;
		__entry->pid = ((process == NULL) ? PID_NONE : process_pid_of(process));
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->clone_flags = clone_flags;
		__entry->oom_score_adj = process_oom_score_adj_of(process);
	),
	TP_PRINT("pid=%d comm=%s clone_flags=%lx oom_score_adj=%hd",
		__entry->pid, __entry->comm, __entry->clone_flags, __entry->oom_score_adj
	)
)

TRACE_EVENT(task, task_rename,
	TP_PROTO(struct thread_s *, thread, const char *, newcomm, pid_t, pid),
	TP_ARGS(thread, newcomm, pid),
	TP_FIELD(
		__field(pid_t, pid)
		__array(char, oldcomm, HMCAP_THREAD_NAME_SIZE)
		__array(char, newcomm, HMCAP_THREAD_NAME_SIZE)
		__field(short, oom_score_adj)
	),
	TP_ASSIGN(
		__entry->pid = pid;
		strncpy_s(__entry->oldcomm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		strncpy_s(__entry->newcomm, HMCAP_THREAD_NAME_SIZE, newcomm,
			strnlen(newcomm, HMCAP_THREAD_NAME_SIZE - 1));
		__entry->oom_score_adj = ((thread == NULL) ? -1 : process_oom_score_adj_of(thread->process));
	),
	TP_PRINT("pid=%d oldcomm=%s newcomm=%s oom_score_adj=%hd",
		__entry->pid, __entry->oldcomm, __entry->newcomm, __entry->oom_score_adj
	)
)

