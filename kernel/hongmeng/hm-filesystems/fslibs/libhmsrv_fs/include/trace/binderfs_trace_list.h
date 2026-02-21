/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: binderfs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 7 15:30:54 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

 TRACE_EVENT(binder, binder_get_referred_proc_start,
	TP_PROTO(pid_t, pid),
	TP_ARGS(pid),
	TP_FIELD(
		__field(unsigned int, pid)
	),
	TP_ASSIGN(
		__entry->pid = pid;
	),
	TP_PRINT("pid=%d", __entry->pid)
)

TRACE_EVENT(binder, binder_get_referred_proc_end,
	TP_PROTO(int, err),
	TP_ARGS(err),
	TP_FIELD(
		__field(int, err)
	),
	TP_ASSIGN(
		__entry->err = err;
	),
	TP_PRINT("ret=%d", __entry->err)
)

TRACE_EVENT(binder, binder_ioctl,
	TP_PROTO(unsigned int, cmd, unsigned long, arg),
	TP_ARGS(cmd, arg),
	TP_FIELD(
		__field(unsigned int, cmd)
		__field(unsigned long, arg)
	),
	TP_ASSIGN(
		__entry->cmd = cmd;
		__entry->arg = arg;
	),
	TP_PRINT("cmd=0x%x arg=0x%lx", __entry->cmd, __entry->arg)
)

TRACE_EVENT(binder, binder_transaction,
	TP_PROTO(int, debug_id, bool, reply, struct hmb_thread *, sender_thread,
		 struct hmb_node *, target_node, struct hmbproc_context *, target_ctx,
		 struct hmb_thread *, target_thread, unsigned int, flags, unsigned int, code),
	TP_ARGS(debug_id, reply, sender_thread, target_node, target_ctx, target_thread, flags, code),
	TP_FIELD(
		__field(int, debug_id)
		__field(int, target_node)
		__field(int, to_proc)
		__field(int, to_thread)
		__field(int, reply)
		__field(unsigned int, code)
		__field(unsigned int, flags)
	),
	TP_ASSIGN(
		__entry->debug_id = debug_id;
		__entry->target_node = target_node != NULL ? target_node->idx : 0;
		__entry->to_proc = target_ctx != NULL ? target_ctx->pid : 0;
		__entry->to_thread = target_thread != NULL ? target_thread->tid : 0;
		__entry->reply = reply;
		__entry->code = code;
		__entry->flags = flags;

		if (sender_thread != NULL) {
			__entry->th.common_pid = sender_thread->tid;
		}
	),
	TP_PRINT("transaction=%d dest_node=%d dest_proc=%d dest_thread=%d reply=%d flags=0x%x code=0x%x",
		 __entry->debug_id, __entry->target_node, __entry->to_proc, __entry->to_thread,
		 __entry->reply, __entry->flags, __entry->code
	)
)

TRACE_EVENT(binder, binder_transaction_received,
	TP_PROTO(int, debug_id, struct hmb_thread *, thread),
	TP_ARGS(debug_id, thread),
	TP_FIELD(
		__field(int, debug_id)
	),
	TP_ASSIGN(
		__entry->debug_id = debug_id;

		if (thread != NULL) {
			__entry->th.common_pid = thread->tid;
		}
	),
	TP_PRINT("transaction=%d", __entry->debug_id)
)

TRACE_EVENT(binder, binder_transaction_alloc_buf,
	TP_PROTO(struct hmb_buffer *, buffer),
	TP_ARGS(buffer),
	TP_FIELD(
		__field(int, debug_id)
		__field(size_t, data_size)
		__field(size_t, offsets_size)
		__field(size_t, extra_buffers_size)
	),
	TP_ASSIGN(
		__entry->debug_id = buffer->tr_idx;
		__entry->data_size = buffer->data_size;
		__entry->offsets_size = buffer->offsets_buffer_size;
		__entry->extra_buffers_size = buffer->extra_buffer_size;
	),
	TP_PRINT("transaction=%d data_size=%zd offsets_size=%zd extra_buffers_size=%zd",
		  __entry->debug_id, __entry->data_size, __entry->offsets_size,
		  __entry->extra_buffers_size)
)

TRACE_EVENT(binder, binder_transaction_buffer_release,
	TP_PROTO(struct hmb_buffer *, buffer),
	TP_ARGS(buffer),
	TP_FIELD(
		__field(int, debug_id)
		__field(size_t, data_size)
		__field(size_t, offsets_size)
		__field(size_t, extra_buffers_size)
	),
	TP_ASSIGN(
		__entry->debug_id = buffer->tr_idx;
		__entry->data_size = buffer->data_size;
		__entry->offsets_size = buffer->offsets_buffer_size;
		__entry->extra_buffers_size = buffer->extra_buffer_size;
	),
	TP_PRINT("transaction=%d data_size=%zd offsets_size=%zd extra_buffers_size=%zd",
		  __entry->debug_id, __entry->data_size, __entry->offsets_size,
		  __entry->extra_buffers_size)
)

TRACE_EVENT(binder, binder_transaction_failed_buffer_release,
	TP_PROTO(struct hmb_buffer *, buffer),
	TP_ARGS(buffer),
	TP_FIELD(
		__field(int, debug_id)
		__field(size_t, data_size)
		__field(size_t, offsets_size)
		__field(size_t, extra_buffers_size)
	),
	TP_ASSIGN(
		__entry->debug_id = buffer->tr_idx;
		__entry->data_size = buffer->data_size;
		__entry->offsets_size = buffer->offsets_buffer_size;
		__entry->extra_buffers_size = buffer->extra_buffer_size;
	),
	TP_PRINT("transaction=%d data_size=%zd offsets_size=%zd extra_buffers_size=%zd",
		  __entry->debug_id, __entry->data_size, __entry->offsets_size,
		  __entry->extra_buffers_size)
)

