/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 14:05:24 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */
TRACE_EVENT(power, suspend_resume,
	TP_PROTO(const char*, action,
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
		strcpy_s(__entry->action, 32, action);
	),
	TP_PRINT("%s[%d] %s",
		__entry->action, __entry->val, (__entry->start) ? "begin" : "end"
	)
)

TRACE_EVENT(power, clock_set_rate,
	TP_PROTO(
		 const char*, name,
		 unsigned int, state,
		 unsigned int, cpu_id
	),
	TP_ARGS(name, state, cpu_id),
	TP_FIELD(
		__string(name, name)
		__field(__u64, state)
		__field(__u64, cpu_id)
	),
	TP_ASSIGN(
		__entry->state = state;
		__entry->cpu_id = cpu_id;
		__assign_str(name, name);
	),
	TP_PRINT("%s state=%lu cpu_id=%lu", __get_str(name), (unsigned long)__entry->state, (unsigned long)__entry->cpu_id)
)

TRACE_EVENT(power, idle_select_info,
	TP_PROTO(
		 const char*, name,
		 unsigned int, data,
		 unsigned int, cpu_id
	),
	TP_ARGS(name, data, cpu_id),
	TP_FIELD(
		__string(name, name)
		__field(__u64, data)
		__field(__u64, cpu_id)
	),
	TP_ASSIGN(
		__entry->data = data;
		__entry->cpu_id = cpu_id;
		__assign_str(name, name);
	),
	TP_PRINT("%s state=%lu cpu_id=%lu", __get_str(name), (unsigned long)__entry->data, (unsigned long)__entry->cpu_id)
)

TRACE_EVENT(power, memlat_frequency,
	TP_PROTO(
		 const char *, tag,
		 unsigned int, state,
		 unsigned int, cpu_cluster_id
	),
	TP_ARGS(tag, state, cpu_cluster_id),
	TP_FIELD(
		__field(const char *, tag)
		__field(unsigned int, state)
		__field(unsigned int, cpu_cluster_id)
	),
	TP_ASSIGN(
		__entry->state = state;
	),
	TP_PRINT("state=%lu", (unsigned long)__entry->state)
)

TRACE_EVENT(power, cpu_frequency,
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

TRACE_EVENT(power, cpu_frequency_limits,
	TP_PROTO(unsigned int, min,
		unsigned int, max,
		unsigned int, cpu_id
	),
	TP_ARGS(min, max, cpu_id),
	TP_FIELD(
		__field(__u32, min)
		__field(__u32, max)
		__field(__u32, cpu_id)
	),
	TP_ASSIGN(
		__entry->min = min;
		__entry->max = max;
		__entry->cpu_id = cpu_id;
	),
	TP_PRINT("min=%lu max=%lu cpu_id=%lu",
		  (unsigned long)__entry->min,
		  (unsigned long)__entry->max,
		  (unsigned long)__entry->cpu_id
	)
)

TRACE_EVENT(power, sched_ind_listener_mips_data,
	TP_PROTO(unsigned int, cpu,
		 unsigned int, chg_event,
		 unsigned int, avg_ipc,
		 unsigned int, cpu_demand,
		 unsigned int, task_demand,
		 unsigned int, inst,
		 unsigned int, cycle
	),
	TP_ARGS(cpu, chg_event, avg_ipc, cpu_demand, task_demand, inst, cycle),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(unsigned int, chg_event)
		__field(unsigned int, avg_ipc)
		__field(unsigned int, cpu_demand)
		__field(unsigned int, task_demand)
		__field(unsigned int, inst)
		__field(unsigned int, cycle)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->chg_event = chg_event;
		__entry->avg_ipc = avg_ipc;
		__entry->cpu_demand = cpu_demand;
		__entry->task_demand = task_demand;
		__entry->inst = inst;
		__entry->cycle = cycle;
	),
	TP_PRINT("cpu=%u chg_event=%u avg_ipc=%u cpu_demand=%u task_demand=%u inst=%u cycle=%u",
		  __entry->cpu, __entry->chg_event, __entry->avg_ipc, __entry->cpu_demand, __entry->task_demand, __entry->inst,
		  __entry->cycle
	)
)

