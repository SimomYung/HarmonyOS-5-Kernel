/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: trace list for cgroup
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 21 04:16:58 2025
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(cgroup, cgroup_attach_task,
	TP_PROTO(struct res_group *, rgrp, struct process_s *, process, const char *, comm),
	TP_ARGS(rgrp, process, comm),
	TP_FIELD(
		__field(int, dst_root)
		__field(int, dst_level)
		__field(__u64, dst_id)
		__field(int, pid)
		__string(dst_path, rgrp->hash_key)
		__array(char, comm, HMCAP_THREAD_NAME_SIZE)
	),
	TP_ASSIGN(
		__entry->dst_root = 0;
		__entry->dst_id = 0;
		__entry->dst_level = 0;
		__entry->pid = (process == NULL ? PID_NONE : process_pid_of(process));
		strncpy_s(__entry->comm, HMCAP_THREAD_NAME_SIZE, comm, strnlen(comm, HMCAP_THREAD_NAME_SIZE - 1));
		__assign_str(dst_path, rgrp->hash_key);
	),
	TP_PRINT("dst_root=%d dst_id=%llu dst_level=%d dst_path=%s pid=%d comm=%s",
		__entry->dst_root, __entry->dst_id, __entry->dst_level, __get_str(dst_path), __entry->pid, __entry->comm
	)
)

#define DEFINE_CGROUP_EVENT(FUNC)                                           \
	TRACE_EVENT(cgroup, FUNC,                                           \
		TP_PROTO(struct res_group *, rgrp),                         \
		TP_ARGS(rgrp),						    \
		TP_FIELD(                                                   \
			__field(int, root)                                  \
			__field(int, level)                                 \
			__field(__u64, id)                                  \
			__string(path, rgrp->hash_key)                      \
		),                                                          \
		TP_ASSIGN(                                                  \
			__entry->root = 0;                                  \
			__entry->id = 0;                                   \
			__entry->level = 0;                                 \
			__assign_str(path, rgrp->hash_key);                 \
		),                                                          \
		TP_PRINT("root=%d id=%llu level=%d path=%s",               \
			__entry->root, __entry->id, __entry->level, __get_str(path)                                \
		)                                                           \
	)

DEFINE_CGROUP_EVENT(cgroup_mkdir)
DEFINE_CGROUP_EVENT(cgroup_rmdir)
DEFINE_CGROUP_EVENT(cgroup_freeze)
DEFINE_CGROUP_EVENT(cgroup_unfreeze)
