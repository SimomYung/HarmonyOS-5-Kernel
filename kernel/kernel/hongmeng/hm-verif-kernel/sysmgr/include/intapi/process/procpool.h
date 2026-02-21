/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for process pool interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:58:22 2019
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROCPOOL_H
#define SYSMGR_INTAPI_PROCESS_PROCPOOL_H

#include <hmkernel/capability.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libalgo/rbtree.h>
#include <lib/utils.h>
#include <lib/dlist.h>
#include <unistd.h>
#include <stdint.h>

struct procpool_local_info {
	unsigned long tbl_index;
	struct dlist_node head;
};

#define PROCPOOL_LOCAL_INFO_INIT(name)			\
struct procpool_local_info name =			\
{							\
	.tbl_index = ULONG_MAX,				\
	.head = DLIST_HEAD_INIT(name.head)		\
}

#define MAX_DLIST_NODE		32

struct procpool_nodes {
	struct rbt_node cnode_idx_rbt_node;
	struct rbt_node cnode_hdlr_rbt_node;
	struct dlist_node proc_dlist_node;
	struct dlist_node dnode_tbl[MAX_DLIST_NODE];
};

struct process_s;
struct pid_ns;

void procpool_init(void);
void procpool_add_proc_dlist(struct process_s *process);
int procpool_add_cnode_hdlr(struct process_s *process);
int procpool_add_cnode_idx(struct process_s *process);
void procpool_clean_cnode_idx(struct process_s *process);
void procpool_clean_cnode_hdlr(struct process_s *process);
void procpool_clean_proc_dlist(struct process_s *process);
void procpool_cleanup(struct process_s *process);
struct raw_rwlock *procpool_rwlock(void);
struct raw_rwlock *procpool_dlist_rwlock(void);
struct process_s *procpool_lookup_cnode_idx_nolock(uint32_t cnode_idx);
struct process_s *procpool_lookup_cnode_idx(uint32_t cnode_idx);
struct process_s *procpool_get_process_by_cnode_idx(uint32_t cnode_idx);
struct process_s *procpool_lookup_cnode_hdlr_nolock(rref_t cnode_hdlr_rref);
struct process_s *procpool_lookup_cnode_hdlr(rref_t cnode_hdlr_rref);
int do_procpool_for_each_process(int (*fn)(struct process_s *, void *),
				 void *data, struct pid_ns *ns);
int procpool_for_each_process(int (*fn)(struct process_s *, void *),
			      void *data, struct pid_ns *ns);
int procpool_for_each_process_nolock(int (*fn)(struct process_s *, void *),
				     void *data, struct pid_ns *ns);
int procpool_for_each_process_trylock(int (*fn)(struct process_s *, void *),
				      void *data, struct pid_ns *ns);
int procpool_get_processes_nr(void);
void procpool_dlist_rdlock(void);
void procpool_dlist_rwlock_unlock(void);

void procpool_alloc_local_slot(struct procpool_local_info *info);
void procpool_free_local_slot(struct procpool_local_info *info);
int procpool_for_local_proclist(int (*fn)(struct process_s *, void *),
				void *data, struct pid_ns *ns,
				struct procpool_local_info *info);
int procpool_try_collect_local_proclist(struct procpool_local_info *info);
int procpool_for_each_process_local(int (*fn)(struct process_s *, void *),
				    void *data, struct pid_ns *ns);
int procpool_for_each_process_local_nolock(int (*fn)(struct process_s *, void *),
					   void *data, struct pid_ns *ns);
#endif
