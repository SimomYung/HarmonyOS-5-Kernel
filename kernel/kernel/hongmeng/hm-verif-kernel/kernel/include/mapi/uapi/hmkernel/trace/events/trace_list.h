/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Kernel tracepoints
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 01 17:20:17 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */
#include "syscall_trace_list.h"
#include "irq_trace_list.h"

TRACE_TCB_EVENT_HOT(sched, sched_switch,
	TP_PROTO(struct trace_tcb_state_s *, prev_state, struct tcb_s *, next_tcb),
	TP_ARGS(prev_state, next_tcb),
	TP_FIELD(
		__array(char, pname, 16)
		__field(int, prev_tid)
		__field(int, pprio)
		__field(long, pstate)
		__array(char, nname, 16)
		__field(int, next_tid)
		__field(int, nprio)
		__array(unsigned char, ninfo, 8)
	),
	TP_ASSIGN(
		if (prev_state->state == TCB_STATE_INVALID) {
			str_cpy_p_to_a(__entry->pname, "NULL");
			__entry->pprio = 0;
			__entry->prev_tid = 0;
		} else {
			str_cpy_p_to_a(__entry->pname, prev_state->name);
			__entry->pprio = prev_state->prio;
			/*
			 * tppmgr is the idle process of hm but its tid is not zero. To modify its display in
			 * smartperf, we should set it to zero manually.
			 */
			__entry->prev_tid = prev_state->is_idle ? 0 : prev_state->tid;
		}
		str_cpy_p_to_a(__entry->nname, tcb_name_of(next_tcb));
		__entry->pstate = prev_state->state;
		__entry->nprio = sched_tcb_get_prio(next_tcb);
		if (tcb_is_prev_idle(next_tcb)) {
			__entry->next_tid = 0;
		} else {
			__entry->next_tid = *(ptr_cast(tcb_miscdata_of(next_tcb, MISCDATA_TCB_TID), int));
		}
		mem_zero_s(__entry->ninfo);
		sched_tcb_pack_info(next_tcb, __entry->ninfo, sizeof(__entry->ninfo));
	),
	TP_PRINT(
		"prev_comm=%s prev_pid=%d prev_prio=%d prev_state=%s"
		" ==> next_comm=%s next_pid=%d next_prio=%d next_info=%s",
		__entry->pname, __entry->prev_tid, __entry->pprio, hm_trace_tcb_state2str(__entry->pstate),
		__entry->nname, __entry->next_tid, __entry->nprio,
		hm_trace_tcb_unpack_schedinfo(__entry->ninfo, sizeof(__entry->ninfo))
	)
)

TRACE_TCB_EVENT(sched, sched_mark_restricted,
	TP_PROTO(struct tcb_s *, tcb, bool, mark, bool, user, bool, mode),
	TP_ARGS(tcb, mark, user, mode),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(int, mark)
		__field(int, user)
		__field(int, mode)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->mark = mark ? 1 : 0;
		__entry->user = user ? 1 : 0;
		__entry->mode = mode ? 1 : 0;
	),
	TP_PRINT(
		"tid=%d comm=%s prio=%d %s mark=%d user=%d",
		__entry->tid, __entry->name, __entry->prio, __entry->mode ? "supress" : "update_time", __entry->mark, __entry->user
	)
)

TRACE_EVENT(sched, hmlt_update_pcpu_raw_load,
	TP_PROTO(unsigned int, cpu,
		__u64, curr_raw_load,
		__u64, prev_raw_load),
	TP_ARGS(cpu, curr_raw_load, prev_raw_load),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__u64, curr_raw_load)
		__field(__u64, prev_raw_load)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->curr_raw_load = curr_raw_load;
		__entry->prev_raw_load = prev_raw_load;),
	TP_PRINT("cpu=%u curr_load=%llu prev_load=%llu\n",
		 __entry->cpu, __entry->curr_raw_load, __entry->prev_raw_load))

TRACE_EVENT(sched, sched_rotation,
	TP_PROTO(unsigned int, src_cpu,  unsigned int, dst_cpu),
	TP_ARGS(src_cpu, dst_cpu),
	TP_FIELD(
		__field(__u32, src_cpu)
		__field(__u32, dst_cpu)
	),
	TP_ASSIGN(
		__entry->src_cpu = src_cpu;
		__entry->dst_cpu = dst_cpu;
	),
	TP_PRINT("src_cpu=%u dst_cpu=%u", __entry->src_cpu, __entry->dst_cpu)
)

TRACE_EVENT(sched, rotation_checkpoint,
	TP_PROTO(bool, enabled, __s64, sum_exec, __u64, time_diff),
	TP_ARGS(enabled, sum_exec, time_diff),
	TP_FIELD(
		__field(bool, enabled)
		__field(__s64, sum_exec)
		__field(__u64, time_diff)
	),
	TP_ASSIGN(
		__entry->enabled = enabled;
		__entry->sum_exec = sum_exec;
		__entry->time_diff = time_diff;
	),
	TP_PRINT(
		"enabled=%d sum_exec=%lld time_diff=%llu",
		__entry->enabled, __entry->sum_exec, __entry->time_diff
	)
)

TRACE_EVENT(sched, sched_ui_critical_section,
	TP_PROTO(bool, begin, int, nr_ui),
	TP_ARGS(begin, nr_ui),
	TP_FIELD(
		__field(bool, begin)
		__field(__s32, nr_ui)
	),
	TP_ASSIGN(
		__entry->begin = begin;
		__entry->nr_ui = nr_ui;
	),
	TP_PRINT("ui_critical_section=%d nr_ui=%d", __entry->begin, __entry->nr_ui)
)

TRACE_TCB_EVENT(sched, sched_switch_ninfo,
	TP_PROTO(struct tcb_s *, next_tcb),
	TP_ARGS(next_tcb),
	TP_FIELD(
		__array(char, ninfo, 48)
	),
	TP_ASSIGN(
		mem_zero_s(__entry->ninfo);
		sched_tcb_get_info(next_tcb, __entry->ninfo, sizeof(__entry->ninfo));
	),
	TP_PRINT("%s", __entry->ninfo)
)

TRACE_TCB_EVENT(sched, sched_blocked_reason,
	TP_PROTO(
		 struct tcb_s *, tcb,
		 enum tcb_block_reason, reason,
		 __u64, delta),
	TP_ARGS(tcb, reason, delta),
	TP_FIELD(
		__field(int, pid)
		__field(void *, caller)
		__field(bool, iowait)
		__field(__u64, delay)
		__field(unsigned long, offset)
		__field(unsigned long, size)
		__array(char, func_name, SYM_FUNC_NAME_SIZE)
		__array(char, mod_name, SYM_MOD_NAME_SIZE)
	),
	TP_ASSIGN(
		  __u64 freq = (__u64)vtimekeep_freq();
		  __entry->delay = ((delta / freq) * __NSEC_PER_SEC) + ((delta % freq) * __NSEC_PER_SEC / freq);
		  __entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		  trace_set_block_info(tcb, reason, __entry->mod_name, __entry->func_name,
				       &__entry->offset, &__entry->size, &__entry->iowait);
	),
	TP_PRINT(
		"pid=%d iowait=%d caller=%s+0x%lx/0x%lx[%s] delay=%llu",
		__entry->pid, __entry->iowait,
		__entry->func_name, __entry->offset, __entry->size, __entry->mod_name,
		__entry->delay >> 10
	)
)

#define DEFINE_WAKEUP_EVENT(FUNC)									\
	TRACE_TCB_EVENT(sched, FUNC,									\
		TP_PROTO(struct tcb_s *, tcb),								\
		TP_ARGS(tcb),										\
		TP_FIELD(										\
			__array(char, pname, 16)							\
			__field(int, pid)								\
			__field(int, prio)								\
			__field(int, success)								\
			__field(int, target_cpu)							\
		),											\
		TP_ASSIGN(										\
			str_cpy_p_to_a(__entry->pname, tcb_name_of(tcb));				\
			__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));	\
			__entry->prio = sched_tcb_get_prio(tcb);					\
			__entry->success = 1; /* rudiment, kill when possible */			\
			__entry->target_cpu = (int)tcb_cpu_of(tcb);					\
		),											\
		TP_PRINT(										\
			"comm=%s pid=%d prio=%d target_cpu=%03d",					\
			 __entry->pname, __entry->pid, __entry->prio,					\
			 __entry->target_cpu)								\
	)

DEFINE_WAKEUP_EVENT(sched_wakeup)
DEFINE_WAKEUP_EVENT(sched_waking)
DEFINE_WAKEUP_EVENT(sched_wakeup_new)

TRACE_TCB_EVENT(sched, hmlt_update_tcb_load,
	TP_PROTO(const struct tcb_s *, tcb,
		unsigned int, cpu,
		unsigned int, capacity,
		unsigned long long, runtime_us,
		unsigned long long, runtime,
		unsigned short int, curr_load,
		unsigned short int, prev_load,
		unsigned short int, toptask_index,
		unsigned short int *, hists,
		unsigned short int *, top_load_hists),
	TP_ARGS(tcb, cpu, capacity, runtime_us, runtime, curr_load, prev_load, toptask_index, hists, top_load_hists),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u32, cpu)
		__field(__u32, curr_cap)
		__field(__u64, runtime_us)
		__field(__u64, runtime)
		__field(__u16, curr_load)
		__field(__u16, prev_load)
		__field(__u16, toptask_index)
		__array(__u16, hists, 5)
		__array(__u16, top_load_hists, 5)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of((struct tcb_s *)tcb, MISCDATA_TCB_TID), __s32));
		__entry->cpu = cpu;
		__entry->curr_cap = capacity;
		__entry->runtime_us = runtime_us;
		__entry->runtime = runtime;
		__entry->curr_load = curr_load;
		__entry->prev_load = prev_load;
		__entry->toptask_index = toptask_index;
		(void)memcpy(__entry->hists, hists, sizeof(__entry->hists));
		(void)memcpy(__entry->top_load_hists, top_load_hists, sizeof(__entry->top_load_hists));
	),
	TP_PRINT("tid=%d comm=%s runtime_us=%"__PRIu64" runtime=%"__PRIu64" "
		 "curr_capacity=%u curr_load=%u prev_load=%u cpu = %u "
		 "hist0=%u hist1=%u hist2=%u hist3=%u hist4=%u "
		 "top hist0=%u hist1=%u hist2=%u hist3=%u hist4=%u idx=%u\n",
		 __entry->tid, __entry->name, __entry->runtime_us, __entry->runtime, __entry->curr_cap,
		 __entry->curr_load, __entry->prev_load, __entry->cpu, __entry->hists[0], __entry->hists[1],
		 __entry->hists[2], __entry->hists[3], __entry->hists[4], __entry->top_load_hists[0],
		 __entry->top_load_hists[1], __entry->top_load_hists[2], __entry->top_load_hists[3],
		 __entry->top_load_hists[4], __entry->toptask_index
	)
)

