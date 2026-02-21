/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 25 16:16:35 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#include <hmsysmgr/trace/events/hp_turbo_trace_list.h>

TRACE_EVENT(sched, sched_process_exit,
	TP_PROTO(struct thread_s *, thread),
	TP_ARGS(thread),
	TP_FIELD(
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(int, prio)
	),
	TP_ASSIGN(
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = ((thread == NULL) ? PID_NONE : thread_tid_of(thread));
		__entry->prio = ((thread == NULL) ? -1 : thread_getpriority(thread));
	),
	TP_PRINT("comm=%s pid=%d prio=%d",
		__entry->comm, __entry->pid, __entry->prio
	)
)

TRACE_EVENT(sched, sched_process_free,
	TP_PROTO(struct thread_s *, thread),
	TP_ARGS(thread),
	TP_FIELD(
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(int, prio)
	),
	TP_ASSIGN(
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = ((thread == NULL) ? PID_NONE : thread_tid_of(thread));
		__entry->prio = ((thread == NULL) ? -1 : thread_getpriority(thread));
	),
	TP_PRINT("comm=%s pid=%d prio=%d",
		__entry->comm, __entry->pid, __entry->prio
	)
)

TRACE_EVENT(sched, sched_radical_user_qos,
	TP_PROTO(struct thread_s *, thread),
	TP_ARGS(thread),
	TP_FIELD(
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(pid_t, pid)
		__field(int, prio)
	),
	TP_ASSIGN(
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		__entry->pid = ((thread == NULL) ? PID_NONE : thread_tid_of(thread));
		__entry->prio = ((thread == NULL) ? -1 : thread_getpriority(thread));
	),
	TP_PRINT("comm=%s pid=%d prio=%d",
		__entry->comm, __entry->pid, __entry->prio
	)
)

TRACE_EVENT(oom, oom_score_adj_update,
	TP_PROTO(struct process_s *, process),
	TP_ARGS(process),
	TP_FIELD(
		__field(int, pid)
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
		__field(int, oom_score_adj)
	),
	TP_ASSIGN(
		__entry->pid = process_pid_of(process);
		process_main_thread_name_of(process, __entry->comm, HMCAP_THREAD_NAME_SIZE);
		__entry->oom_score_adj = process_oom_score_adj_of(process);
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
	TP_PROTO(unsigned long, target),
	TP_ARGS(target),
	TP_FIELD(
		__field(unsigned long, memfree)
		__field(unsigned long, target)
	),
	TP_ASSIGN(
		__entry->memfree = memstat_free();
		__entry->target = target;
	),
	TP_PRINT("memfree=%lu reclaim_target=%lu",
		__entry->memfree, __entry->target
	)
)

TRACE_EVENT(memmgr, reclaim_thread_sleep,
	TP_PROTO(unsigned long, nr_reclaimed),
	TP_ARGS(nr_reclaimed),
	TP_FIELD(
		__field(unsigned long, memfree)
		__field(unsigned long, nr_reclaimed)
	),
	TP_ASSIGN(
		__entry->memfree = memstat_free();
		__entry->nr_reclaimed = nr_reclaimed;
	),
	TP_PRINT("memfree=%lu nr_reclaimed=%lu",
		__entry->memfree, __entry->nr_reclaimed
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
	TP_PROTO(struct thread_s *, thread, const char *, newcomm),
	TP_ARGS(thread, newcomm),
	TP_FIELD(
		__field(pid_t, pid)
		__array(char, oldcomm, HMCAP_THREAD_NAME_SIZE)
		__array(char, newcomm, HMCAP_THREAD_NAME_SIZE)
		__field(short, oom_score_adj)
	),
	TP_ASSIGN(
		__entry->pid = ((thread == NULL) ? PID_NONE : thread_tid_of(thread));
		strncpy_s(__entry->oldcomm, HMCAP_THREAD_NAME_SIZE, thread_name_of(thread), HMCAP_THREAD_NAME_SIZE - 1);
		strncpy_s(__entry->newcomm, HMCAP_THREAD_NAME_SIZE, newcomm,
			strnlen(newcomm, HMCAP_THREAD_NAME_SIZE - 1));
		__entry->oom_score_adj = ((thread == NULL) ? -1 : process_oom_score_adj_of(thread->process));
	),
	TP_PRINT("pid=%d oldcomm=%s newcomm=%s oom_score_adj=%hd",
		__entry->pid, __entry->oldcomm, __entry->newcomm, __entry->oom_score_adj
	)
)

TRACE_EVENT(ptrace, ptrace_handle_request,
	TP_PROTO(const struct ptrace_arg_s *, ptrace_arg, int, ret_val),
	TP_ARGS(ptrace_arg, ret_val),
	TP_FIELD(
		__field(int, request)
		__field(pid_t, pid)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->request = ptrace_arg->request;
		__entry->pid = ptrace_arg->pid;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("pid=%d request=%d return %d",
		 __entry->pid, __entry->request, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, attach_debugger,
	TP_PROTO(const struct thread_s *, debugger_thread,
		 const struct thread_s *, thread, bool, is_seize, int, ret_val),
	TP_ARGS(debugger_thread, thread, is_seize, ret_val),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(bool, is_seize)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = thread_tid_of(debugger_thread);
		__entry->tracee_pid = thread_tid_of(thread);
		__entry->is_seize = is_seize;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d is_seize=%d return %d",
		__entry->tracer_pid, __entry->tracee_pid, __entry->is_seize, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, detach_debugger,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread),
	TP_ARGS(debugger, thread),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = thread_tid_of(thread);
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d",
		__entry->tracer_pid, __entry->tracee_pid
	)
)

TRACE_EVENT(ptrace, handle_debug_act,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread,
		 enum pdebug_act_type, act_type, bool, enqueue_head, int, ret_val),
	TP_ARGS(debugger, thread, act_type, enqueue_head, ret_val),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(int, act_type)
		__field(int, enqueue_head)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = thread_tid_of(thread);
		__entry->act_type = act_type;
		__entry->enqueue_head = enqueue_head;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d act_type=%d enqueue_head=%d return %d",
		__entry->tracer_pid, __entry->tracee_pid, __entry->act_type,
		__entry->enqueue_head, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, handle_debug_act_direct,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread,
		 const struct pdebug_act *, act, enum pdebug_act_type, act_type,
		 int, ret_val),
	TP_ARGS(debugger, thread, act, act_type, ret_val),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(__u64, data)
		__field(int, act_type)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = thread_tid_of(thread);
		__entry->act_type = act_type;
		__entry->data = act != NULL ? ptr_to_ulong(act->data) : 0;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d act_type=%d data=0x%llx return %d",
		__entry->tracer_pid, __entry->tracee_pid, __entry->act_type,
		__entry->data, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, process_debug_act,
	TP_PROTO(const struct process_s *, debugger, const struct pdebug_act *, act,
		 int, ret_val),
	TP_ARGS(debugger, act, ret_val),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(int, act_type)
		__field(__u64, data)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->act_type = act->type;
		__entry->data = ptr_to_ulong(act->data);
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d act_type=%d data=0x%llx return %d",
		__entry->tracer_pid, __entry->act_type, __entry->data, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, notify_event,
	TP_PROTO(const struct process_s *, debugger, const struct pdebug_event *, event),
	TP_ARGS(debugger, event),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(int, event_type)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->event_type = event->type;
	),
	TP_PRINT("tracer_pid=%d event_type=%d",
		__entry->tracer_pid, __entry->event_type
	)
)