TRACE_EVENT(power, mips_cpu_freq,
	TP_PROTO(unsigned int, cpu,
		unsigned int, reporting_policy,
		unsigned int, cpum_freq,
		unsigned int, taskm_freq,
		unsigned int, pid_freq,
		unsigned int, maxm_freq,
		unsigned int, freq
	),
	TP_ARGS(cpu, reporting_policy, cpum_freq, taskm_freq, pid_freq, maxm_freq, freq),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(unsigned int, reporting_policy)
		__field(unsigned int, cpum_freq)
		__field(unsigned int, taskm_freq)
		__field(unsigned int, pid_freq)
		__field(unsigned int, maxm_freq)
		__field(unsigned int, freq)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->reporting_policy = reporting_policy;
		__entry->cpum_freq = cpum_freq;
		__entry->taskm_freq = taskm_freq;
		__entry->pid_freq = pid_freq;
		__entry->maxm_freq = maxm_freq;
		__entry->freq = freq;
	),
	TP_PRINT("cpu=%u reporting_policy=%u cpum_freq=%u taskm_freq=%u pid_freq=%u "
		"maxm_freq=%u freq=%u",
		__entry->cpu, __entry->reporting_policy, __entry->cpum_freq, __entry->taskm_freq,
		__entry->pid_freq, __entry->maxm_freq, __entry->freq
	)
)

TRACE_EVENT(power, update_mips_cluster_freq,
	TP_PROTO(unsigned int, cluster_id,
		unsigned int, ddl_freq,
		unsigned int, top_freq,
		unsigned int, next_freq
	),
	TP_ARGS(cluster_id, ddl_freq, top_freq, next_freq),
	TP_FIELD(
		__field(unsigned int, cluster_id)
		__field(unsigned int, ddl_freq)
		__field(unsigned int, top_freq)
		__field(unsigned int, next_freq)
	),
	TP_ASSIGN(
		__entry->cluster_id = cluster_id;
		__entry->ddl_freq = ddl_freq;
		__entry->top_freq = top_freq;
		__entry->next_freq = next_freq;
	),
	TP_PRINT("cluster_id=%u ddl_freq=%u top_freq=%u next_freq=%u",
		__entry->cluster_id, __entry->ddl_freq, __entry->top_freq, __entry->next_freq
	)
)

TRACE_EVENT(power, update_mips_mem_freq,
	TP_PROTO(unsigned int, cluster_id,
		unsigned int, type,
		unsigned int, next_freq
	),
	TP_ARGS(cluster_id, type, next_freq),
	TP_FIELD(
		__field(unsigned int, cluster_id)
		__field(unsigned int, type)
		__field(unsigned int, next_freq)
	),
	TP_ASSIGN(
		__entry->cluster_id = cluster_id;
		__entry->type = type;
		__entry->next_freq = next_freq;
	),
	TP_PRINT("cluster_id=%u type=%u next_freq=%u",
		__entry->cluster_id, __entry->type, __entry->next_freq
	)
)

TRACE_EVENT(power, rid_reflect,
	TP_PROTO(
		 unsigned int, cpu,
		 unsigned long long, expect_us,
		 unsigned long long, actual_us,
		 int, is_intercept
	),
	TP_ARGS(cpu, expect_us, actual_us, is_intercept),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(unsigned long long, expect_us)
		__field(unsigned long long, actual_us)
		__field(int, is_intercept)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->expect_us = expect_us;
		__entry->actual_us = actual_us;
		__entry->is_intercept = is_intercept;
	),
	TP_PRINT("cpu=%u expect_us=%llu actual_us=%llu is_intercept=%d",
		  __entry->cpu,
		  __entry->expect_us,
		  __entry->actual_us,
		  __entry->is_intercept
	)
)

