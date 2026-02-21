/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: vfs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 18 11:42:15 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(vfs, hkids_do_mount,
	TP_PROTO(const char *, src, const char *, tgt, const char *, fstype, unsigned long, flag, const char *, opts),
	TP_ARGS(src, tgt, fstype, flag, opts),

	TP_FIELD(
		__field(unsigned long, flag)
	),
	TP_ASSIGN(
		__entry->flag = flag;
	),
	TP_PRINT("do_mount flag=%lx",
		__entry->flag
	)
)