TRACE_EVENT(ptrace, do_notify_debugger,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread,
		 const struct __siginfo *, siginfo),
	TP_ARGS(debugger, thread, siginfo),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(__u32, si_code)
		__field(__u32, signum)
		__field(__u32, si_type)
		__field(__s32, pid)
		__field(__u32, uid)
		__field(__s32, status)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = thread_tid_of(thread);
		__entry->si_code = siginfo->si_code;
		__entry->signum = siginfo->signum;
		__entry->si_type = siginfo->si_type;
		__entry->pid = siginfo->__sigchld.pid;
		__entry->uid = siginfo->__sigchld.uid;
		__entry->status = siginfo->__sigchld.status;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d si_code=%u signum=%u si_type=%u pid=%d uid=%u status=0x%x",
		 __entry->tracer_pid, __entry->tracee_pid, __entry->si_code,
		 __entry->signum, __entry->si_type, __entry->pid, __entry->uid,
		 __entry->status
	)
)

TRACE_EVENT(ptrace, get_thread_status,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread,
		 pid_t, wait_pid, const struct pdebug_event *, event),
	TP_ARGS(debugger, thread, wait_pid, event),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(pid_t, wait_pid)
		__field(int, event_type)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = thread_tid_of(thread);
		__entry->wait_pid = wait_pid;
		__entry->event_type = event->type;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d wait_pid=%d event_type=%d",
		 __entry->tracer_pid, __entry->tracee_pid, __entry->wait_pid,
		 __entry->event_type
	)
)

TRACE_EVENT(cgroup, cgroup_attach_task,
	TP_PROTO(struct res_group *, rgrp, struct process_s *, process, const char *, comm),
	TP_ARGS(rgrp, process, comm),
	TP_FIELD(
		__field(int, dst_root)
		__field(int, dst_level)
		__field(__u64, dst_id)
		__field(int, pid)
		__string(dst_path, rgrp->hash_key)
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
	),
	TP_ASSIGN(
		__entry->dst_root = 0;
		__entry->dst_id = 0;
		__entry->dst_level = 0;
		__entry->pid = (process == NULL ? PID_NONE : process_pid_of(process));
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, comm, strnlen(comm, HMCAP_THREAD_NAME_SIZE - 1));
		__assign_str(dst_path, rgrp->hash_key);
	),
	TP_PRINT("dst_root=%d dst_id=%llu dst_level=%d dst_path=%s pid=%d comm=%s",
		__entry->dst_root, __entry->dst_id, __entry->dst_level, __get_str(dst_path), __entry->pid, __entry->comm
	)
)

