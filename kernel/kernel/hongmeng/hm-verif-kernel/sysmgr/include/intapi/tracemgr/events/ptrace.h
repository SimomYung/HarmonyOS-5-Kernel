/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: trace list for ptrace
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 21 04:16:58 2025
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

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
		 const struct thread_s *, thread, bool, is_seize, int, ret_val,
		 pid_t, tracer_pid, pid_t, tracee_pid),
	TP_ARGS(debugger_thread, thread, is_seize, ret_val, tracer_pid, tracee_pid),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(bool, is_seize)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = tracer_pid;
		__entry->tracee_pid = tracee_pid;
		__entry->is_seize = is_seize;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d is_seize=%d return %d",
		__entry->tracer_pid, __entry->tracee_pid, __entry->is_seize, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, detach_debugger,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread,
		 pid_t, tracee_pid),
	TP_ARGS(debugger, thread, tracee_pid),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = tracee_pid;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d",
		__entry->tracer_pid, __entry->tracee_pid
	)
)

TRACE_EVENT(ptrace, handle_debug_act,
	TP_PROTO(const struct process_s *, debugger, const struct thread_s *, thread,
		 enum pdebug_act_type, act_type, bool, enqueue_head, int, ret_val,
		 pid_t, tracee_pid),
	TP_ARGS(debugger, thread, act_type, enqueue_head, ret_val, tracee_pid),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(int, act_type)
		__field(int, enqueue_head)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = tracee_pid;
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
		 int, ret_val, pid_t, tracee_pid),
	TP_ARGS(debugger, thread, act, act_type, ret_val, tracee_pid),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(int, act_type)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = tracee_pid;
		__entry->act_type = act_type;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d act_type=%d return %d",
		__entry->tracer_pid, __entry->tracee_pid, __entry->act_type, __entry->ret_val
	)
)

TRACE_EVENT(ptrace, process_debug_act,
	TP_PROTO(const struct process_s *, debugger, const struct pdebug_act *, act,
		 int, ret_val),
	TP_ARGS(debugger, act, ret_val),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(int, act_type)
		__field(int, ret_val)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->act_type = act->type;
		__entry->ret_val = ret_val;
	),
	TP_PRINT("tracer_pid=%d act_type=%d return %d",
		__entry->tracer_pid, __entry->act_type, __entry->ret_val
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
		 const struct __siginfo *, siginfo, pid_t, tracee_pid),
	TP_ARGS(debugger, thread, siginfo, tracee_pid),
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
		__entry->tracee_pid = tracee_pid;
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
		 pid_t, wait_pid, const struct pdebug_event *, event, pid_t, tracee_pid),
	TP_ARGS(debugger, thread, wait_pid, event, tracee_pid),
	TP_FIELD(
		__field(pid_t, tracer_pid)
		__field(pid_t, tracee_pid)
		__field(pid_t, wait_pid)
		__field(int, event_type)
	),
	TP_ASSIGN(
		__entry->tracer_pid = process_pid_of(debugger);
		__entry->tracee_pid = tracee_pid;
		__entry->wait_pid = wait_pid;
		__entry->event_type = event->type;
	),
	TP_PRINT("tracer_pid=%d tracee_pid=%d wait_pid=%d event_type=%d",
		 __entry->tracer_pid, __entry->tracee_pid, __entry->wait_pid,
		 __entry->event_type
	)
)