TRACE_TCB_EVENT(sched, hmlt_update_tcb_load_in_cpu,
	TP_PROTO(const struct tcb_s *, tcb,
		unsigned int, cpu,
		unsigned int, curr_run,
		unsigned int, prev_run,
		unsigned short int *, curr_in_cpu,
		unsigned short int *, prev_in_cpu),
	TP_ARGS(tcb, cpu, curr_run, prev_run, curr_in_cpu, prev_in_cpu),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u32, cpu)
		__field(__u32, curr_run)
		__field(__u32, prev_run)
		__array(__u16, curr_in_cpu, 12)
		__array(__u16, prev_in_cpu, 12)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of((struct tcb_s *)tcb, MISCDATA_TCB_TID), __s32));
		__entry->cpu = cpu;
		__entry->curr_run = curr_run;
		__entry->prev_run = prev_run;
		(void)memcpy(__entry->curr_in_cpu, curr_in_cpu, sizeof(__entry->curr_in_cpu));
		(void)memcpy(__entry->prev_in_cpu, prev_in_cpu, sizeof(__entry->prev_in_cpu));
	),
	TP_PRINT("tid=%d comm=%s cpu=%u curr_run=%u prev_run=%u "
		 "curr=[%u %u %u %u %u %u %u %u %u %u %u %u] "
		 "prev=[%u %u %u %u %u %u %u %u %u %u %u %u]\n",
		 __entry->tid, __entry->name, __entry->cpu, __entry->curr_run, __entry->prev_run,
		 __entry->curr_in_cpu[0], __entry->curr_in_cpu[1], __entry->curr_in_cpu[2], __entry->curr_in_cpu[3],
		 __entry->curr_in_cpu[4], __entry->curr_in_cpu[5], __entry->curr_in_cpu[6], __entry->curr_in_cpu[7],
		 __entry->curr_in_cpu[8], __entry->curr_in_cpu[9], __entry->curr_in_cpu[10], __entry->curr_in_cpu[11],
		 __entry->prev_in_cpu[0], __entry->prev_in_cpu[1], __entry->prev_in_cpu[2], __entry->prev_in_cpu[3],
		 __entry->prev_in_cpu[4], __entry->prev_in_cpu[5], __entry->prev_in_cpu[6], __entry->prev_in_cpu[7],
		 __entry->prev_in_cpu[8], __entry->prev_in_cpu[9], __entry->prev_in_cpu[10], __entry->prev_in_cpu[11]
	)
)

TRACE_TCB_EVENT(sched, hmlt_update_cpu_load,
	TP_PROTO(unsigned int, cpu,
		unsigned int, capacity,
		unsigned long long, curr_time_us,
		unsigned int, curr_load,
		unsigned int, prev_load),
	TP_ARGS(cpu, capacity, curr_time_us, curr_load, prev_load),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__u32, curr_cap)
		__field(__u64, curr_time_us)
		__field(__u32, curr_load)
		__field(__u32, prev_load)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->curr_cap = capacity;
		__entry->curr_time_us = curr_time_us;
		__entry->curr_load = curr_load;
		__entry->prev_load = prev_load;),
	TP_PRINT("cpu=%u curr_time_us=%"__PRIu64" curr_capacity=%u curr_load=%u prev_load=%u\n",
		 __entry->cpu, __entry->curr_time_us, __entry->curr_cap, __entry->curr_load, __entry->prev_load))

TRACE_TCB_EVENT(sched, sched_boost_task,
	TP_PROTO(const struct tcb_s *, tcb,
		 int, boost,
		 unsigned int, load,
		 unsigned int, min_load,
		 unsigned int, max_load),
	TP_ARGS(tcb, boost, load, min_load, max_load),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u32, load)
		__field(__u32, boost)
		__field(__u32, min_load)
		__field(__u32, max_load)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of((struct tcb_s *)tcb, MISCDATA_TCB_TID), __s32));
		__entry->load = load;
		__entry->boost = (__u32)boost;
		__entry->min_load = min_load;
		__entry->max_load = max_load;),
	TP_PRINT("comm=%s tid=%d load=%lu boost=%d min_load=%lu max_load=%lu\n",
		 __entry->name, __entry->tid, __entry->load,
		 __entry->boost, __entry->min_load, __entry->max_load)
)

TRACE_TCB_EVENT(sched, hmlt_update_pcpu_load,
	TP_PROTO(unsigned int, cpu,
		unsigned int, core,
		unsigned int, capacity,
		unsigned int, nr_running_cpu,
		unsigned long long, curr_time_us,
		unsigned int, curr_load,
		unsigned int, prev_load),
	TP_ARGS(cpu, core, capacity, nr_running_cpu, curr_time_us, curr_load, prev_load),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__u32, core)
		__field(__u32, curr_cap)
		__field(__u32, nr_running_cpu)
		__field(__u64, curr_time_us)
		__field(__u32, curr_load)
		__field(__u32, prev_load)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->core = core;
		__entry->curr_cap = capacity;
		__entry->nr_running_cpu = nr_running_cpu;
		__entry->curr_time_us = curr_time_us;
		__entry->curr_load = curr_load;
		__entry->prev_load = prev_load;),
	TP_PRINT("cpu=%u core=%u curr_capacity=%u nr_running_cpu=%u curr_time_us=%"__PRIu64" curr_load=%u prev_load=%u\n",
		 __entry->cpu, __entry->core, __entry->curr_cap, __entry->nr_running_cpu,
		 __entry->curr_time_us, __entry->curr_load, __entry->prev_load))

TRACE_EVENT(ipi, ipi_raise,
	TP_PROTO(__u32, target, unsigned int, reason_id),
	TP_ARGS(target, reason_id),
	TP_FIELD(
		__field(unsigned long, target)
		__field(const char *, reason)
	),
	TP_ASSIGN(
		__entry->target = target;
		__entry->reason = (const char *)(uintptr_t)reason_id;
	),
	TP_PRINT("target_mask=0x%x (%s)", __entry->target, hmtrace_tp_string_of((unsigned int)__entry->reason))
)

#define DEFINE_IPI_EVENT(FUNC)							\
	TRACE_EVENT(ipi, FUNC,							\
		TP_PROTO(unsigned int, reason_id),					\
		TP_ARGS(reason_id),						\
		TP_FIELD(							\
			__field(const char *, reason)				\
		),								\
		TP_ASSIGN(							\
			__entry->reason = (const char *)(uintptr_t)reason_id;	\
		),								\
		TP_PRINT("(%s)", hmtrace_tp_string_of((unsigned int)__entry->reason))				\
	)

DEFINE_IPI_EVENT(ipi_entry)
DEFINE_IPI_EVENT(ipi_exit)

TRACE_TCB_EVENT(sched, sched_pi_setprio,
	TP_PROTO(struct tcb_s *, tcb, struct tcb_s *, pi_tcb),
	TP_ARGS(tcb, pi_tcb),
	TP_FIELD(
		__array(char, pname, 16)
		__field(int, pid)
		__field(int, oldprio)
		__field(int, newprio)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->pname, tcb_name_of(tcb));
		__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->oldprio = sched_tcb_get_prio(tcb);
		__entry->newprio = pi_tcb ? min(sched_tcb_get_prio(tcb), sched_tcb_get_prio(pi_tcb)) :
				sched_tcb_get_prio(tcb);
	),
	TP_PRINT("comm=%s pid=%d oldprio=%d newprio=%d",
		__entry->pname, __entry->pid,
		__entry->oldprio, __entry->newprio
	)
)

TRACE_EVENT(sched, relative_slack_ns,
	TP_PROTO(__s64, time, int, id, unsigned int, pct, __u64, slack),
	TP_ARGS(time, id, pct, slack),
	TP_FIELD(
		__field(long, time)
		__field(int, id)
		__field(unsigned int, pct)
		__field(long, slack)
	),
	TP_ASSIGN(
		__entry->time = time;
		__entry->id = id;
		__entry->pct = pct;
		__entry->slack = (long)slack;
	),
	TP_PRINT("request_time=%ldms task_css_id=%d pct=%d%% slack=%ldms",
		__entry->time, __entry->id, __entry->pct, __entry->slack
	)
)

TRACE_TCB_EVENT(sched, debug_softexpires_next,
	TP_PROTO(unsigned int, cpu, __u64, now, __u64, soft, __u64, hard),
	TP_ARGS(cpu, now, soft, hard),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(unsigned long, now)
		__field(unsigned long, soft)
		__field(unsigned long, hard)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->now = now;
		__entry->soft = soft;
		__entry->hard = hard;
	),
	TP_PRINT("cpu=%u now=%lu(us) softexp=%lu exp=%lu",
		__entry->cpu, __entry->now, __entry->soft, __entry->hard
	)
)

TRACE_TCB_EVENT(sched, sched_cpu_hotplug,
	TP_PROTO(__u32, affected_cpu, __s32, error, __s32, status),
	TP_ARGS(affected_cpu, error, status),
	TP_FIELD(
		__field(__u32, affected_cpu)
		__field(__s32, error)
		__field(__s32, status)
	),
	TP_ASSIGN(__entry->affected_cpu = affected_cpu;
		__entry->error = error;
		__entry->status = status;
	),
	TP_PRINT("cpu %u %s error=%d", __entry->affected_cpu,
		(__entry->status == 1) ? "online" : "offline", __entry->error
	)
)