#define XACCT_MAX_TRACE_BUF (64)
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
			NOFAIL(strcpy_s(__entry->name, XACCT_MAX_TRACE_BUF, name));
		}
	),
	TP_PRINT("%c|%d|%s",
		 "EB"[__entry->start], __entry->pid, __entry->start ? __entry->name : ""
	)
)

#define DEFINE_CGROUP_EVENT(FUNC)                                           \
	TRACE_EVENT(cgroup, FUNC,                                           \
		TP_PROTO(struct res_group *, rgrp),                         \
		TP_ARGS(rgrp),						    \
		TP_FIELD(                                                   \
			__field(int, root)                                  \
			__field(int, level)                                 \
			__field(__u64, id)                                  \
			__string(path, rgrp->hash_key)                      \
		),                                                          \
		TP_ASSIGN(                                                  \
			__entry->root = 0;                                  \
			__entry->id = 0;                                   \
			__entry->level = 0;                                 \
			__assign_str(path, rgrp->hash_key);                 \
		),                                                          \
		TP_PRINT("root=%d id=%llu level=%d path=%s",               \
			__entry->root, __entry->id, __entry->level, __get_str(path)                                \
		)                                                           \
	)

DEFINE_CGROUP_EVENT(cgroup_mkdir)
DEFINE_CGROUP_EVENT(cgroup_rmdir)
DEFINE_CGROUP_EVENT(cgroup_freeze)
DEFINE_CGROUP_EVENT(cgroup_unfreeze)

#include <hmsysmgr/trace/events/damon.h>

TRACE_EVENT(hkids, hkids_ptrace,
	TP_PROTO(struct thread_s *, debugger_thread, struct ptrace_arg_s *, arg),
	TP_ARGS(debugger_thread, arg),
	TP_FIELD(
		__field(pid_t, debugger_pid)
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->debugger_pid = ((debugger_thread == NULL) ? PID_NONE : thread_tid_of(debugger_thread));
		__entry->pid = arg->pid;
	),
	TP_PRINT("debugger_pid=%d pid=%d",
		__entry->debugger_pid, __entry->pid
	)
)

TRACE_EVENT(hkids, hkids_task_newtask,
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

TRACE_EVENT(hkids, hkids_process_exit,
	TP_PROTO(struct process_s *, process, int, exit_status),
	TP_ARGS(process, exit_status),
	TP_FIELD(
		__field(int, exit_status)
	),
	TP_ASSIGN(
		__entry->exit_status = exit_status;
	),
	TP_PRINT("exit_status=%d",
		__entry->exit_status
	)
)

TRACE_EVENT(hkids, hkids_process_coredump,
	TP_PROTO(struct process_s *, process),
	TP_ARGS(process),
	TP_FIELD(
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->pid = ((process == NULL) ? PID_NONE : process_pid_of(process));
	),
	TP_PRINT("pid=%d",
		__entry->pid
	)
)

TRACE_EVENT(hkids, hkids_task_setuid,
	TP_PROTO(struct process_s *, process, struct thread_s *, thread, uid_t, uid),
	TP_ARGS(process, thread, uid),
	TP_FIELD(
		__field(pid_t, pid)
		__field(uid_t, new_uid)
	),
	TP_ASSIGN(
		__entry->pid = ((process == NULL) ? PID_NONE : process_pid_of(process));
		__entry->new_uid = uid;
	),
	TP_PRINT("pid=%d new_uid=%d",
		__entry->pid, __entry->new_uid
	)
)

TRACE_EVENT(hkids, hkids_task_setgid,
	TP_PROTO(struct process_s *, process, struct thread_s *, thread, gid_t, gid),
	TP_ARGS(process, thread, gid),
	TP_FIELD(
		__field(pid_t, pid)
		__field(gid_t, new_gid)
	),
	TP_ASSIGN(
		__entry->pid = ((process == NULL) ? PID_NONE : process_pid_of(process));
		__entry->new_gid = gid;
	),
	TP_PRINT("pid=%d new_gid=%d",
		__entry->pid, __entry->new_gid
	)
)

TRACE_EVENT(hkids, hkids_process_exec,
	TP_PROTO(struct thread_s *, thread, const char *, path, const char **, argv, const char **, envp),
	TP_ARGS(thread, path, argv, envp),
	TP_FIELD(
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->pid = ((thread == NULL) ? PID_NONE : thread_tid_of(thread));
	),
	TP_PRINT("pid=%d new_gid=%d",
		__entry->pid
	)
)