TRACE_EVENT(binder, binder_ioctl_done,
	TP_PROTO(int, err),
	TP_ARGS(err),
	TP_FIELD(
		__field(int, err)
	),
	TP_ASSIGN(
		__entry->err = err;
	),
	TP_PRINT("ret=%d", __entry->err)
)

TRACE_EVENT(binder, binder_write_done,
	TP_PROTO(int, err),
	TP_ARGS(err),
	TP_FIELD(
		__field(int, err)
	),
	TP_ASSIGN(
		__entry->err = err;
	),
	TP_PRINT("ret=%d", __entry->err)
)

TRACE_EVENT(binder, binder_read_done,
	TP_PROTO(int, err),
	TP_ARGS(err),
	TP_FIELD(
		__field(int, err)
	),
	TP_ASSIGN(
		__entry->err = err;
	),
	TP_PRINT("ret=%d", __entry->err)
)

TRACE_EVENT(binder, binder_wait_for_work,
	TP_PROTO(bool, proc_work, bool, transaction_stack, bool, thread_todo),
	TP_ARGS(proc_work, transaction_stack, thread_todo),
	TP_FIELD(
		__field(bool, proc_work)
		__field(bool, transaction_stack)
		__field(bool, thread_todo)
	),
	TP_ASSIGN(
		__entry->proc_work = proc_work;
		__entry->transaction_stack = transaction_stack;
		__entry->thread_todo = thread_todo;
	),
	TP_PRINT("proc_work=%d transaction_stack=%d thread_todo=%d",
		  __entry->proc_work, __entry->transaction_stack,
		  __entry->thread_todo)
)

TRACE_EVENT(binder, binder_read_work_type,
	TP_PROTO(struct hmb_work *, work),
	TP_ARGS(work),
	TP_FIELD(
		__field(int, work_type)
		__field(int, index)
	),
	TP_ASSIGN(
		__entry->work_type = work->type;
		__entry->index = work->type == HMB_WORK_TRANSACTION ? container_of(work, struct hmb_transac, work)->idx : 0;
	),
	TP_PRINT("work_type=%d, idx=%d", __entry->work_type, __entry->index)
)

TRACE_EVENT(binder, binder_wakeup,
	TP_PROTO(int, idx, int, sender_tid, int, target_tid),
	TP_ARGS(idx, sender_tid, target_tid),
	TP_FIELD(
		__field(int, idx)
		__field(int, sender_tid)
		__field(int, target_tid)
	),
	TP_ASSIGN(
		__entry->idx = idx;
		__entry->sender_tid = sender_tid;
		__entry->target_tid = target_tid;
	),
	TP_PRINT("idx=%d, sender_tid=%d, target_tid=%d", __entry->idx, __entry->sender_tid, __entry->target_tid)
)

TRACE_EVENT(binder, binder_delete_death_notification,
	TP_PROTO(int, idx, int, tid, int, desc),
	TP_ARGS(idx, tid, desc),
	TP_FIELD(
		__field(int, idx)
		__field(int, tid)
		__field(int, desc)
	),
	TP_ASSIGN(
		__entry->idx = idx;
		__entry->tid = tid;
		__entry->desc = desc;
	),
	TP_PRINT("delete death notification, idx=%d, tid=%d, desc=%d", __entry->idx, __entry->tid, __entry->desc)
)

TRACE_EVENT(binder, binder_received_death_notify,
	TP_PROTO(int, pid, int, tid, unsigned long, cookie, bool, flag),
	TP_ARGS(pid, tid, cookie, flag),
	TP_FIELD(
		__field(int, pid)
		__field(int, tid)
		__field(unsigned long, cookie)
		__field(bool, flag)
	),
	TP_ASSIGN(
		__entry->pid = pid;
		__entry->tid = tid;
		__entry->cookie = cookie;
		__entry->flag = flag;
	),
	TP_PRINT("recive death notify, pid=%d, tid=%d, cookie=%lx, is_dead_binder=%d",
		__entry->pid, __entry->tid, __entry->cookie, __entry->flag)
)

TRACE_EVENT(binder, binder_notify_death,
	TP_PROTO(int, pid, unsigned long, cookie),
	TP_ARGS(pid, cookie),
	TP_FIELD(
		__field(int, pid)
		__field(unsigned long, cookie)
	),
	TP_ASSIGN(
		__entry->pid = pid;
		__entry->cookie = cookie;
	),
	TP_PRINT("notify death, pid=%d, cookie=%lx", __entry->pid, __entry->cookie)
)

TRACE_EVENT(binder, binder_request_death_notification,
	TP_PROTO(int, pid, unsigned long, cookie),
	TP_ARGS(pid, cookie),
	TP_FIELD(
		__field(int, pid)
		__field(unsigned long, cookie)
	),
	TP_ASSIGN(
		__entry->pid = pid;
		__entry->cookie = cookie;
	),
	TP_PRINT("request death notification, pid=%d, cookie=%lx", __entry->pid, __entry->cookie)
)

TRACE_EVENT(binder, binder_clear_death_notification,
	TP_PROTO(int, pid, unsigned long, cookie),
	TP_ARGS(pid, cookie),
	TP_FIELD(
		__field(int, pid)
		__field(unsigned long, cookie)
	),
	TP_ASSIGN(
		__entry->pid = pid;
		__entry->cookie = cookie;
	),
	TP_PRINT("clear death notification, pid=%d, cookie=%lx", __entry->pid, __entry->cookie)
)
