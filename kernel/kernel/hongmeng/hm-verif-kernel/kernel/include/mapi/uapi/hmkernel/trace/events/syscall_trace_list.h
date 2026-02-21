/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Syscall Kernel tracepoints
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 20 17:20:17 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(syscall, syscall,
	TP_PROTO(unsigned int, scno, unsigned int, is_fastpath),
	TP_ARGS(scno, is_fastpath),
	TP_FIELD(
		__field(__u32, sequence)
		__field(__u32, scno)
		__field(__u32, is_fastpath)
	),
	TP_ASSIGN(
		TRACE_READ_SEQ(__entry->sequence);
		__entry->scno = scno;
		__entry->is_fastpath = is_fastpath;
	),
	TP_PRINT(
		"[seq=%u] %u %s", __entry->sequence, __entry->scno,
		(__entry->is_fastpath != 0 ? "fastpath" : "slowpath")
	)
)
