/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Basic definitions of resmgr used by all managers
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 01:59:54 2021
 */
#ifndef SYSMGR_INTAPI_RES_RES_DEF_H
#define SYSMGR_INTAPI_RES_RES_DEF_H

#include <lib/dlist.h>
#include <intapi/res/res_info.h>
#include <intapi/thread/thread.h>
#include <libalgo/rb_htable.h>
#include <hongmeng/compiler.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmebr/ebr_dlist.h>
#include <libhmebr/ebr_rb_htable.h>
#include <intapi/mem/page.h>

#define MAX_CFILE_NAME		64

raw_static_assert(MAX_RES_COUNT <= sizeof(unsigned int) * 8UL,
		resource_types_must_beq_little_than_bits_of_uint);

/*
 * A controller_file represents a file in a controller.
 * Files in a controller can be used to set or get attributes
 * (limit, weight, share, etc.) of the controlled resources.
 */
struct controller_file {
	char name[MAX_CFILE_NAME];
	unsigned int flags;

	struct res_controller *rctr;   /* point back to its controller */

	char *hash_key;                /* controller file name. Used for hash key */
	ebr_rbt_hnode hash_node;    /* node maintained on hashtable */

	/* read values from this file */
	int (*read)(struct res_group *rgrp, char *buf, size_t len, size_t *size);
	/* write values into this file */
	int (*write)(struct res_group *rgrp, char *buf, size_t len, size_t *size);
};

/*
 * Operations for res_controller
 */
struct controller_operations {
	/* alloc a resource */
	struct res_obj *(*alloc)(void);
	/* free a resource */
	void (*free)(struct res_obj *robj);
	/* enable resource */
	void (*enable)(struct res_obj *robj);
	/* disable/cleanup resource */
	void (*disable)(struct res_obj *robj);
	/* apply group attributes to migrated process */
	int (*migrate)(struct process_s *process);
	/* check/precharge before threads migrated to new group */
	int (*pre_migrate)(struct res_group *rgrp, struct process_s *process, bool is_thread_migrate);
	/* revert what we have done in pre_migrate in error cases */
	void (*post_migrate)(struct res_group *rgrp, struct process_s *process, bool is_thread_migrate);
	/* apply group attributes to new thread */
	void (*attach)(struct thread_s *thread, bool is_thread_migrate);
	/* check/precharge for new thread */
	int (*pre_attach)(struct process_s *process, struct thread_s *thread);
	/* revert what we have done in pre_attach in error cases */
	void (*post_attach)(struct process_s *process, struct thread_s *thread);
	/* release resources for dead thread */
	void (*release)(struct thread_s *thread);
};

/*
 * A resource controller is usually responsible for distributing a
 * specific type of system resource along the hierarchy although
 * there are utility controllers which serve purposes other than
 * resource distribution.
 */
struct res_controller {
	unsigned int id;
	const char *name;

	struct controller_operations *cops;
	struct controller_file *cfiles;
	/* If %true, this controller supports thread mode,
	 * and can control resource at thread granularity.
	 * Full function of threaded model will be supported
	 * in the future.
	 */
	bool threaded;
};

struct res_group_node {
	struct raw_mutex node_mutex;   /* protect list struct dlist_node children */
	struct dlist_node sibling;   /* linked into children list of its parent's group */
	struct dlist_node children;  /* all children of this group are linked into this list */
	struct res_group_node *parent;  /* parent node */
};

#ifdef CONFIG_RESMGR_COMPAT_V1
enum res_group_root_type {
	RES_GROUP_NOT_ROOT,
	RES_GROUP_LOGIC_ROOT,
	RES_GROUP_REAL_ROOT,
	RES_GROUP_ROOT_TYPE_MAX,
};
#endif

/*
 * A resource group contains a bunch of processes and a set of
 * resources. We can get/set each resource's limit/share for
 * all processes in this group. That means, resources are
 * limited or shared among processes in the same group.
 *
 * All resource groups are organized into resmgr.
 */
struct res_group {
	struct raw_mutex rg_mutex;   /* protect tasks list */
	struct raw_refcnt refcnt;    /* refcnt held by threads and robjs */
#ifdef CONFIG_RESMGR_COMPAT_V1
	/* Tasks list linked in this group. Every element in the task_list
	 * array cooresponds to tasks in one kind of controllers of this group.
	 */
	struct dlist_node tasks_list[MAX_RES_COUNT];
	enum res_group_root_type logic_root;
#else
	struct dlist_node tasks;     /* tasks linked in this group */
#endif

	struct res_group_node rg_node; /* node in resmgr tree */

	char *hash_key;       	     /* resource group name. Used for hash key */
	ebr_rbt_hnode hash_node;  /* node maintained on hashtable */

	struct res_obj *robjs[MAX_RES_COUNT]; /* resources of this group */
	unsigned long long subtree_control; /* bitmasks of subsystems enabled on the child cgroup */
	struct ebr_entry entry;
};

static inline int res_obj_get(struct res_obj *robj)
{
	BUG_ON(robj == NULL);
	return raw_refcnt_get(&robj->refcnt);
}

static inline void res_obj_put(struct res_obj *robj)
{
	int cnt = raw_refcnt_put(&robj->refcnt);
	BUG_ON(cnt < 0, "put robj failed, cnt=%d, id=%u, name=%s\n", cnt, robj->rctr->id, robj->rctr->name);
}

static inline struct res_obj *group_robj(const struct res_group *rgrp, unsigned int cid)
{
	return (rgrp == NULL) ? NULL : ebr_dereference(rgrp->robjs[cid]);
}

#endif