TRACE_TCB_EVENT(sched, idle_slack,
	TP_PROTO(__s32, id, __u64, value),
	TP_ARGS(id, value),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, pid)
		__field(__u32, id)
		__field(__u64, value)
		__array(char, name, 16)
	),
	TP_ASSIGN(struct tcb_s *tcb = tcb_current();
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_PID), __s32));
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->id = (__u32)(id != -1) ? id : 0;
		__entry->value = value;
	),
	TP_PRINT("idle_slack_%d state=%llu cpu_id=0 comm=%s tid=%d pid=%d",
		__entry->id, __entry->value, __entry->name, __entry->tid, __entry->pid
	)
)

TRACE_EVENT(sched, deadline_group,
	TP_PROTO(const char *, str, __s32, id, __u64, state),
	TP_ARGS(str, id, state),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, pid)
		__field(__s32, id)
		__field(__u64, state)
		__array(char, name, 16)
		__string(str, str)
	),
	TP_ASSIGN(struct tcb_s *tcb = tcb_current();
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_PID), __s32));
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__assign_str(str, str);
		__entry->id = id != -1 ? id : 0;
		__entry->state = state;
	),
	TP_PRINT("%s_%d state=%llu cpu_id=0 comm=%s tid=%d pid=%d",
		__get_str(str), __entry->id, __entry->state, __entry->name, __entry->tid, __entry->pid
	)
)

TRACE_TCB_EVENT(sched, sched_choose_cpu,
	TP_PROTO(struct tcb_s *, tcb, bool, prefer_idle, unsigned int, load, int, boost, unsigned int, boost_flag),
	TP_ARGS(tcb, prefer_idle, load, boost, boost_flag),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, prio)
		__field(__u32, sched_flags)
		__field(__u32, target_cpu)
		__field(__u32, prefer_idle)
		__field(__u64, cref)
		__field(__u32, load)
		__field(__u32, boost)
		__field(__u32, boost_flag)
		__array(char, name, 16)
	),
	TP_ASSIGN(str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->cref = tcb_cref_of(tcb);
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->sched_flags = tcb_schedflags_of(tcb);
		__entry->target_cpu = tcb_cpu_of(tcb);
		__entry->prefer_idle = (__u32)prefer_idle;
		__entry->load = load;
		__entry->boost = boost;
		__entry->boost_flag = boost_flag;
	),
	TP_PRINT("tcb=%s load=%lu boost=%d cref=%llx tid=%d prio=%d sched_flags=0x%x "
		"prefer_idle=%u boost_flag=%u select cpu:%u",
		__entry->name, __entry->load, __entry->boost, __entry->cref, __entry->tid,
		__entry->prio, __entry->sched_flags,
		__entry->prefer_idle, __entry->boost_flag, __entry->target_cpu
	)
)

TRACE_TCB_EVENT(sched, sched_prefer_cookie_choose_cpumask,
	TP_PROTO(struct tcb_s *, tcb, u64, prefer_mask, u64, allowed_mask, u64, wanted_mask),
	TP_ARGS(tcb, prefer_mask, allowed_mask, wanted_mask),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, prio)
		__field(__u64, prefer_mask)
		__field(__u64, allowed_mask)
		__field(__u64, wanted_mask)
		__field(__u64, prefer_cookie)
		__array(char, name, 16)
	),
	TP_ASSIGN(str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->prefer_cookie = tcb_prefer_cookie_of(tcb);
		__entry->prefer_mask = prefer_mask;
		__entry->allowed_mask = allowed_mask;
		__entry->wanted_mask = wanted_mask;
	),
	TP_PRINT("tcb=%s tid=%d prio=%d prefer_cookie=0x%x prefer_mask=0x%x "
		"allowed_mask=0x%x wanted_mask=0x%x",
		__entry->name, __entry->tid, __entry->prio, __entry->prefer_cookie,
		__entry->prefer_mask, __entry->allowed_mask, __entry->wanted_mask
	)
)

TRACE_TCB_EVENT(sched, sched_smt_force_expeller,
	TP_PROTO(struct tcb_s *, tcb, unsigned int, cpu, unsigned int, type),
	TP_ARGS(tcb, cpu, type),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, prio)
		__field(__u32, cpu)
		__field(__u32, type)
		__array(char, name, 16)
	),
	TP_ASSIGN(str_cpy_p_to_a(__entry->name, tcb != NULL ? tcb_name_of(tcb) : "NULL");
		__entry->prio = tcb ? sched_tcb_get_prio(tcb) : (-1);
		__entry->tid = tcb ? (*(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32))) : (-1);
		__entry->cpu = cpu;
		__entry->type = type;
	),
	TP_PRINT("tcb=%s tid=%d prio=%d cpu=%u type=%u",
		__entry->name, __entry->tid, __entry->prio, __entry->cpu, __entry->type
	)
)

TRACE_TCB_EVENT(sched, sched_set_scheduler,
	TP_PROTO(const struct tcb_s *, tcb,
		int, old_policy,
		int, old_sched_priority,
		int, old_ices_priority,
		unsigned int, reason),
	TP_ARGS(tcb, old_policy, old_sched_priority, old_ices_priority, reason),
	TP_FIELD(
		__field(__s32, src_tid)
		__field(__s32, dst_tid)
		__field(__s32, old_policy)
		__field(__s32, old_sched_priority)
		__field(__s32, old_ices_priority)
		__field(__s32, new_policy)
		__field(__u32, new_sched_priority)
		__field(__u32, new_ices_priority)
		__field(__u32, reason)
	),
	TP_ASSIGN(struct tcb_sched_s *ts = tcb_sched_mdata_of(tcb, __comm);
		__entry->src_tid = *(ptr_cast(tcb_miscdata_of(tcb_current(), MISCDATA_TCB_TID), __s32));
		__entry->dst_tid = *(ptr_cast(tcb_miscdata_of((struct tcb_s *)tcb, MISCDATA_TCB_TID), __s32));
		__entry->old_policy = old_policy;
		__entry->old_sched_priority = old_sched_priority;
		__entry->old_ices_priority = old_ices_priority;
		__entry->new_policy = ts->sched_policy;
		__entry->new_sched_priority = ts->sched_priority;
		__entry->new_ices_priority = ts->ices_priority;
		__entry->reason = reason;
	),
	TP_PRINT("[src:%d dst:%d] old_policy=%d old_prio=%d old_user_prio=%d--> "
		"new_policy=%d new_prio=%u new_user_prio=%u reason=%u",
		__entry->src_tid, __entry->dst_tid, __entry->old_policy, __entry->old_sched_priority,
		__entry->old_ices_priority, __entry->new_policy, __entry->new_sched_priority,
		__entry->new_ices_priority, __entry->reason
	)
)

TRACE_TCB_EVENT(sched, core_sched_check_match,
	TP_PROTO(unsigned int, cpu,
		struct tcb_s *, tcb,
		struct tcb_s *, sibling,
		int, stat,
		unsigned int, lcpu,
		unsigned int, pcpu,
		__u32, need_resched_mask,
		bool, need_mark),
	TP_ARGS(cpu, tcb, sibling, stat, lcpu, pcpu, need_resched_mask, need_mark),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__s32, tid)
		__field(__u64, cookie)
		__field(__s32, prio)
		__array(char, name, 32)
		__field(__s32, stid)
		__field(__u64, scookie)
		__field(__s32, sprio)
		__array(char, sname, 32)
		__field(__s32, stat)
		__field(__u32, lcpu)
		__field(__u32, pcpu)
		__field(unsigned long, need_resched_mask)
		__field(__u32, need_mark)
	),
	TP_ASSIGN(__entry->cpu = cpu;
		__entry->tid = tcb ? (*(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32))) : (-1);
		__entry->cookie = tcb ? tcb_secure_cookie_of(tcb) : 0UL;
		__entry->prio = core_sched_task_abs_prio(tcb);
		str_cpy_p_to_a(__entry->name, tcb ? tcb_name_of(tcb) : "NULL");
		__entry->stid = sibling ? (*(ptr_cast(tcb_miscdata_of(sibling, MISCDATA_TCB_TID), __s32))) : (-1);
		__entry->scookie = sibling ? tcb_secure_cookie_of(sibling) : 0UL;
		__entry->sprio = core_sched_task_abs_prio(sibling);
		str_cpy_p_to_a(__entry->sname, sibling ? tcb_name_of(sibling) : "NULL");
		__entry->stat = stat;
		__entry->need_resched_mask = need_resched_mask;
		__entry->need_mark = (u32)need_mark;
		__entry->lcpu = lcpu;
		__entry->pcpu = pcpu;
	),
	TP_PRINT("cpuid=%u tcb=%d/0x%llx comm=%s abs_prio=%d"
			  " sibling=%d/0x%llx comm=%s abs_prio=%d match_stat=%d lcpu=%u pcpu=%u need_resched_mask=0x%x need_mark=%u",
			__entry->cpu, __entry->tid, __entry->cookie, __entry->name, __entry->prio,
			__entry->stid, __entry->scookie, __entry->sname, __entry->sprio, __entry->stat,
			__entry->lcpu, __entry->pcpu, __entry->need_resched_mask, __entry->need_mark
	)
)

TRACE_TCB_EVENT(sched, core_match_prefer_cookie,
	TP_PROTO(unsigned int, cpu,
		u64, prefer_cookie,
		unsigned int, sibling_cpu,
		u64, sibling_prefer_cookie,
		bool, matched),
	TP_ARGS(cpu, prefer_cookie, sibling_cpu, sibling_prefer_cookie, matched),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__u64, prefer_cookie)
		__field(__u32, sibling_cpu)
		__field(__u64, sibling_prefer_cookie)
		__field(bool, matched)
	),
	TP_ASSIGN(__entry->cpu = cpu;
		  __entry->prefer_cookie = prefer_cookie;
		  __entry->sibling_cpu = sibling_cpu;
		  __entry->sibling_prefer_cookie = sibling_prefer_cookie;
		  __entry->matched = matched;
	),
	TP_PRINT("cpuid=%u, prefer_cookie=%llu, sibling_cpu=%d, sibling_prefer_cookie=%llu matched%u",
		 __entry->cpu, __entry->prefer_cookie, __entry->sibling_cpu, __entry->sibling_prefer_cookie, __entry->matched
	)
)