TRACE_EVENT(power, rid_predict,
	TP_PROTO(
		 unsigned int, cpu,
		 int, intercept_num,
		 unsigned long long, max,
		 unsigned long long, next_timer,
		 unsigned long long, predict
	),
	TP_ARGS(cpu, intercept_num, max, next_timer, predict),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(int, intercept_num)
		__field(unsigned long long, max)
		__field(unsigned long long, next_timer)
		__field(unsigned long long, predict)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->intercept_num = intercept_num;
		__entry->max = max;
		__entry->next_timer = next_timer;
		__entry->predict = predict;
	),
	TP_PRINT("cpu=%u intercept_num=%d, max=%llu next_timer=%llu predict=%llu",
		  __entry->cpu,
		  __entry->intercept_num,
		  __entry->max,
		  __entry->next_timer,
		  __entry->predict
	)
)

TRACE_EVENT(power, cpu_tpa_info,
	TP_PROTO(
		int, cluster_id,
		int, temp,
		int, freq_cur,
		int, freq_next,
		int, is_used,
		int, pred_temp
	),
	TP_ARGS(cluster_id, temp, freq_cur, freq_next, is_used, pred_temp),
	TP_FIELD(
		__field(int, cluster_id)
		__field(int, temp)
		__field(int, freq_cur)
		__field(int, freq_next)
		__field(int, is_used)
		__field(int, pred_temp)
	),
	TP_ASSIGN(
		__entry->cluster_id = cluster_id;
		__entry->temp = temp;
		__entry->freq_cur = freq_cur;
		__entry->freq_next = freq_next;
		__entry->is_used = is_used;
		__entry->pred_temp = pred_temp;
	),
	TP_PRINT("cluster_id is %d temp=%d freq_cur=%d "
		"freq_next=%d is_used=%d pred_temp=%d",
		__entry->cluster_id, __entry->temp, __entry->freq_cur,
		__entry->freq_next, __entry->is_used, __entry->pred_temp
	)
)

TRACE_EVENT(pid_freq, pid_freq_event,
	TP_PROTO(
		int, cpu,
		unsigned long long, cpu_cycles,
		int, cpu_usage,
		struct pid_control_cpu_info*, pid_control_cpu,
		int, freq_out
	),
	TP_ARGS(cpu, cpu_cycles, cpu_usage, pid_control_cpu, freq_out),
	TP_FIELD(
		__field(int, cpu)
		__field(unsigned long long, cpu_cycles)
		__field(int, cpu_usage)
		__field(int, target_load_out)
		__field(int, avg_freq)
		__field(int, lastfreq)
		__field(int, offset_freq)
		__field(int, freq_out)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->cpu_cycles = cpu_cycles;
		__entry->cpu_usage = cpu_usage;
		__entry->target_load_out = pid_control_cpu->target_load_out;
		__entry->avg_freq = pid_control_cpu->window_avg_freq;
		__entry->lastfreq = pid_control_cpu->pid_freq_out;
		__entry->offset_freq = pid_control_cpu->offset_freq;
		__entry->freq_out = freq_out;
	),
	TP_PRINT("cpu=%d raw-cpu-cycles=%llu cpu_usage=%d nowTarget=%d avg_freq=%d lastfreq=%d offset_freq=%d freq_out=%d",
		__entry->cpu, __entry->cpu_cycles, __entry->cpu_usage, __entry->target_load_out, __entry->avg_freq,
		__entry->lastfreq, __entry->offset_freq, __entry->freq_out
	)
)

TRACE_EVENT(pid_freq, calculate_util_err,
	TP_PROTO(
		int, cpu,
		int, curr_util,
		int, target_util,
		struct pid_control_cpu_info*, pid_control_cpu
	),
	TP_ARGS(cpu, curr_util, target_util, pid_control_cpu),
	TP_FIELD(
		__field(int, cpu)
		__field(int, curr_util)
		__field(int, target_util)
		__field(int, target_load_out)
		__field(int, normalized_util_error)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->curr_util = curr_util;
		__entry->target_util = target_util;
		__entry->target_load_out = pid_control_cpu->target_load_out;
		__entry->normalized_util_error = pid_control_cpu->freq_err;
	),
	TP_PRINT("cpu=%d curr_util=%d target_util=%d "
		"speed_final_target=%d normalized_util_error=%d",
		__entry->cpu, __entry->curr_util, __entry->target_util,
		__entry->target_load_out, __entry->normalized_util_error
	)
)

