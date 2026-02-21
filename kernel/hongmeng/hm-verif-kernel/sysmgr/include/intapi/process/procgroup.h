/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definitions of process group operations
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 12:58:22 2019
 */
#ifndef SYSMGR_INCLUDE_INTAPI_PROCESS_PROCGROUP_H
#define SYSMGR_INCLUDE_INTAPI_PROCESS_PROCGROUP_H

#include <intapi/process/proc_core.h>

struct pid_s *get_pid_s_by_pgid(pid_t pgid, struct pid_ns *caller_ns);
void process_leave_group(struct process_s *process);
void process_move_into_pgroup(struct process_s *process, struct pid_s *pgid);

static inline void pgroup_set_sid(struct pid_s *pgid, struct pid_s *sid)
{
	pgid->sid = sid;
}
int process_pgrp_isorphaned(struct process_s *process);

/*
 * foreach operations need to be protected with lock
 */
#define procgroup_foreach_process_in_group(proc, pgroup)	\
	struct process_s *_next_proc = NULL;	\
	dlist_for_each_entry_safe(proc, _next_proc, &(pgroup)->heads[PIDTYPE_PGID],	\
				  struct process_s, pid_link.pid_links[PIDTYPE_PGID])

#endif