TRACE_TCB_EVENT(sched, core_sched_select_task,
	TP_PROTO(unsigned int, cpuid,
		struct tcb_s *, prev,
		struct tcb_s *, next,
		struct tcb_s *, sibling,
		unsigned int, lcpu,
		unsigned int, pcpu,
		__u32, need_resched_mask,
		unsigned, core_stat),
	TP_ARGS(cpuid, prev, next, sibling, lcpu, pcpu, need_resched_mask, core_stat),
	TP_FIELD(
		__field(__u32, cpuid)
		__field(__s32, ptid)
		__field(__u64, pcookie)
		__field(__s32, pprio)
		__array(char, pname, 32)
		__field(__s32, ntid)
		__field(__u64, ncookie)
		__field(__s32, nprio)
		__array(char, nname, 32)
		__field(__s32, stid)
		__field(__u64, scookie)
		__field(__s32, sprio)
		__array(char, sname, 32)
		__field(__u32, lcpu)
		__field(__u32, pcpu)
		__field(unsigned long, need_resched_mask)
		__field(__u32, core_stat)
	),
	TP_ASSIGN(__entry->cpuid = cpuid;
		__entry->ptid = (*(ptr_cast(tcb_miscdata_of(prev, MISCDATA_TCB_TID), __s32)));
		__entry->pcookie = tcb_secure_cookie_of(prev);
		__entry->pprio = core_sched_task_abs_prio(prev);
		str_cpy_p_to_a(__entry->pname, tcb_name_of(prev));
		__entry->ntid = next ? (*(ptr_cast(tcb_miscdata_of(next, MISCDATA_TCB_TID), __s32))) : (-1);
		__entry->ncookie = next ? tcb_secure_cookie_of(next) : 0UL;
		__entry->nprio = core_sched_task_abs_prio(next);
		str_cpy_p_to_a(__entry->nname, next ? tcb_name_of(next) : "NULL");
		__entry->stid = sibling ? (*(ptr_cast(tcb_miscdata_of(sibling, MISCDATA_TCB_TID), __s32))) : (-1);
		__entry->scookie = sibling ? tcb_secure_cookie_of(sibling) : 0UL;
		__entry->sprio = core_sched_task_abs_prio(sibling);
		str_cpy_p_to_a(__entry->sname, sibling ? tcb_name_of(sibling) : "NULL");
		__entry->need_resched_mask = need_resched_mask;
		__entry->lcpu = lcpu;
		__entry->pcpu = pcpu;
		__entry->core_stat = core_stat;
	),
	TP_PRINT("cpuid=%u prev=%d/0x%llx comm=%s abs_prio=%d next=%d/0x%llx comm=%s abs_prio=%d"
			  " sibling=%d/0x%llx comm=%s abs_prio=%d lcpu=%u pcpu=%u need_resched_mask=0x%x core_stat=%u",
			__entry->cpuid, __entry->ptid, __entry->pcookie, __entry->pname, __entry->pprio,
			__entry->ntid, __entry->ncookie, __entry->nname, __entry->nprio,
			__entry->stid, __entry->scookie, __entry->sname, __entry->sprio,
			__entry->lcpu, __entry->pcpu, __entry->need_resched_mask, __entry->core_stat
	)
)

TRACE_TCB_EVENT(sched, sched_global_boost,
	TP_PROTO(bool, is_eager),
	TP_ARGS(is_eager),
	TP_FIELD(
		__field(bool, is_eager)
	),
	TP_ASSIGN(
		__entry->is_eager = is_eager;
	),
	TP_PRINT("global_capacity_eager = %d ", __entry->is_eager
	)
)

TRACE_TCB_EVENT(sched, sched_level_overutilized,
	TP_PROTO(unsigned int, id,
		 bool, overutilized,
		 unsigned int, margin,
		 unsigned int, load,
		 unsigned int, capacity),
	TP_ARGS(id, overutilized, margin, load, capacity),
	TP_FIELD(
		__field(__u32, id)
		__field(bool, overutilized)
		__field(__u32, margin)
		__field(__u32, load)
		__field(__u32, capacity)
	),
	TP_ASSIGN(
		__entry->id = id;
		__entry->overutilized = overutilized;
		__entry->margin = margin;
		__entry->load = load;
		__entry->capacity = capacity;
	),
	TP_PRINT("level id = %d, overutilized = %d, margin = %u, load = %u, capacity = %u ",
		__entry->id, __entry->overutilized,
		__entry->margin, __entry->load, __entry->capacity
	)
)

TRACE_TCB_EVENT(sched, sched_task_load_overutilized,
	TP_PROTO(struct tcb_s *, tcb,
		 unsigned int, task_load,
		 unsigned int, task_margin,
		 unsigned int, avg_tcb_load,
		 unsigned int, cluster_id,
		 bool, degrade,
		 bool, overutilized
		),
	TP_ARGS(tcb, task_load, task_margin, avg_tcb_load, cluster_id, degrade, overutilized),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u32, task_load)
		__field(__u32, task_margin)
		__field(__u32, avg_tcb_load)
		__field(__u32, cluster_id)
		__field(__u32, degrade)
		__field(__u32, overutilized)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, (tcb == NULL ?
					       "deadline_grp" :
					       tcb_name_of(tcb)));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->cluster_id = cluster_id;
		__entry->task_load = task_load;
		__entry->task_margin = task_margin;
		__entry->avg_tcb_load = avg_tcb_load;
		__entry->degrade = (__u32)degrade;
		__entry->overutilized = (__u32)overutilized;
	),
	TP_PRINT("comm=%s tid=%d task_load=%u task_margin=%u cluster=%u avg_tcb_load=%u degrade=%u overutilized=%u",
		  __entry->name, __entry->tid, __entry->task_load, __entry->task_margin, __entry->cluster_id,
		  __entry->avg_tcb_load, __entry->degrade, __entry->overutilized)
)

TRACE_TCB_EVENT(sched, sched_choose_overutil_cluster,
	TP_PROTO(struct tcb_s *, tcb,
		 unsigned int, prefer_mask,
		 unsigned int, overutil_fit_mask,
		 unsigned int, degrade_mask
		),
	TP_ARGS(tcb, prefer_mask, overutil_fit_mask, degrade_mask),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u32, prefer_mask)
		__field(__u32, overutil_fit_mask)
		__field(__u32, degrade_mask)
	),
	TP_ASSIGN(
		if(tcb != NULL) {
			__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
			str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		} else {
			str_cpy_p_to_a(__entry->name, "NULL");
			__entry->tid = -1;
		}
		__entry->prefer_mask = prefer_mask;
		__entry->overutil_fit_mask = overutil_fit_mask;
		__entry->degrade_mask = degrade_mask;
	),
	TP_PRINT("comm=%s tid=%d prefer_mask=%x overutil_fit_mask=%x degrade_mask=%x",
		  __entry->name, __entry->tid, __entry->prefer_mask,
		  __entry->overutil_fit_mask, __entry->degrade_mask)
)

TRACE_EVENT(sched, rtsbw_ipi_enter,
	TP_PROTO(struct tcb_s *, tcb, int, event),
	TP_ARGS(tcb, event),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(int, event)
		__field(__u32, sched_flags)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->event = event;
		__entry->sched_flags = tcb_schedflags_of(tcb);
	),
	TP_PRINT(
		"pid=%d comm=%s prio=%d sched_flags=0x%x event=%s",
		__entry->tid, __entry->name, __entry->prio, __entry->sched_flags,
		__entry->event == 0 ? "MISSION_DOWNGRADE" : "MISSION_RECOVER"
	)
)

TRACE_TCB_EVENT(sched, rtsbw_ipi_mission_prepare,
	TP_PROTO(struct tcb_s *, tcb, int, event),
	TP_ARGS(tcb, event),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(int, event)
		__field(__u32, sched_flags)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->event = event;
		__entry->sched_flags = tcb_schedflags_of(tcb);
	),
	TP_PRINT(
		"pid=%d comm=%s prio=%d sched_flags=0x%x event=%s",
		__entry->tid, __entry->name, __entry->prio, __entry->sched_flags,
		__entry->event == 0 ? "MISSION_DOWNGRADE" : "MISSION_RECOVER"
	)
)

TRACE_TCB_EVENT(sched, rtsbw_ipi_mission_too_large,
	TP_PROTO(struct tcb_s *, tcb, int, event),
	TP_ARGS(tcb, event),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(int, event)
		__field(__u32, sched_flags)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->event = event;
		__entry->sched_flags = tcb_schedflags_of(tcb);
	),
	TP_PRINT(
		"pid=%d comm=%s prio=%d sched_flags=0x%x event=%s",
		__entry->tid, __entry->name, __entry->prio, __entry->sched_flags,
		__entry->event == 0 ? "MISSION_DOWNGRADE" : "MISSION_RECOVER"
	)
)

TRACE_TCB_EVENT(sched, rtsbw_ipi_do_handle_attr_change,
	TP_PROTO(struct tcb_s *, tcb, int, event),
	TP_ARGS(tcb, event),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(int, event)
		__field(__u32, sched_flags)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->event = event;
		__entry->sched_flags = tcb_schedflags_of(tcb);
	),
	TP_PRINT(
		"pid=%d comm=%s prio=%d sched_flags=0x%x event=%s",
		__entry->tid, __entry->name, __entry->prio, __entry->sched_flags,
		__entry->event == 0 ? "MISSION_DOWNGRADE" : "MISSION_RECOVER"
	)
)