TRACE_EVENT(pid_freq, freq_pid_control,
	TP_PROTO(
		int, cpu,
		struct pid_control_cpu_info*, pid_control_cpu,
		int, p_freq,
		int, i_freq,
		int, d_freq
	),
	TP_ARGS(cpu, pid_control_cpu, p_freq, i_freq, d_freq),
	TP_FIELD(
		__field(int, cpu)
		__field(int, freq_err_sum)
		__field(int, p_freq)
		__field(int, i_freq)
		__field(int, d_freq)
		__field(int, window_avg_freq)
		__field(int, last_freq_err)
		__field(int, normalized_util_error)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->freq_err_sum = pid_control_cpu->freq_err_sum;
		__entry->p_freq = p_freq;
		__entry->i_freq = i_freq;
		__entry->d_freq = d_freq;
		__entry->window_avg_freq = pid_control_cpu->window_avg_freq;
		__entry->last_freq_err = pid_control_cpu->last_freq_err;
		__entry->normalized_util_error = pid_control_cpu->freq_err;
	),
	TP_PRINT("cpu=%d pid_control_cpu->freq_err_sum=%d p_freq=%d i_freq=%d d_freq=%d "
		"avg_freq=%d last_freq_err=%d normalized_util_error=%d",
		__entry->cpu, __entry->freq_err_sum, __entry->p_freq, __entry->i_freq, __entry->d_freq,
		__entry->window_avg_freq, __entry->last_freq_err, __entry->normalized_util_error
	)
)

TRACE_EVENT(pid_freq, usage_speed_pid,
	TP_PROTO(
		int, cpu,
		int, speed,
		struct pid_control_cpu_info*, pid_control_cpu,
		int, p_speed,
		int, i_speed,
		int, d_speed,
		int, speed_target
	),
	TP_ARGS(cpu, speed, pid_control_cpu, p_speed, i_speed, d_speed, speed_target),
	TP_FIELD(
		__field(int, cpu)
		__field(int, speed)
		__field(int, usage_speed_err_sum)
		__field(int, p_speed)
		__field(int, i_speed)
		__field(int, d_speed)
		__field(int, speed_target)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->speed = speed;
		__entry->usage_speed_err_sum = pid_control_cpu->usage_speed_err_sum;
		__entry->p_speed = p_speed;
		__entry->i_speed = i_speed;
		__entry->d_speed = d_speed;
		__entry->speed_target = speed_target;
	),
	TP_PRINT("cpu=%d speed=%d usage_speed_err_sum=%d "
		"p_speed=%d i_speed=%d d_speed=%d speed_target=%d",
		__entry->cpu, __entry->speed, __entry->usage_speed_err_sum,
		__entry->p_speed, __entry->i_speed, __entry->d_speed, __entry->speed_target
	)
)

TRACE_EVENT(pid_freq, update_avg_freq,
	TP_PROTO(
		int, cpu,
		int, cur_avg,
		int, cur_freq,
		unsigned long long, now,
		unsigned long long, last_time
	),
	TP_ARGS(cpu, cur_avg, cur_freq, now, last_time),
	TP_FIELD(
		__field(int, cpu)
		__field(int, cur_avg)
		__field(int, cur_freq)
		__field(unsigned long long, now)
		__field(unsigned long long, last_time)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->cur_avg = cur_avg;
		__entry->cur_freq = cur_freq;
		__entry->now = now;
		__entry->last_time = last_time;
	),
	TP_PRINT("cpu=%d cur_avg=%d cur_freq=%d now=%llu last_time=%llu",
		__entry->cpu, __entry->cur_avg,
		__entry->cur_freq, __entry->now, __entry->last_time
	)
)

