/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: trace of io_uring
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 22 15:30:54 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#include <trace/fs_trace_templates.h>

/**
 *
 * @ring_ctx: pointer to a ring context structure
 * @hf_ctx: pointer to hyperframe context with the ring context
 * @opcode: opcode of request
 * @user_data: user data associated with the request
 * @result: hyperframe request return result
 * @ion: whether ion buffer
 * @fd: hyperframe file fd
 *
 * Allows to track SQE from submitting to completing.
 */

TRACE_EVENT(hyperframe, hyperframe_req_init,
	TP_PROTO(void *, ring_ctx, void *, hf_ctx, uint8_t, opcode, uint64_t, ud, int, fd),
	TP_ARGS(ring_ctx, hf_ctx, opcode, ud, fd),
	TP_FIELD(
		__field(void *, ring_ctx)
		__field(void *, hf_ctx)
		__field(uint8_t, opcode)
		__field(uint64_t, ud)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->ring_ctx = ring_ctx;
		__entry->hf_ctx = hf_ctx;
		__entry->opcode = opcode;
		__entry->ud = ud;
		__entry->fd = fd;
	),
	TP_PRINT("ring:%pK - %pK, op:%d, user_data:0x%llx, fd:%d",
		__entry->ring_ctx, __entry->hf_ctx, __entry->opcode,
		(unsigned long long) __entry->ud, __entry->fd)
)

TRACE_EVENT(hyperframe, hyperframe_add_list,
	TP_PROTO(void *, hf_ctx, uint8_t, opcode, bool, ion, int, fd),
	TP_ARGS(hf_ctx, opcode, ion, fd),
	TP_FIELD(
		__field(void *, hf_ctx)
		__field(uint8_t, opcode)
		__field(bool, ion)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->hf_ctx = hf_ctx;
		__entry->opcode = opcode;
		__entry->ion = ion;
		__entry->fd = fd;
	),
	TP_PRINT("ring:%pK, op:%d, ion:%d, fd:%d",
		__entry->hf_ctx, __entry->opcode,
		__entry->ion, __entry->fd)
)

TRACE_EVENT(hyperframe, hyperframe_work_start,
	TP_PROTO(void *, hf_ctx, uint8_t, opcode, int, io_cnt, int, fd),
	TP_ARGS(hf_ctx, opcode, io_cnt, fd),
	TP_FIELD(
		__field(void *, hf_ctx)
		__field(uint8_t, opcode)
		__field(int, io_cnt)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->hf_ctx = hf_ctx;
		__entry->opcode = opcode;
		__entry->io_cnt = io_cnt;
		__entry->fd = fd;
	),
	TP_PRINT("ring:%pK, op:%d, io_count:%d, fd:%d",
		__entry->hf_ctx, __entry->opcode,
		__entry->io_cnt, __entry->fd)
)

TRACE_EVENT(hyperframe, hyperframe_req_dispatch,
	TP_PROTO(uint8_t, opcode, ssize_t, result, int, fd),
	TP_ARGS(opcode, result, fd),
	TP_FIELD(
		__field(uint8_t, opcode)
		__field(ssize_t, result)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->opcode = opcode;
		__entry->result = result;
		__entry->fd = fd;
	),
	TP_PRINT("op:%d, result:%zd, fd:%d",
		__entry->opcode, __entry->result, __entry->fd)
)

TRACE_EVENT(hyperframe, hyperframe_irq_complete,
	TP_PROTO(void *, hf_ctx, uint8_t, opcode, int, comp_cnt, int, fd),
	TP_ARGS(hf_ctx, opcode, comp_cnt, fd),
	TP_FIELD(
		__field(void *, hf_ctx)
		__field(uint8_t, opcode)
		__field(int, comp_cnt)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->hf_ctx		= hf_ctx;
		__entry->opcode		= opcode;
		__entry->comp_cnt	= comp_cnt;
		__entry->fd			= fd;
	),
	TP_PRINT("ring:%pK, op:%d, comp_count:%d, fd:%d",
			  __entry->hf_ctx, __entry->opcode,
			  __entry->comp_cnt, __entry->fd)
)

TRACE_EVENT(hyperframe, hyperframe_fill_cqe,
	TP_PROTO(void *, hf_ctx, uint8_t, opcode, int, disp_count,
			int, comp_count, int, fd),
	TP_ARGS(hf_ctx, opcode, disp_count, comp_count, fd),
	TP_FIELD(
		__field(void *, hf_ctx)
		__field(uint8_t, opcode)
		__field(int, disp_count)
		__field(int, comp_count)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->hf_ctx = hf_ctx;
		__entry->opcode = opcode;
		__entry->disp_count = disp_count;
		__entry->comp_count = comp_count;
		__entry->fd = fd;
	),
	TP_PRINT("ring:%pK, op:%d, disp:%d, comp:%d, fd:%d",
		__entry->hf_ctx, __entry->opcode, __entry->disp_count,
		__entry->comp_count, __entry->fd)
)

TRACE_EVENT(hyperframe, hyperframe_req_completed,
	TP_PROTO(uint8_t, opcode, uint64_t, ud, uint32_t, result, int, ref, int, fd),
	TP_ARGS(opcode, ud, result, ref, fd),
	TP_FIELD(
		__field(uint8_t, opcode)
		__field(uint64_t, ud)
		__field(uint32_t, result)
		__field(int, ref)
		__field(int, fd)
	),
	TP_ASSIGN(
		__entry->opcode = opcode;
		__entry->ud = ud;
		__entry->result = result;
		__entry->ref = ref;
		__entry->fd = fd;
	),
	TP_PRINT("op:%d, user_data:0x%llx, ret:%u, ref:%d, fd:%d",
		__entry->opcode, (unsigned long long) __entry->ud,
		__entry->result, __entry->ref, __entry->fd)
)
