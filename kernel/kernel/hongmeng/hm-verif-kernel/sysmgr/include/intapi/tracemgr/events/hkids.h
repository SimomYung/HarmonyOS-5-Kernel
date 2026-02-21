/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: trace list for hkids
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 21 04:16:58 2025
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(hkids, hkids_ptrace,
	TP_PROTO(struct thread_s *, debugger_thread, struct ptrace_arg_s *, arg, pid_t, debugger_pid),
	TP_ARGS(debugger_thread, arg, debugger_pid),
	TP_FIELD(
		__field(pid_t, debugger_pid)
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->debugger_pid = debugger_pid;
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

TRACE_EVENT(hkids, hkids_process_excep_dump,
	TP_PROTO(struct process_s *, process, const struct __siginfo *, si, struct thread_s *, thread),
	TP_ARGS(process, si, thread),
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

TRACE_EVENT(hkids, hkids_process_exec_action,
	TP_PROTO(struct thread_s *, thread, const char *, path, const char **, argv, const char **, envp,
		pid_t, pid, int *, retval),
	TP_ARGS(thread, path, argv, envp, pid, retval),
	TP_FIELD(
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->pid = pid;
	),
	TP_PRINT("pid=%d",
		__entry->pid
	)
)

TRACE_EVENT(hkids, hkids_process_exec_event,
	TP_PROTO(struct thread_s *, thread, const char *, path, const char **, argv, const char **, envp,
		pid_t, pid),
	TP_ARGS(thread, path, argv, envp, pid),
	TP_FIELD(
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->pid = pid;
	),
	TP_PRINT("pid=%d",
		__entry->pid
	)
)

TRACE_EVENT(hkids, hkids_process_mmap_hap,
	TP_PROTO(struct file *, file),
	TP_ARGS(file),
	TP_FIELD(
		__field(pid_t, pid)
	),
	TP_ASSIGN(
		__entry->pid = PID_NONE;
	),
	TP_PRINT("pid=%d",
		__entry->pid
	)
)