TRACE_EVENT(pid_freq, iowait_pid_control,
	TP_PROTO(
		unsigned int, cpu,
		struct pid_control_cpu_info *, pid_control_cpu,
		long long, run_time,
		int *, loss_hist,
		int, p_loss,
		int, i_loss,
		int, d_loss
	),
	TP_ARGS(cpu, pid_control_cpu, run_time, loss_hist, p_loss, i_loss, d_loss),
	TP_FIELD(
		__field(unsigned int, cpu)
		__field(int, offset_freq)
		__field(long long, run_time)
		__array(int, loss_hist, 6)
		__field(int, p_loss)
		__field(int, i_loss)
		__field(int, d_loss)
		__field(int, iowait_target)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->offset_freq = pid_control_cpu->offset_freq;
		__entry->run_time = run_time;
		memcpy_s(__entry->loss_hist, 6 * sizeof(int), loss_hist, 6 * sizeof(int));
		__entry->p_loss = p_loss;
		__entry->i_loss = i_loss;
		__entry->d_loss = d_loss;
		__entry->iowait_target = pid_control_cpu->iowait_target;
	),
	TP_PRINT("cpu=%u offset_freq=%d runtime=%lld loss_hist=%d %d %d %d %d %d "
			"p_loss=%d i_loss=%d d_loss=%d iowait_target=%d",
		__entry->cpu, __entry->offset_freq, __entry->run_time,  __entry->loss_hist[0], __entry->loss_hist[1],
		__entry->loss_hist[2], __entry->loss_hist[3], __entry->loss_hist[4], __entry->loss_hist[5],
		__entry->p_loss, __entry->i_loss, __entry->d_loss, __entry->iowait_target
	)
)

TRACE_EVENT(pid_freq, thermal_pid_control,
	TP_PROTO(
		int, cpu,
		int, thermal_err,
		int, thermal_err_sum,
		int, thermal_err_diff,
		int, p_thermal,
		int, i_thermal,
		int, d_thermal,
		int, thermal_target
	),
	TP_ARGS(cpu, thermal_err, thermal_err_sum, thermal_err_diff, p_thermal, i_thermal, d_thermal, thermal_target),
	TP_FIELD(
		__field(int, cpu)
		__field(int, thermal_err)
		__field(int, thermal_err_sum)
		__field(int, thermal_err_diff)
		__field(int, p_thermal)
		__field(int, i_thermal)
		__field(int, d_thermal)
		__field(int, thermal_target)
	),
	TP_ASSIGN(
		__entry->cpu = cpu;
		__entry->thermal_err = thermal_err;
		__entry->thermal_err_sum = thermal_err_sum;
		__entry->thermal_err_diff = thermal_err_diff;
		__entry->p_thermal = p_thermal;
		__entry->i_thermal = i_thermal;
		__entry->d_thermal = d_thermal;
		__entry->thermal_target = thermal_target;
	),
	TP_PRINT("cpu is %d thermal_err=%d thermal_err_sum=%d thermal_err_diff=%d "
		"p_thermal=%d i_thermal=%d d_thermal=%d thermal_target=%d",
		__entry->cpu, __entry->thermal_err, __entry->thermal_err_sum, __entry->thermal_err_diff,
		__entry->p_thermal, __entry->i_thermal, __entry->d_thermal, __entry->thermal_target
	)
)

TRACE_EVENT(pid_freq, thermal_cooling,
	TP_PROTO(
		const char*, region_name,
		const char*, cooler_name,
		unsigned long long, thermal,
		unsigned long long, quota,
		int, action
	),
	TP_ARGS(region_name, cooler_name, thermal, quota, action),
	TP_FIELD(
		__string(region_name, region_name)
		__string(cooler_name, cooler_name)
		__field(unsigned long long, thermal)
		__field(unsigned long long, quota)
		__field(int, action)
	),
	TP_ASSIGN(
		__assign_str(region_name, region_name);
		__assign_str(cooler_name, cooler_name);
		__entry->thermal = thermal;
		__entry->quota = quota;
		__entry->action = action;
	),
	TP_PRINT("[Thermal] region=%s cooler=%s thermal=%llu quota=%llu action=%d",
		__get_str(region_name), __get_str(cooler_name), __entry->thermal, __entry->quota,
		__entry->action
	)
)

