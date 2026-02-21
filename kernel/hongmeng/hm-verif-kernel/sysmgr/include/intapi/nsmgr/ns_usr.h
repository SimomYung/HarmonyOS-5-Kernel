/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Structures and interfaces for user namespace
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 07 14:35:21 2023
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_USR_H
#define SYSMGR_INCLUDE_NSMGR_NS_USR_H

#include <lib/utils.h>
#include <hongmeng/types.h>
#include <libsec/cred.h>
#include <intapi/nsmgr/ns_common.h>
#include <intapi/nsmgr/usr_count.h>

#ifndef CLONE_NEWUSER
#define CLONE_NEWUSER	(0x10000000)
#endif

#define USERNS_SETGROUPS_ALLOWED 1UL

/* usr_ns is referenced by process security credential & procfs userns files
 * usr_ns_cred is referenced by thread base_cred, process security base_cred & usr_ns
 */
struct usr_ns {
	struct ns_common ns_common;
	struct ugid_map uid_map;
	struct ugid_map gid_map;
	struct ugid_map projid_map;
	int level;
	uid_t owner;
	gid_t group;
	unsigned long flags;

	/* current idx & usr_ns_cred pointer */
	cred_idx_t idx;
	struct usr_ns_cred *usr_ns_cred;

	/* parent usr_ns pointer */
	struct usr_ns *parent;

	/* /proc/sys/user/xxx max count */
	int usr_count_max[USR_COUNT_MAX];
	struct usr_count *usr_count;
};

struct ns_operation *fetch_usr_ns_op(void);
void set_usr_ns_cred(struct usr_ns_cred *targ_usrns_cred, struct usr_ns *src);
bool userns_can_setgroups(struct thread_s *thread);

static inline int
usr_ns_get(struct usr_ns *ns)
{
	return ns_common_get(&ns->ns_common);
}

static inline void
usr_ns_put(struct usr_ns *ns)
{
	return ns_common_put(&ns->ns_common);
}

static inline struct usr_ns *
common_to_usr_ns(struct ns_common *ns)
{
	return container_of(ns, struct usr_ns, ns_common);
}

#ifdef CONFIG_USER_NS
struct thread_s;
struct process_s;
extern int create_usr_ns(struct base_cred *cred, struct usr_ns **usrns_new);

#define DECLARE_PROCFS_IDMAP(idmap)								\
extern int procfs_tid_##idmap##_read(struct thread_s *thread, char *data, size_t data_len);	\
extern int procfs_tid_##idmap##_write(struct thread_s *thread, uint64_t pos, const char *src,	\
				      size_t size, size_t *wsize);				\
extern int procfs_pid_##idmap##_read(struct process_s *process, char *data, size_t data_len);	\
extern int procfs_pid_##idmap##_write(struct process_s *process, uint64_t pos, const char *src,	\
				      size_t size, size_t *wsize);				\

DECLARE_PROCFS_IDMAP(uid_map);
DECLARE_PROCFS_IDMAP(gid_map);
DECLARE_PROCFS_IDMAP(projid_map);
DECLARE_PROCFS_IDMAP(setgroups);

#else
static inline int create_usr_ns(struct base_cred *cred, struct usr_ns **usrns_new)
{
	UNUSED(cred, usrns_new);
	return E_HM_INVAL;
}
#endif

#endif