TRACE_TCB_EVENT(sched, rt_soft_bandwidth_list_change,
	TP_PROTO(struct tcb_s *, tcb, int, event),
	TP_ARGS(tcb, event),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(int, event)
		__field(__u32, sched_flags)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->event = event;
		__entry->sched_flags = tcb_schedflags_of(tcb);
	),
	TP_PRINT(
		"pid=%d comm=%s prio=%d event=%d sched_flags=0x%x",
		__entry->tid, __entry->name, __entry->prio, __entry->event, __entry->sched_flags
	)
)

TRACE_TCB_EVENT(sched, rt_bandwidth_state_update,
	TP_PROTO(unsigned int, cpu, bool, suppressed),
	TP_ARGS(cpu, suppressed),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__u32, suppressed)
	),
	TP_ASSIGN(__entry->cpu = cpu;
		__entry->suppressed = (__u32)suppressed;
	),
	TP_PRINT("cpu %u rt bandwidth state %u",
		__entry->cpu, __entry->suppressed
	)
)

TRACE_TCB_EVENT(sched, rt_soft_bandwidth_suppressed,
	TP_PROTO(struct tcb_s *, tcb),
	TP_ARGS(tcb),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, prio)
		__field(__u32, sched_flags)
		__field(__u32, target_cpu)
		__field(__u64, cref)
		__array(char, name, 16)
	),
	TP_ASSIGN(str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->cref = tcb_cref_of(tcb);
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->sched_flags = tcb_schedflags_of(tcb);
	),
	TP_PRINT("tcb=%s cref=%llx tid=%d prio=%d sched_flags=0x%x",
		__entry->name, __entry->cref, __entry->tid, __entry->prio, __entry->sched_flags
	)
)

TRACE_TCB_EVENT(sched, rt_soft_bandwidth_reload,
	TP_PROTO(__u64, epoch,
		__s64, real_max_runtime,
		__s64, runtime_last,
		__u64, max_runtime,
		__u64, max_runtime_us),
	TP_ARGS(epoch, real_max_runtime, runtime_last, max_runtime, max_runtime_us),
	TP_FIELD(
		__field(__u64, epoch)
		__field(__s64, real_max_runtime)
		__field(__s64, runtime_last)
		__field(__u64, max_runtime)
		__field(__u64, max_runtime_us)
	),
	TP_ASSIGN(
		__entry->epoch = epoch;
		__entry->real_max_runtime = real_max_runtime;
		__entry->runtime_last = runtime_last;
		__entry->max_runtime = max_runtime;
		__entry->max_runtime_us = max_runtime_us;
	),
	TP_PRINT("epoch=%llu real_max_runtime=%lld "
		 "runtime_last=%lld max_runtime=%llu max_runtime_us=%llu",
		__entry->epoch, __entry->real_max_runtime, __entry->runtime_last,
		__entry->max_runtime, __entry->max_runtime_us
	)
)

TRACE_EVENT(sched, rt_cpu_quota_suppressed,
	TP_PROTO(__u64, curr_epoch,
		__u64, rt_exec_time_tock,
		__s64, real_max_runtime_tock,
		__u32, epoch_period,
		struct scheduler_s *, scheduler),
	TP_ARGS(curr_epoch, rt_exec_time_tock, real_max_runtime_tock, epoch_period, scheduler),
	TP_FIELD(
		__field(__u64, curr_epoch)
		__field(__u64, scheduler_cref)
		__field(__u64, rt_exec_time)
		__field(__s64, real_max_runtime)
		__field(__s64, real_max_runtime_tock)
		__field(__u32, epoch_period)
	),
	TP_ASSIGN(__entry->curr_epoch = curr_epoch;
		__entry->scheduler_cref = scheduler_cref_of(scheduler);
		__u64 freq = (__u64)vtimekeep_freq();
		__entry->rt_exec_time = rt_exec_time_tock * __USEC_PER_SEC / (__s64)freq;
		__entry->real_max_runtime = real_max_runtime_tock * __USEC_PER_SEC / (__s64)freq;
		__entry->real_max_runtime_tock = real_max_runtime_tock;
		__entry->epoch_period = epoch_period;
	),
	TP_PRINT("curr_epoch=%llu scheduler_cref=%llx rt_runtime=%llu real_max_runtime=%lld \
		  real_max_runtime_tock=%lld epoch_period=%u",
		__entry->curr_epoch, __entry->scheduler_cref, __entry->rt_exec_time,
		__entry->real_max_runtime, __entry->real_max_runtime_tock, __entry->epoch_period
	)
)

TRACE_EVENT(sched, rt_cpu_quota_reload,
	TP_PROTO(__u64, curr_epoch,
		__s64, runtime_tock,
		__u64, max_runtime_tock,
		__s64, real_max_runtime_tock,
		__u32, epoch_period,
		struct scheduler_s *, scheduler),
	TP_ARGS(curr_epoch, runtime_tock, max_runtime_tock, real_max_runtime_tock, epoch_period, scheduler),
	TP_FIELD(
		__field(__u64, curr_epoch)
		__field(__s64, real_max_runtime)
		__field(__s64, real_max_runtime_tock)
		__field(__s64, runtime)
		__field(__u64, max_runtime)
		__field(__u32, epoch_period)
	),
	TP_ASSIGN(
		__entry->curr_epoch = curr_epoch;
		__u64 freq = (__u64)vtimekeep_freq();
		__entry->real_max_runtime_tock = real_max_runtime_tock;
		__entry->real_max_runtime = real_max_runtime_tock * __USEC_PER_SEC / (__s64)freq;
		__entry->runtime = runtime_tock * __USEC_PER_SEC / freq;
		__entry->max_runtime = max_runtime_tock * __USEC_PER_SEC / freq;
		__entry->epoch_period = epoch_period;
	),
	TP_PRINT("curr_epoch=%llu real_max_runtime=%lld "
		 "real_max_runtime_tock=%lld runtime=%lld max_runtime=%llu epoch_period=%u",
		__entry->curr_epoch, __entry->real_max_runtime, __entry->real_max_runtime_tock,
		__entry->runtime, __entry->max_runtime, __entry->epoch_period
	)
)

TRACE_EVENT(sched, handle_migrate_rechoose,
	TP_PROTO(struct tcb_s *, tcb,
		 unsigned int, event,
		 unsigned int, orig_cpu,
		 unsigned int, new_cpu),
	TP_ARGS(tcb, event, orig_cpu, new_cpu),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(int, prio)
		__field(__u32, event)
		__field(__u32, orig_cpu)
		__field(__u32, new_cpu)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->prio = sched_tcb_get_prio(tcb);
		__entry->event = event;
		__entry->orig_cpu = orig_cpu;
		__entry->new_cpu = new_cpu;
	),
	TP_PRINT(
		"pid=%d comm=%s prio=%d event=%s orig_cpu=%u new_cpu=%d",
		__entry->tid, __entry->name, __entry->prio,
		__entry->event == 0 ? "OFFLINE_MIGRATE" : "ISOLATE_MIGRATE",
		__entry->orig_cpu, __entry->new_cpu
	)
)

TRACE_EVENT(sched, async_notify_repick,
	TP_PROTO(unsigned int, cpu, struct tcb_s *, tcb, const char *, err),
	TP_ARGS(cpu, tcb, err),
	TP_FIELD(
		__field(unsigned int, cpu)
		__array(char, name, 16)
		__array(char, err, 16)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		str_cpy_p_to_a(__entry->err, err);
	),
	TP_PRINT(
		"let cpu %u to notify tcb %s repick, ret=%s",
		__entry->cpu, __entry->name, __entry->err
	)
)

TRACE_EVENT(sched, cpu_key_thread_rollover,
	TP_PROTO(int, cpu, __s64, load, __s64, key_time, __s64, window),
	TP_ARGS(cpu, load, key_time, window),
	TP_FIELD(
		__field(int, cpu)
		__field(__s64, load)
		__field(__s64, key_time)
		__field(__s64, window)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->load = load;
		__entry->key_time = key_time;
		__entry->window = window;
	),
	TP_PRINT(
		"cpu=%d load=%lld key_thread_time=%lld window_size=%lld",
		__entry->cpu, __entry->load, __entry->key_time, __entry->window
	)
)

TRACE_EVENT(sched, sched_check_cpu,
	TP_PROTO(const struct tcb_s *, tcb,
		unsigned int, curr_cpu,
		u64, tcb_time,
		u64, curr_time,
		bool, allowed),
	TP_ARGS(tcb, curr_cpu, tcb_time, curr_time, allowed),
	TP_FIELD(
		__field(__u32, sequence)
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u64, tcb_time)
		__field(__u64, cur_time)
		__field(__u64, cpu_epoch)
		__field(__u64, tcb_epoch)
		__field(__u32, sched_flags)
		__field(__u32, tcb_cpu)
		__field(__u32, tcb_cpu_lv)
		__field(__u32, cur_cpu)
		__field(__u32, cur_cpu_lv)
		__field(__u32, allowed)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->sched_flags = tcb_schedflags_of(tcb);
		__entry->tcb_cpu = tcb_cpu_of(tcb);
		__entry->tcb_cpu_lv = cputopo_cluster_id_of(__entry->tcb_cpu);
		__entry->tcb_time = tcb_time;
		__entry->cur_cpu = curr_cpu;
		__entry->cur_cpu_lv = cputopo_cluster_id_of(__entry->cur_cpu);
		__entry->cur_time = curr_time;
		__entry->cpu_epoch = rt_cpu_epoch_of(tcb_cpu_of(tcb));
		__entry->tcb_epoch = rt_tcb_epoch_of(tcb);
		__entry->allowed = (u32)allowed;
	),
	TP_PRINT("tcb=%s tid=%d sched_flags=0x%x tcb_cpu:%u level:%u sc_time:%llu "
		 "on cur_cpu:%u level:%u cur_time:%llu cpu_epoch:%llu tcb_epoch:%llu "
		 "check_allowed:%u sched_delay:%lluus\n",
		 __entry->name, __entry->tid, __entry->sched_flags, __entry->tcb_cpu, __entry->tcb_cpu_lv,
		 __entry->tcb_time, __entry->cur_cpu, __entry->cur_cpu_lv, __entry->cur_time,
		 __entry->cpu_epoch, __entry->tcb_epoch, __entry->allowed, __entry->cur_time - __entry->tcb_time
	)
)

