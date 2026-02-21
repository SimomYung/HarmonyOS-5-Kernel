/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: wbt tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 06 15:30:54 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#include <trace/fs_trace_templates.h>

TRACE_EVENT(wbt, wbt_stat,
		TP_PROTO(struct block_fi_rq_stat *, stat),
		TP_ARGS(stat),
		TP_FIELD(
			__field(uint64_t, rmin)
			__field(int32_t, rnr_samples)
			__field(uint64_t, wmin)
			__field(int32_t, wnr_samples)
			__field(uint64_t, frmin)
			__field(int32_t, frnr_samples)
			__field(uint64_t, fwmin)
			__field(int32_t, fwnr_samples)
		),
		TP_ASSIGN(
			__entry->rmin = stat[0].min;
			__entry->rnr_samples = stat[0].nr_samples;
			__entry->wmin = stat[1].min;
			__entry->wnr_samples = stat[1].nr_samples;
			__entry->frmin = stat[2].min;
			__entry->frnr_samples = stat[2].nr_samples;
			__entry->fwmin = stat[3].min;
			__entry->fwnr_samples = stat[3].nr_samples;
		),
		TP_PRINT("rmin=%llu, rsamples=%d, wmin=%llu, wsamples=%d, "
			"frmin=%llu, frsamples=%d, fwmin=%llu, fwsamples=%d",
			__entry->rmin, __entry->rnr_samples, __entry->wmin, __entry->wnr_samples,
			__entry->frmin, __entry->frnr_samples, __entry->fwmin, __entry->fwnr_samples)
)

TRACE_EVENT(wbt, wbt_lat,
		TP_PROTO(uint64_t, lat),
		TP_ARGS(lat),
		TP_FIELD(
			__field(uint64_t, lat)
		),
		TP_ASSIGN(
			__entry->lat = (lat / 1000);
		),
		TP_PRINT("latency %lluus", __entry->lat)
)

TRACE_EVENT(wbt, wbt_step,
		TP_PROTO(const char *, msg, int, step, uint64_t, window,
			unsigned int, bg, unsigned int, normal, unsigned int, max),
		TP_ARGS(msg, step, window, bg, normal, max),
		TP_FIELD(
			__field(const char *, msg)
			__field(int, step)
			__field(uint64_t, window)
			__field(unsigned int, bg)
			__field(unsigned int, normal)
			__field(unsigned int, max)
		),
		TP_ASSIGN(
			__entry->msg = msg;
			__entry->step = step;
			__entry->window = window / 1000;
			__entry->bg = bg;
			__entry->normal = normal;
			__entry->max = max;
		),
		TP_PRINT("%s: step=%d, window=%lluus, background=%u, normal=%u, max=%u",
			__entry->msg, __entry->step, __entry->window,
			__entry->bg, __entry->normal, __entry->max)
)

TRACE_EVENT(wbt, wbt_timer,
		TP_PROTO(int, status, int, step, unsigned int, inflight),
		TP_ARGS(status, step, inflight),
		TP_FIELD(
			__field(int, status)
			__field(int, step)
			__field(unsigned int, inflight)
		),
		TP_ASSIGN(
			__entry->status = status;
			__entry->step = step;
			__entry->inflight = inflight;
		),
		TP_PRINT("status=%d, step=%d, inflight=%u",
			__entry->status, __entry->step, __entry->inflight)
)
