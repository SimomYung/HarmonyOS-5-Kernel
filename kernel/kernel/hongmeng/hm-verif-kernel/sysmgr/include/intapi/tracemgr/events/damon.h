/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: DAMON tracepoint list
 * Author: Huawei CBG OS Lab
 * Create: Wed Mar 20 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(damon, damos_before_apply,
	TP_PROTO(unsigned int, context_idx, unsigned int, scheme_idx,
		unsigned int, target_idx, struct damon_region *, r,
		unsigned int, nr_regions),
	TP_ARGS(context_idx, target_idx, scheme_idx, r, nr_regions),
	TP_FIELD(
		__field(unsigned int,	context_idx)
		__field(unsigned int,	scheme_idx)
		__field(unsigned long,	target_idx)
		__field(unsigned long,	start)
		__field(unsigned long,	end)
		__field(unsigned int,	nr_accesses)
		__field(unsigned int,	age)
		__field(unsigned int,	nr_regions)
	),
	TP_ASSIGN(
		__entry->context_idx	= context_idx;
		__entry->scheme_idx	= scheme_idx;
		__entry->target_idx	= target_idx;
		__entry->start		= r->addr_region.start;
		__entry->end		= r->addr_region.end;
		/* Basepoints conversion, see DAMON_BP_RATIO */
		__entry->nr_accesses	= r->nr_accesses_bp / 10000;
		__entry->age		= r->age;
		__entry->nr_regions	= nr_regions;
	),
	TP_PRINT("ctx_idx=%u scheme_idx=%u target_idx=%lu nr_regions=%u %lu-%lu: %u %u",
		__entry->context_idx, __entry->scheme_idx,
		__entry->target_idx, __entry->nr_regions,
		__entry->start, __entry->end,
		__entry->nr_accesses, __entry->age)
)

TRACE_EVENT(damon, damon_aggregated,
	TP_PROTO(unsigned int, target_id, struct damon_region *, r,
		 struct damon_target *, target),
	TP_ARGS(target_id, r, target),
	TP_FIELD(
		__field(unsigned long,	target_id)
		__field(unsigned int,	nr_regions)
		__field(unsigned long,	start)
		__field(unsigned long,	end)
		__field(unsigned int,	nr_accesses)
		__field(unsigned int,	age)
	),
	TP_ASSIGN(
		__entry->target_id	= target_id;
		__entry->nr_regions	= target->nr_regions;
		__entry->start		= r->addr_region.start;
		__entry->end		= r->addr_region.end;
		__entry->nr_accesses	= r->nr_accesses;
		__entry->age		= r->age;
	),
	TP_PRINT("target_id=%lu nr_regions=%u %lu-%lu: %u %u",
		__entry->target_id, __entry->nr_regions,
		__entry->start, __entry->end,
		__entry->nr_accesses, __entry->age)
)

TRACE_EVENT(damon, kdamond_start,
	TP_PROTO(unsigned int, id, unsigned int, tid),
	TP_ARGS(id, tid),
	TP_FIELD(
		__field(unsigned int, id)
		__field(unsigned int, tid)
	),
	TP_ASSIGN(
		__entry->id	= id;
		__entry->tid	= tid;
	),
	TP_PRINT("start id=%u tid=%u",
		__entry->id, __entry->tid)
)

TRACE_EVENT(damon, kdamond_finish,
	TP_PROTO(unsigned int, id, unsigned int, tid),
	TP_ARGS(id, tid),
	TP_FIELD(
		__field(unsigned int, id)
		__field(unsigned int, tid)
	),
	TP_ASSIGN(
		__entry->id	= id;
		__entry->tid	= tid;
	),
	TP_PRINT("finish id=%u tid=%u",
		__entry->id, __entry->tid)
)