TRACE_EVENT(sched, sched_sum_load,
	TP_PROTO(const struct tcb_s *, tcb,
		unsigned int, cluster_id,
		unsigned int, event,
		int, delta,
		int, load),
	TP_ARGS(tcb, cluster_id, event, delta, load),
	TP_FIELD(
		__field(__s32, tid)
		__field(__u32, cluster_id)
		__field(__u32, event)
		__field(__s32, delta)
		__field(__s32, load)
	),
	TP_ASSIGN(
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->cluster_id = cluster_id;
		__entry->event = event;
		__entry->delta = delta;
		__entry->load = load;
	),
	TP_PRINT("tid = %d, cluster = %d, event = %u, delta = %d, load = %d ",
		__entry->tid, __entry->cluster_id,
		__entry->event, __entry->delta, __entry->load
	)
)

TRACE_EVENT(sched, rt_cpu_capacity_quota_reload,
	TP_PROTO(__s64, load,
		__u64, max_load,
		__s64, real_max_load),
	TP_ARGS(load, max_load, real_max_load),
	TP_FIELD(
		__field(__s64, load)
		__field(__u64, max_load)
		__field(__s64, real_max_load)
	),
	TP_ASSIGN(
		__entry->load = load;
		__entry->max_load = max_load;
		__entry->real_max_load = real_max_load;
	),
	TP_PRINT("rt_exec_load=%lld max_load=%llu real_max_load=%lld",
		__entry->load, __entry->max_load, __entry->real_max_load
	)
)

TRACE_EVENT(sched, rt_cpu_capacity_quota_suppressed,
	TP_PROTO(__s64, load,
		__u64, max_load,
		__s64, real_max_load),
	TP_ARGS(load, max_load, real_max_load),
	TP_FIELD(
		__field(__s64, load)
		__field(__u64, max_load)
		__field(__s64, real_max_load)
	),
	TP_ASSIGN(
		__entry->load = load;
		__entry->max_load = max_load;
		__entry->real_max_load = real_max_load;
	),
	TP_PRINT("rt_exec_load=%lld max_load=%llu real_max_load=%lld",
		__entry->load, __entry->max_load, __entry->real_max_load
	)
)

#define TRACE_TIMER_RBT_INSERT			0x1
#define TRACE_TIMER_RBT_REMOVE			0x2
#define TRACE_TIMER_RBT_EXPIRE_BEGIN		0x3
#define TRACE_TIMER_RBT_EXPIRE_END		0x4
TRACE_EVENT(timer, timer_rbt_operation,
	TP_PROTO(int, action, int, type, int, clk_ori_id, int, clk_id, int, flag, __u64, expire),
	TP_ARGS(action, type, clk_ori_id, clk_id, flag, expire),
	TP_FIELD(
		__field(int, action)
		__field(int, type)
		__field(int, clk_ori_id)
		__field(int, clk_id)
		__field(int, flag)
		__field(__u64, expire)
	),
	TP_ASSIGN(
		__entry->action = action;
		__entry->type = type;
		__entry->clk_ori_id = clk_ori_id;
		__entry->clk_id = clk_id;
		__entry->flag = flag;
		__entry->expire = expire;
	),
	TP_PRINT("action=%d type=%d clk_ori_id=%d clk_id=%d flag=%d expire=%lld",
		__entry->action, __entry->type, __entry->clk_ori_id, __entry->clk_id, __entry->flag, __entry->expire
	)
)

TRACE_TCB_EVENT(sched, sched_boost,
	TP_PROTO(struct tcb_s *, tcb,
		 struct sched_entity_boost_s *, boost_entity,
		 unsigned int, next_rq,
		 bool, enter
		),
	TP_ARGS(tcb, boost_entity, next_rq, enter),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__u32, rq)
		__field(__u32, next_rq)
		__field(__u32, boost_cnt)
		__field(__u64, time_slice)
		__field(__u64, on_cpu)
		__field(bool, enter)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, (tcb == NULL ?
					       "boost" :
					       tcb_name_of(tcb)));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->rq = tcb_rq_of(tcb);
		__entry->next_rq = next_rq;
		__entry->boost_cnt = boost_entity->boost_cnt;
		__entry->time_slice = boost_entity->time_slice;
		__entry->on_cpu = boost_entity->on_cpu;
		__entry->enter = enter;
	),
	TP_PRINT("comm=%s tid=%d curr_rq=%u next_rq=%u boost_cnt=%u time_slice=%llu, on_cpu=%llu enter=%d",
		__entry->name, __entry->tid, __entry->rq, __entry->next_rq, __entry->boost_cnt,
		__entry->time_slice, __entry->on_cpu, __entry->enter
	)
)

TRACE_TCB_EVENT(sched, sched_preempt,
	TP_PROTO(struct tcb_s *, tcb,
		__u32, type,
		const char *, err
		),
	TP_ARGS(tcb, type, err),
	TP_FIELD(
		__array(char, name, 16)
		__field(__s32, tid)
		__field(__s32, pid)
		__field(__u32, type)
		__field(__u32, cpu)
		__array(char, err, 16)
		),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, (tcb == NULL ?
					       "preempt" :
					       tcb_name_of(tcb)));
		str_cpy_p_to_a(__entry->err, err);
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->type = type;
		__entry->cpu = smp_current_cpu_id();
		),
	TP_PRINT("comm=%s tid=%d pid=%d type=%u cpu=%u err=%s",
		 __entry->name, __entry->tid, __entry->pid, __entry->type, __entry->cpu, __entry->err
		)
)

TRACE_TCB_EVENT(power_kernel, cpu_idle,
	TP_PROTO(unsigned int, state, unsigned int, cpu_id),
	TP_ARGS(state, cpu_id),
	TP_FIELD(
		__field(unsigned int, state)
		__field(unsigned int, cpu_id)
	),
	TP_ASSIGN(
		__entry->state = state;
		__entry->cpu_id = cpu_id;
	),
	TP_PRINT("state=%u cpu_id=%u",
		__entry->state, __entry->cpu_id
	)
)

TRACE_TCB_EVENT(sched, xdep_sync,
	TP_PROTO(struct tcb_s *, waiter,
		struct tcb_s *, holder,
		int, holder_prev_prio,
		int, holder_curr_prio,
		int, sync_type,
		int, boost_type),
	TP_ARGS(waiter, holder, holder_prev_prio, holder_curr_prio, sync_type, boost_type),
	TP_FIELD(
		__field(const char*, _holder_tcb_name)
		__field(const char*, _waiter_tcb_name)
		__field(__s32, _waiter_priority)
		__field(__s32, _holder_prev_prio)
		__field(__s32, _holder_curr_prio)
		__field(__s32, _waiter_tid)
		__field(__s32, _holder_tid)
		__field(__s32, _lock_type)
		__field(__s32, _boost_type)
	),
	TP_ASSIGN(__entry->_holder_tcb_name = tcb_name_of(holder);
			__entry->_waiter_tcb_name = (waiter == NULL) ? "None" : tcb_name_of(waiter);
			__entry->_waiter_priority = (waiter == NULL) ? 0 : tcb_prio_of(waiter);
			__entry->_holder_prev_prio = holder_prev_prio;
			__entry->_holder_curr_prio = holder_curr_prio;
			__entry->_waiter_tid = (waiter == NULL) ? 0 : (__s32)(*tcb_miscdata_of(waiter, MISCDATA_TCB_TID));
			__entry->_holder_tid = (__s32)(*tcb_miscdata_of(holder, MISCDATA_TCB_TID));
			__entry->_lock_type = sync_type;
			__entry->_boost_type = boost_type;
	),
	TP_PRINT("waiter:%s(%d), holder:%s(%d), waiter_prio:%d, holder_prev_prio:%d, " \
		"holder_curr_prio:%d, sync_type:%d, boost_type:%d\n",
		__entry->_waiter_tcb_name, __entry->_waiter_tid, __entry->_holder_tcb_name, __entry->_holder_tid,
		__entry->_waiter_priority, __entry->_holder_prev_prio, __entry->_holder_curr_prio,
		__entry->_lock_type, __entry->_boost_type
	)
)

TRACE_TCB_EVENT(power_kernel, irq_info,
	TP_PROTO(unsigned int, irq, int, need_update, unsigned int, wakeup_flag),
	TP_ARGS(irq, need_update, wakeup_flag),
	TP_FIELD(
		__field(unsigned int, irq)
		__field(int, need_update)
		__field(unsigned int, wakeup_flag)
	),
	TP_ASSIGN(
		__entry->irq = irq;
		__entry->need_update = need_update;
		__entry->wakeup_flag = wakeup_flag;
	),
	TP_PRINT("irq=%u interval=%d continuity=%d",
		__entry->irq, __entry->need_update, __entry->wakeup_flag
	)
)

TRACE_EVENT(power_kernel, suspend_resume,
	TP_PROTO(const char *, action,
		 int, val,
		 bool, start
		),
	TP_ARGS(action, val, start),
	TP_FIELD(
		__array(char, action, 32)
		__field(int, val)
		__field(bool, start)
	),
	TP_ASSIGN(
		__entry->val = val;
		__entry->start = start;
		str_cpy_p_to_a(__entry->action, action);
	),

	TP_PRINT("%s[%d] %s",
		__entry->action, __entry->val, (__entry->start)?"begin":"end"
	)
)