TRACE_EVENT(pid_freq, do_pid_control,
	TP_PROTO(
		long long, curr_temp,
		long long, target,
		long long, err,
		long long, err_sum,
		long long, err_diff,
		long long, p,
		long long, i,
		long long, d,
		long long, sus_thermal,
		long long, output
	),
	TP_ARGS(curr_temp, target, err, err_sum, err_diff, p, i, d, sus_thermal, output),
	TP_FIELD(
		__field(long long, curr_temp)
		__field(long long, target)
		__field(long long, err)
		__field(long long, err_sum)
		__field(long long, err_diff)
		__field(long long, p)
		__field(long long, i)
		__field(long long, d)
		__field(long long, sus_thermal)
		__field(long long, output)
	),
	TP_ASSIGN(
		__entry->curr_temp = curr_temp;
		__entry->target = target;
		__entry->err = err;
		__entry->err_sum = err_sum;
		__entry->err_diff = err_diff;
		__entry->p = p;
		__entry->i = i;
		__entry->d = d;
		__entry->sus_thermal = sus_thermal;
		__entry->output = output;
	),
	TP_PRINT("[Thermal] curr_temp=%lld target=%lld "
		"err=%lld err_sum=%lld err_diff=%lld "
		"p=%lld i=%lld d=%lld sus_thermal=%lld, output=%lld",
		__entry->curr_temp, __entry->target,
		__entry->err, __entry->err_sum, __entry->err_diff,
		__entry->p, __entry->i, __entry->d,
		__entry->sus_thermal, __entry->output
	)
)

TRACE_EVENT(pid_freq, thermal_cluster_cooling,
	TP_PROTO(
		unsigned int, cluster,
		unsigned long long, thermal,
		unsigned long long, weight_thermal,
		unsigned long long, quota,
		unsigned long long, total_load,
		unsigned int, next_freq_indx
	),
	TP_ARGS(cluster, thermal, weight_thermal, quota, total_load, next_freq_indx),
	TP_FIELD(
		__field(unsigned int, cluster)
		__field(unsigned long long, thermal)
		__field(unsigned long long, weight_thermal)
		__field(unsigned long long, quota)
		__field(unsigned long long, total_load)
		__field(unsigned int, next_freq_indx)
	),
	TP_ASSIGN(
		__entry->cluster = cluster;
		__entry->thermal = thermal;
		__entry->weight_thermal = weight_thermal;
		__entry->quota = quota;
		__entry->total_load = total_load;
		__entry->next_freq_indx = next_freq_indx;
	),
	TP_PRINT("[Thermal] cluster%u thermal=%llu w_thermal=%llu "
		"quota=%llu load=%llu next_freq_indx=%u",
		__entry->cluster, __entry->thermal, __entry->weight_thermal,
		__entry->quota, __entry->total_load,  __entry->next_freq_indx
	)
)

TRACE_EVENT(pid_freq, thermal_cluster_limit_freq,
	TP_PROTO(
		const char*, region_name,
		int, cluster,
		unsigned int, opp
	),
	TP_ARGS(region_name, cluster, opp),
	TP_FIELD(
		__string(region_name, region_name)
		__field(int, cluster)
		__field(unsigned long long, opp)
	),
	TP_ASSIGN(
		__assign_str(region_name, region_name);
		__entry->cluster = cluster;
		__entry->opp = opp;
	),
	TP_PRINT("[Thermal] %s limit cpu cluster %d to %u",
		__get_str(region_name), __entry->cluster,
		__entry->opp
	)
)

TRACE_EVENT(pid_freq, thermal_cooler_limit_freq,
	TP_PROTO(
		const char*, region_name,
		const char*, cooler_name,
		unsigned int, opp
	),
	TP_ARGS(region_name, cooler_name, opp),
	TP_FIELD(
		__string(region_name, region_name)
		__string(cooler_name, cooler_name)
		__field(unsigned long long, opp)
	),
	TP_ASSIGN(
		__assign_str(region_name, region_name);
		__assign_str(cooler_name, cooler_name);
		__entry->opp = opp;
	),
	TP_PRINT("[Thermal] %s limit %s to %u",
		__get_str(region_name), __get_str(cooler_name),
		__entry->opp
	)
)
