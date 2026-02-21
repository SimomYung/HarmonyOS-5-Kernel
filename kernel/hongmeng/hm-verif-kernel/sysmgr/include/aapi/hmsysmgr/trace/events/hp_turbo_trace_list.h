/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Tracepoint list
 * Author: HISI Kirin Memory
 * Create: Wed Feb 25 16:16:35 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(hp_turbo, hp_turbo_read_start,
	TP_PROTO(unsigned long long, dev_no,
		unsigned int, nr_exts,
		unsigned int, nr_ext_pages,
		unsigned int, page_size),
	TP_ARGS(dev_no, nr_exts, nr_ext_pages, page_size),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(unsigned int, nr_exts)
		__field(unsigned int, nr_ext_pages)
		__field(unsigned int, page_size)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->nr_exts = nr_exts;
		__entry->nr_ext_pages = nr_ext_pages;
		__entry->page_size = page_size;
	),
	TP_PRINT("dev_no=%llu nr_exts=%u nr_ext_pages=%u page_size=%u total=%llu",
		__entry->dev_no, __entry->nr_exts, __entry->nr_ext_pages, __entry->nr_ext_pages,
		(unsigned long long)__entry->nr_exts * __entry->nr_ext_pages * __entry->nr_ext_pages)
)

TRACE_EVENT(hp_turbo, hp_turbo_read_end,
	TP_PROTO(unsigned long long, dev_no,
		int, rc),
	TP_ARGS(dev_no, rc),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(int, rc)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->rc = rc;
	),
	TP_PRINT("dev_no=%llu rc=%d", __entry->dev_no, __entry->rc)
)

TRACE_EVENT(hp_turbo, hp_turbo_write_start,
	TP_PROTO(unsigned long long, dev_no,
		unsigned int, nr_exts,
		unsigned int, nr_ext_pages,
		unsigned int, page_size),
	TP_ARGS(dev_no, nr_exts, nr_ext_pages, page_size),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(unsigned int, nr_exts)
		__field(unsigned int, nr_ext_pages)
		__field(unsigned int, page_size)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->nr_exts = nr_exts;
		__entry->nr_ext_pages = nr_ext_pages;
		__entry->page_size = page_size;
	),
	TP_PRINT("dev_no=%llu nr_exts=%u nr_ext_pages=%u page_size=%u total=%llu",
		__entry->dev_no, __entry->nr_exts, __entry->nr_ext_pages, __entry->nr_ext_pages,
		(unsigned long long)__entry->nr_exts * __entry->nr_ext_pages * __entry->nr_ext_pages)
)

TRACE_EVENT(hp_turbo, hp_turbo_write_end,
	TP_PROTO(unsigned long long, dev_no,
		int, rc),
	TP_ARGS(dev_no, rc),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(int, rc)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->rc = rc;
	),
	TP_PRINT("dev_no=%llu rc=%d", __entry->dev_no, __entry->rc)
)

TRACE_EVENT(hp_turbo, hp_turbo_read_issue,
	TP_PROTO(unsigned long long, dev_no,
		unsigned int, nr_exts,
		int, min_eid,
		unsigned int, nr_iov,
		unsigned long long, pos,
		uint32_t, page_size),
	TP_ARGS(dev_no, nr_exts, min_eid, nr_iov, pos, page_size),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(unsigned int, nr_exts)
		__field(int, min_eid)
		__field(unsigned int, nr_iov)
		__field(unsigned long long, pos)
		__field(unsigned int, page_size)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->nr_exts = nr_exts;
		__entry->min_eid = min_eid;
		__entry->nr_iov = nr_iov;
		__entry->pos = pos;
		__entry->page_size = page_size;
	),
	TP_PRINT("dev_no=%llu nr_exts=%u min_eid=%d nr_iov=%u pos=%u page_size=%u total=%llu",
		__entry->dev_no, __entry->nr_exts, __entry->min_eid, __entry->nr_iov,
		 __entry->pos, __entry->page_size,
		(unsigned long long)__entry->page_size * __entry->nr_iov)
)

TRACE_EVENT(hp_turbo, hp_turbo_read_complete,
	TP_PROTO(unsigned long long, dev_no,
		int, min_eid,
		unsigned long long, rc),
	TP_ARGS(dev_no, min_eid, rc),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(int, min_eid)
		__field(unsigned long long, rc)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->min_eid = min_eid;
		__entry->rc = rc;
	),
	TP_PRINT("dev_no=%llu min_eid=%d result=%llu", __entry->dev_no, __entry->min_eid, __entry->rc)
)

TRACE_EVENT(hp_turbo, hp_turbo_write_issue,
	TP_PROTO(unsigned long long, dev_no,
		unsigned int, nr_exts,
		int, min_eid,
		unsigned int, nr_iov,
		unsigned long long, pos,
		unsigned int, page_size),
	TP_ARGS(dev_no, nr_exts, min_eid, nr_iov, pos, page_size),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(unsigned int, nr_exts)
		__field(int, min_eid)
		__field(unsigned int, nr_iov)
		__field(unsigned long long, pos)
		__field(unsigned int, page_size)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->nr_exts = nr_exts;
		__entry->min_eid = min_eid;
		__entry->nr_iov = nr_iov;
		__entry->pos = pos;
		__entry->page_size = page_size;
	),
	TP_PRINT("dev_no=%llu nr_exts=%u min_eid=%d nr_iov=%u pos=%u page_size=%u total=%llu",
		__entry->dev_no, __entry->nr_exts, __entry->min_eid, __entry->nr_iov,
		 __entry->pos, __entry->page_size,
		(unsigned long long)__entry->page_size * __entry->nr_iov)
)

TRACE_EVENT(hp_turbo, hp_turbo_write_complete,
	TP_PROTO(unsigned long long, dev_no,
		int, min_eid,
		unsigned long long, rc),
	TP_ARGS(dev_no, min_eid, rc),
	TP_FIELD(
		__field(unsigned long long, dev_no)
		__field(int, min_eid)
		__field(unsigned long long, rc)
	),
	TP_ASSIGN(
		__entry->dev_no = dev_no;
		__entry->min_eid = min_eid;
		__entry->rc = rc;
	),
	TP_PRINT("dev_no=%llu min_eid=%d result=%llu", __entry->dev_no, __entry->min_eid, __entry->rc)
)
