/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ns group contains all the namespaces for a thread.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 28 16:33:44 2020
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_GROUP_H
#define SYSMGR_INCLUDE_NSMGR_NS_GROUP_H

#include <intapi/nsmgr/ns_common.h>
#include <intapi/nsmgr/ns_mnt.h>
#include <intapi/nsmgr/ns_net.h>
#include <intapi/nsmgr/ns_uts.h>
#include <intapi/nsmgr/ns_ipc.h>

/*
 * For ns_group, each thread structure has a ns_group.
 * If CLONE_NS is not set or setns never called in a process,
 * child will share the same ns_group structure with parent.
 *
 * If child shares the same ns_group with parent, it should get
 * ns_group refcnt when child created.
 *
 * For ns_group access rules:
 * 1. A thread created will get the ns_group refcnt and will put it
 *    when the thread terminated(structure freed).
 *
 *    So if we hold a thread refcnt, the thread holds the ns_group refcnt
 *    the ns_group will be safe.
 *
 *    When put ns_refcnt, must release thread first then put ns_group,
 *    it will be safe even don't have any lock.
 *
 * 2. ns_group will hold each sub-namespace structure reference count.
 *    - when switch sub-namespace, just increases refcnt of new ns and decrease
 *      the old ns refcnt.
 *    - when destroy ns_group, just put all sub-namespace structure will be fine.
 */
struct base_cred;

struct ns_group_s {
	struct raw_refcnt_nr refcnt;
	struct mnt_ns *mntns;
	struct net_ns *netns;
	struct uts_ns *utsns;
	struct pid_ns *pidns_for_children;
	struct ipc_ns *ipcns;
};

void ns_group_destroy(struct ns_group_s *ns);
int ns_group_copy(struct thread_s *new,
		  struct thread_s *current,
		  unsigned int flags);
int ns_group_create(struct thread_s *new,
				 struct thread_s *current,
				 unsigned int flags, struct ns_group_s **ns_group_new);

static inline int
ns_group_get(struct ns_group_s *group)
{
	return raw_refcnt_nr_get(&group->refcnt);
}

static inline void
ns_group_put(struct ns_group_s *group)
{
	int cnt = raw_refcnt_nr_put(&group->refcnt);
	if (cnt == 0) {
		ns_group_destroy(group);
	}
}

void ns_group_init_root(void);
struct ns_group_s *root_ns_group(void);
struct ns_group_s *ns_group_get_root(void);
void update_cred_nsid_group(struct base_cred *cred, struct thread_s *thread);

#endif