TRACE_TCB_EVENT(sched, eer_prefer_cluster_each,
	TP_PROTO(const struct tcb_s *, tcb,
		 unsigned int, cluster_id,
		 unsigned int, pred_util,
		 unsigned int, task_load,
		 unsigned int, cluster_load,
		 unsigned int, top_load,
		 unsigned int, eer
		),
	TP_ARGS(tcb, cluster_id, pred_util, task_load, cluster_load, top_load, eer),
	TP_FIELD(
		__array(char, name, 16)
		__field(__u32, cluster_id)
		__field(__u32, pred_util)
		__field(__u32, task_load)
		__field(__u32, cluster_load)
		__field(__u32, top_load)
		__field(__u32, eer)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->name, (tcb == NULL ?
					       "deadline_grp" :
					       tcb_name_of(tcb)));
		__entry->cluster_id = cluster_id;
		__entry->pred_util = pred_util;
		__entry->task_load = task_load;
		__entry->cluster_load = cluster_load;
		__entry->top_load = top_load;
		__entry->eer = eer;
	),

	TP_PRINT("comm=%s cluster=%u eer=%u pred_util=%u task_load=%u cluster_load=%u top_load=%u",
		  __entry->name, __entry->cluster_id, __entry->eer, __entry->pred_util,
		  __entry->task_load, __entry->cluster_load, __entry->top_load)
)

TRACE_TCB_EVENT(power_kernel, sched_ind_get_util,
	TP_PROTO(unsigned int, cpu,
		 unsigned int, chg_event,
		 unsigned int, util,
		 unsigned int, cluster_load,
		 unsigned int, top_load,
		 unsigned int, ddl_load,
		 unsigned int, user_freq,
		 unsigned int, min_util,
		 unsigned int, iowait,
		 unsigned int, cluster_min_load,
		 unsigned int, max_tcb_load,
		 unsigned int, raw_load
		),
	TP_ARGS(cpu, chg_event, util, cluster_load, top_load, ddl_load,
		user_freq, min_util, iowait, cluster_min_load, max_tcb_load, raw_load),
	TP_FIELD(
		__field(__u32, cpu)
		__field(__u32, chg_event)
		__field(__u32, util)
		__field(__u32, cluster_load)
		__field(__u32, top_load)
		__field(__u32, ddl_load)
		__field(__u32, user_freq)
		__field(__u32, min_util)
		__field(__u32, iowait)
		__field(__u32, cluster_min_load)
		__field(__u32, max_tcb_load)
		__field(__u32, raw_load)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->util = util;
		__entry->cluster_load = cluster_load;
		__entry->top_load = top_load;
		__entry->ddl_load = ddl_load;
		__entry->user_freq = user_freq;
		__entry->min_util = min_util;
		__entry->chg_event = chg_event;
		__entry->iowait = iowait;
		__entry->cluster_min_load = cluster_min_load;
		__entry->max_tcb_load = max_tcb_load;
		__entry->raw_load = raw_load;
	),

	TP_PRINT("cpu=%u util=%u cluster_load=%u "
		 "top_load=%u ddl_load=%u "
		 "user_freq=%u min_util=%u event=%u iowait=%u "
		 "cluster_min_load=%u max_load=%u raw_load =%u",
		 __entry->cpu, __entry->util, __entry->cluster_load,
		 __entry->top_load, __entry->ddl_load, __entry->user_freq,
		 __entry->min_util, __entry->chg_event, __entry->iowait, __entry->cluster_min_load,
		 __entry->max_tcb_load, __entry->raw_load)
)

TRACE_TCB_EVENT(power_kernel, cpufreq_schedutil_eval_target,
	TP_PROTO(unsigned int, cpu_id,
		 unsigned int, util,
		 unsigned int, max,
		 unsigned int, load,
		 unsigned int, rtg_util,
		 unsigned int, rtg_freq,
		 unsigned int, curr,
		 unsigned int, target
	),
	TP_ARGS(cpu_id, util, max, load, rtg_util, rtg_freq, curr, target),
	TP_FIELD(
		__field(__u32, cpu_id)
		__field(__u32, util)
		__field(__u32, max)
		__field(__u32, load)
		__field(__u32, rtg_util)
		__field(__u32, rtg_freq)
		__field(__u32, curr)
		__field(__u32, target)
	),
	TP_ASSIGN(
		__entry->cpu_id = cpu_id;
		__entry->util = util;
		__entry->max = max;
		__entry->load = load;
		__entry->rtg_util = rtg_util;
		__entry->rtg_freq = rtg_freq;
		__entry->curr = curr;
		__entry->target = target;
	),

	TP_PRINT("cpu=%u util=%u max=%u load=%u rtg_util=%u rtg_freq=%u curr=%u target=%u",
		  __entry->cpu_id, __entry->util, __entry->max, __entry->load,
		  __entry->rtg_util, __entry->rtg_freq, __entry->curr, __entry->target
	)
)

TRACE_TCB_EVENT(power_kernel, cpufreq_schedutil_already,
	TP_PROTO(unsigned int, cpu_id,
		 unsigned int, curr,
		 unsigned int, target
	),
	TP_ARGS(cpu_id, curr, target),
	TP_FIELD(
		__field(__u32, cpu_id)
		__field(__u32, curr)
		__field(__u32, target)
	),
	TP_ASSIGN(
		__entry->cpu_id = cpu_id;
		__entry->curr = curr;
		__entry->target = target;
	),

	TP_PRINT("cpu=%u curr=%u target=%u\n",
		  __entry->cpu_id, __entry->curr, __entry->target
	)
)

TRACE_TCB_EVENT(power_kernel, cpufreq_schedutil_notyet,
	TP_PROTO(unsigned int, cpu_id,
		 const char *, reason,
		 unsigned long long, delta_ns,
		 unsigned int, curr,
		 unsigned int, target
	),
	TP_ARGS(cpu_id, reason, delta_ns, curr, target),
	TP_FIELD(
		__field(__u32, cpu_id)
		__array(char, reason, 32)
		__field(__u64, delta_ns)
		__field(__u32, curr)
		__field(__u32, target)
	),
	TP_ASSIGN(
		__entry->cpu_id = cpu_id;
		str_cpy_p_to_a(__entry->reason, reason);
		__entry->delta_ns = delta_ns;
		__entry->curr = curr;
		__entry->target = target;
	),

	TP_PRINT("cpu=%u reason=%s delta_ns=%llu curr=%u target=%u\n",
		  __entry->cpu_id, __entry->reason, __entry->delta_ns, __entry->curr, __entry->target
	)
)

TRACE_TCB_EVENT(power_kernel, mipslt_sched_ind_data,
	TP_PROTO(unsigned int, cpu,
		 unsigned int, chg_event,
		 unsigned int, avg_ipc,
		 unsigned int, cpu_demand,
		 unsigned int, task_demand,
		 unsigned int, inst,
		 unsigned int, cycle,
		 unsigned int, ddl_load,
		 unsigned int, top_load
	),
	TP_ARGS(cpu, chg_event, avg_ipc, cpu_demand, task_demand, inst, cycle, ddl_load, top_load),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(unsigned int, chg_event)
		__field(unsigned int, avg_ipc)
		__field(unsigned int, cpu_demand)
		__field(unsigned int, task_demand)
		__field(unsigned int, inst)
		__field(unsigned int, cycle)
		__field(unsigned int, ddl_load)
		__field(unsigned int, top_load)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->chg_event = chg_event;
		__entry->avg_ipc = avg_ipc;
		__entry->cpu_demand = cpu_demand;
		__entry->task_demand = task_demand;
		__entry->inst = inst;
		__entry->cycle = cycle;
		__entry->ddl_load = ddl_load;
		__entry->top_load = top_load;
	),
	TP_PRINT("cpu=%u chg_event=%u avg_ipc=%u cpu_demand=%u task_demand=%u inst=%u cycle=%u ddl_load=%u top_load=%u",
		  __entry->cpu, __entry->chg_event, __entry->avg_ipc, __entry->cpu_demand, __entry->task_demand, __entry->inst,
		  __entry->cycle, __entry->ddl_load, __entry->top_load
	)
)

TRACE_TCB_EVENT(power_kernel, mipslt_cpu_pcount_hist,
	TP_PROTO(unsigned int, cpu,
		 unsigned int, flags,
		 unsigned int, hist_index,
		 unsigned long long *, pcounts
	),
	TP_ARGS(cpu, flags, hist_index, pcounts),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(unsigned int, flags)
		__field(unsigned int, hist_index)
		__array(__u64, pcounts, 6)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->flags = flags;
		__entry->hist_index = hist_index;
		(void)memcpy(__entry->pcounts, pcounts, sizeof(__entry->pcounts));
	),
	TP_PRINT("cpu=%u flags %u index %u "
		 "pounts0=%llu pounts1=%llu pounts2=%llu pounts3=%llu pounts4=%llu pounts5=%llu\n",
		 __entry->cpu, __entry->flags, __entry->hist_index, __entry->pcounts[0], __entry->pcounts[1],
		 __entry->pcounts[2], __entry->pcounts[3], __entry->pcounts[4], __entry->pcounts[5]
	)
)

TRACE_TCB_EVENT(power_kernel, mips_mem_cpu_freq,
	TP_PROTO(unsigned int, cpu,
		 struct tcb_s *, tcb,
		 unsigned int, type,
		 unsigned int, inst,
		 unsigned int, cycle,
		 unsigned int, refill,
		 unsigned int, ratio,
		 unsigned int, cpu_freq,
		 unsigned int, next_freq
	),
	TP_ARGS(cpu, tcb, type, inst, cycle, refill, ratio, cpu_freq, next_freq),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(__s32, tid)
		__array(char, t_name, 16)
		__field(unsigned int, type)
		__field(unsigned int, inst)
		__field(unsigned int, cycle)
		__field(unsigned int, refill)
		__field(unsigned int, ratio)
		__field(unsigned int, cpu_freq)
		__field(unsigned int, next_freq)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		if(tcb != NULL) {
			__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
			str_cpy_p_to_a(__entry->t_name, tcb_name_of(tcb));
		} else {
			__entry->tid = -1;
			str_cpy_p_to_a(__entry->t_name, "NULL");
		}
		__entry->type = type;
		__entry->inst = inst;
		__entry->cycle = cycle;
		__entry->refill = refill;
		__entry->ratio = ratio;
		__entry->cpu_freq = cpu_freq;
		__entry->next_freq = next_freq;
	),
	TP_PRINT("cpu=%u tid=%d t_name=%s type=%u inst=%u cycle=%u refill=%u ratio=%u cpu_freq=%u next_freq=%u",
		  (unsigned int)__entry->cpu,
		  (int)__entry->tid,
		  __entry->t_name,
		  (unsigned int)__entry->type,
		  (unsigned int)__entry->inst,
		  (unsigned int)__entry->cycle,
		  (unsigned int)__entry->refill,
		  (unsigned int)__entry->ratio,
		  (unsigned int)__entry->cpu_freq,
		  (unsigned int)__entry->next_freq
	)
)

