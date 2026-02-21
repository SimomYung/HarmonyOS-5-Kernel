/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Process session
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 14 15:27:01 2019
 */
#ifndef SYSMGR_INCLUDE_INTAPI_PROCESS_SESSION_H
#define SYSMGR_INCLUDE_INTAPI_PROCESS_SESSION_H

#include <intapi/process/process.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <limits.h>

struct session {
	struct dlist_node node;
	struct pid_s *sid;
	pid_t fg_pgrp;
	struct ttyinfo_s ttyinfo;
	tty_node_t *tty_node;
};

void sessionpool_init(void);
int process_create_session(struct process_s *process);
void sessionpool_delete_session(const struct pid_s *sid);
int session_setfgpgrp(const struct pid_s *sid, pid_t pgid);
pid_t session_fgpgrp_of(const struct pid_s *sid);
void process_leave_session(struct process_s *process);
int session_ttyname(const struct pid_s *sid, char *name, size_t name_len);
int session_set_ttyinfo(const struct pid_s *sid, const struct ttyinfo_s *ttyinfo);
int session_get_ttyinfo(const struct pid_s *sid, struct ttyinfo_s *ttyinfo);

static inline char *
session_name_of(struct session *session)
{
	return session->ttyinfo.name;
}

/*
 * foreach operations need to be protected by lock
 */
#define sessionpool_foreach_session(session, sessionpool)				\
	struct session *_next_session = NULL;					\
	dlist_for_each_entry_safe((session), _next_session, &(sessionpool)->head,	\
				  struct session, node)

/*
 * foreach operations need to be protected by lock
 */
#define session_foreach_process_in_session(process, sid)					\
	struct process_s *_next_process = NULL;							\
	dlist_for_each_entry_safe((process), _next_process, &(sid)->heads[PIDTYPE_SID],	\
				  struct process_s, pid_link.pid_links[PIDTYPE_SID])

#endif
