/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: erofs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 09 15:30:54 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(erofs, erofs_raw_access_readpages_start,
	TP_PROTO(unsigned long long, index, unsigned int, nr_pages, unsigned long, nid),
	TP_ARGS(index, nr_pages, nid),
	TP_FIELD(
		__field(unsigned long long, index)
		__field(unsigned int, nr_pages)
		__field(unsigned long, nid)
	),
	TP_ASSIGN(
		__entry->index = index;
		__entry->nr_pages = nr_pages;
		__entry->nid = nid;
	),
	TP_PRINT("index:%llu nr_pages:%u nid:%lu", __entry->index, __entry->nr_pages, __entry->nid)
)
 
TRACE_EVENT(erofs, erofs_raw_access_readpages_end,
	TP_PROTO(unsigned long, nid, int, res),
	TP_ARGS(nid, res),
	TP_FIELD(
		__field(unsigned long, nid)
		__field(int, res)
	),
	TP_ASSIGN(
		__entry->nid = nid;
		__entry->res = res;
	),
	TP_PRINT("nid:%lu res:%d", __entry->nid, __entry->res)
)
 
TRACE_EVENT(erofs, erofs_read_raw_page_start,
	TP_PROTO(unsigned long long, index, unsigned long, nid),
	TP_ARGS(index, nid),
	TP_FIELD(
		__field(unsigned long long, index)
		__field(unsigned long, nid)
	),
	TP_ASSIGN(
		__entry->index = index;
		__entry->nid = nid;
	),
	TP_PRINT("index:%llu nid:%lu", __entry->index, __entry->nid)
)
 
TRACE_EVENT(erofs, erofs_read_raw_page_end,
	TP_PROTO(unsigned long, nid, int, res),
	TP_ARGS(nid, res),
	TP_FIELD(
		__field(unsigned long, nid)
		__field(int, res)
	),
	TP_ASSIGN(
		__entry->nid = nid;
		__entry->res = res;
	),
	TP_PRINT("nid:%lu res:%d", __entry->nid, __entry->res)
)
 
TRACE_EVENT(erofs, z_erofs_vle_normalaccess_readpages_start,
	TP_PROTO(unsigned long long, index, unsigned int, nr_pages, unsigned long, nid),
	TP_ARGS(index, nr_pages, nid),
	TP_FIELD(
		__field(unsigned long long, index)
		__field(unsigned int, nr_pages)
		__field(unsigned long, nid)
	),
	TP_ASSIGN(
		__entry->index = index;
		__entry->nr_pages = nr_pages;
		__entry->nid = nid;
	),
	TP_PRINT("index:%llu nr_pages:%u nid:%lu", __entry->index, __entry->nr_pages, __entry->nid)
)
 
TRACE_EVENT(erofs, z_erofs_vle_normalaccess_readpages_end,
	TP_PROTO(unsigned long, nid, int, res),
	TP_ARGS(nid, res),
	TP_FIELD(
		__field(unsigned long, nid)
		__field(int, res)
	),
	TP_ASSIGN(
		__entry->nid = nid;
		__entry->res = res;
	),
	TP_PRINT("nid:%lu res:%d", __entry->nid, __entry->res)
)
 
TRACE_EVENT(erofs, z_erofs_vle_normalaccess_readpage_start,
	TP_PROTO(unsigned long long, index, unsigned long, nid),
	TP_ARGS(index, nid),
	TP_FIELD(
		__field(unsigned long long, index)
		__field(unsigned long, nid)
	),
	TP_ASSIGN(
		__entry->index = index;
		__entry->nid = nid;
	),
	TP_PRINT("index:%llu nid:%lu", __entry->index, __entry->nid)
)
 
TRACE_EVENT(erofs, z_erofs_vle_normalaccess_readpage_end,
	TP_PROTO(unsigned long, nid, int, res),
	TP_ARGS(nid, res),
	TP_FIELD(
		__field(unsigned long, nid)
		__field(int, res)
	),
	TP_ASSIGN(
		__entry->nid = nid;
		__entry->res = res;
	),
	TP_PRINT("nid:%lu res:%d", __entry->nid, __entry->res)
)