#define print_array_to_string(array, num, str, len) \
do { \
	int i; \
	int ret; \
	int offset = 0; \
	for (i = 0; i < (num); i++) { \
		ret = snprintf((str) + offset, (len) - offset, "%llu,", (array)[i]); \
		if (ret < 0) { \
			break; \
		} \
		offset += ret; \
	} \
} while (0)

TRACE_TCB_EVENT(power_kernel, phase_perf,
	TP_PROTO(__u64 *, curr, __u64 *, delta, int, num),
	TP_ARGS(curr, delta, num),
	TP_FIELD(
		__array(char, curr_info, 256)
		__array(char, delta_info, 256)
	),
	TP_ASSIGN(
		mem_zero_s(__entry->curr_info);
		print_array_to_string(curr, num, __entry->curr_info, 256);
		mem_zero_s(__entry->delta_info);
		print_array_to_string(delta, num, __entry->delta_info, 256);
	),
	TP_PRINT(
		"curr={%s}, delta={%s}",
		__entry->curr_info, __entry->delta_info
	)
)

TRACE_TCB_EVENT(power_kernel, phase_task_delta,
	TP_PROTO(struct tcb_s *, tcb, __u64, delta_exec, __u64 *, delta, int, num),
	TP_ARGS(tcb, delta_exec, delta, num),
	TP_FIELD(
		__array(char, name, 16)
		__field(int, tid)
		__field(__u64, delta_exec)
		__array(char, info, 256)
	),
	TP_ASSIGN(
		int i;
		int ret;
		int offset = 0;

		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->delta_exec = delta_exec;
		print_array_to_string(delta, num, __entry->info, 256);
	),
	TP_PRINT(
		"comm=%s tid=%d delta_exec=%llu deltas={%s}",
		__entry->name, __entry->tid, __entry->delta_exec, __entry->info
	)
)

TRACE_EVENT(power_kernel, clock_set_rate,
	TP_PROTO(
		 const char*, name,
		 unsigned int, state,
		 unsigned int, cpu_id
	),
	TP_ARGS(name, state, cpu_id),
	TP_FIELD(
		__string(name, name)
		__field(__u32, state)
		__field(__u32, cpu_id)
	),
	TP_ASSIGN(
		__entry->state = state;
		__entry->cpu_id = cpu_id;
		__assign_str(name, name);
	),
	TP_PRINT("%s state=%u cpu_id=%u", __get_str(name), __entry->state, __entry->cpu_id)
)

TRACE_TCB_EVENT(sched, phase_task_feature_data,
	TP_PROTO(
		struct tcb_s *, task, 
	    __u64, sample_count,
		__u64, mem_bound_avg
	),
	TP_ARGS(task, sample_count, mem_bound_avg),
	TP_FIELD(
		__field(int,	pid)
		__array(char,	pname,	16)
		__field(__u64,	sample_count)
		__field(__u64, 	mem_bound_avg)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->pname, tcb_name_of(task));
		__entry->pid = *(ptr_cast(tcb_miscdata_of((struct tcb_s *)task, MISCDATA_TCB_TID), __s32));
		__entry->sample_count = sample_count;
		__entry->mem_bound_avg = mem_bound_avg;
	),
 
	TP_PRINT("pid=%d name=%s sample_count=%u mem_bound_avg=%llu",
		__entry->pid, __entry->pname, __entry->sample_count, __entry->mem_bound_avg)
)
 
TRACE_TCB_EVENT(sched, phase_task_feature_flag,
	TP_PROTO(
		struct tcb_s *, task, 
		__u64, mem_bound_avg
	),
	TP_ARGS(task, mem_bound_avg),
	TP_FIELD(
		__field(int,	pid)
		__array(char,	pname,	16)
		__field(__u64, 	mem_bound_avg)
	),
	TP_ASSIGN(
		str_cpy_p_to_a(__entry->pname, tcb_name_of(task));
		__entry->pid = *(ptr_cast(tcb_miscdata_of((struct tcb_s *)task, MISCDATA_TCB_TID), __s32));
		__entry->mem_bound_avg = mem_bound_avg;
	),
	TP_PRINT("pid=%d name=%s mem_bound_avg=%llu", __entry->pid, __entry->pname, __entry->mem_bound_avg)
)

TRACE_TCB_EVENT(pid_freq, pid_mips_timer,
	TP_PROTO(unsigned int, id, u64, value),
	TP_ARGS(id, value),
	TP_FIELD(
		__field(unsigned int, id)
		__field(__u64, value)
		__array(char, name, 32)
		__field(__s32, tid)
		__field(__s32, pid)
	),
	TP_ASSIGN(
		struct tcb_s *tcb = tcb_current();
		__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_PID), __s32));
		str_cpy_p_to_a(__entry->name, tcb_name_of(tcb));
		__entry->id = id;
		__entry->value = value;
	),
	TP_PRINT("pid_mips_timer_%u state=%llu comm=%s tid=%d pid=%d\n",
		__entry->id, __entry->value, __entry->name, __entry->tid, __entry->pid
	)
)

TRACE_TCB_EVENT(pid_freq, sched_pid,
	TP_PROTO(unsigned int, cpu_id,
		struct tcb_s *, tcb,
		unsigned long long, run_time,
		unsigned long long, iowait_time,
		unsigned long long, delta),
	TP_ARGS(cpu_id, tcb, run_time, iowait_time, delta),
	TP_FIELD(
		__field(unsigned int, cpu_id)
		__array(char, pname, 16)
		__field(unsigned int, pid)
		__field(unsigned long long, run_time)
		__field(unsigned long long, iowait_time)
		__field(unsigned long long, delta)
	),
	TP_ASSIGN(
		__entry->cpu_id = cpu_id;
		str_cpy_p_to_a(__entry->pname, tcb_name_of(tcb));
		__entry->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), int));
		__entry->run_time = run_time;
		__entry->iowait_time = iowait_time;
		__entry->delta = delta;
	),
	TP_PRINT("cpu=%u comm=%s pid=%d run_time=%llu iowait_time=%llu delta=%llu",
		__entry->cpu_id, __entry->pname, __entry->pid, __entry->run_time, __entry->iowait_time, __entry->delta
	)
)

#define MAX_SYSCALL_PARAM 6u
TRACE_EVENT(raw_syscalls, sys_enter,
	TP_PROTO(struct arch_uctx *, uctx,
		long, lcallno
	),
	TP_ARGS(uctx, lcallno),
	TP_FIELD(
		__field(long, id)
		__array(unsigned long, args, MAX_SYSCALL_PARAM)
	),
	TP_ASSIGN(
		__entry->id = lcallno;
        arch_trace_sys_enter(uctx, __entry->args);
	),
	TP_PRINT("NR %ld (%lx, %lx, %lx, %lx, %lx, %lx)",
		__entry->id,
		__entry->args[0], __entry->args[1], __entry->args[2],
		__entry->args[3], __entry->args[4], __entry->args[5]
	)
)

TRACE_EVENT(raw_syscalls, sys_exit,
	TP_PROTO(struct arch_uctx *, uctx, long, lcallno),
	TP_ARGS(uctx, lcallno),
	TP_FIELD(
		__field(long, id)
		__field(long, ret)
	),
	TP_ASSIGN(
		__entry->id = lcallno;
		arch_trace_sys_exit(uctx, &__entry->ret);
	),
	TP_PRINT("NR %ld = %ld",
		__entry->id, __entry->ret
	)
)

TRACE_EVENT(tracing_mark_write, tracing_mark_write,
	TP_PROTO(const char *, content_buffer, __u32, content_size),
	TP_ARGS(content_buffer, content_size),
	TP_FIELD(
		/*
		 * Content_size may not contain '\0'. we need save '\0',
		 * so I need to define a array with length of content_size + 1.
		 */
		__dynamic_array(char, buffer, content_size + 1)
	),
	TP_ASSIGN(
		void *dest = __get_dynamic_array(buffer);
		int err = copy_from_user(dest, content_buffer, content_size);
		if (err < 0) {
			/* write <faulted> or part of it when pagefault */
			const char *err_msg = "<faulted>";
			strncpy(dest, err_msg, content_size);
		} else {
			((char *)dest)[content_size] = '\0';
		}
	),
	TP_PRINT("%s", __get_dynamic_array(buffer))
)

TRACE_EVENT(eventpoll, ppoll,
	TP_PROTO(struct tcb_s *, tcb, __s32, error, const char *, str),
	TP_ARGS(tcb, error, str),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, error)
		__string(str, str)
	),
	TP_ASSIGN(
		if(tcb != NULL) {
			__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		} else {
			__entry->tid = -1;
		}
		__entry->error = error;
		__assign_str(str, str);
	),
	TP_PRINT("tid=%d error=%d, %s", __entry->tid, __entry->error, __get_str(str))
)

TRACE_EVENT(eventpoll, select,
	TP_PROTO(struct tcb_s *, tcb, __s32, error, const char *, str),
	TP_ARGS(tcb, error, str),
	TP_FIELD(
		__field(__s32, tid)
		__field(__s32, error)
		__string(str, str)
	),
	TP_ASSIGN(
		if(tcb != NULL) {
			__entry->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		} else {
			__entry->tid = -1;
		}
		__entry->error = error;
		__assign_str(str, str);
	),
	TP_PRINT("tid=%d err=%d, %s", __entry->tid, __entry->error, __get_str(str))
)
