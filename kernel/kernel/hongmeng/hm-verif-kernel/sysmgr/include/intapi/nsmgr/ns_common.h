/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ns common header file, each namespace must contain a ns_common structure.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 30 14:11:05 2020
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_COMMON_H
#define SYSMGR_INCLUDE_NSMGR_NS_COMMON_H

#include <libalgo/rbtree.h>
#include <hongmeng/panic.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_scopedptr.h>
#include <libsec/cred.h>

#define NS_COMMON_IS_VALID 0x1U

struct thread_s;
struct ns_group_s;
struct ns_operation;
struct ns_common {
	unsigned int id;
	vatomic32_t flags;
	struct raw_refcnt_nr refcnt;
	/*
	 * attach_cnt is used to save the count of attaching this ns by sysif
	 * interface hmhandler_ns_get. When profs try to hmhandler_ns_put the
	 * ns, nsmgr will use attach_cnt to check whether the put request is
	 * valid. In order to achieve the purpose of avoiding redundant release
	 * for ns.
	 */
	struct raw_refcnt_nr attach_cnt;
	struct rbt_node node;
	struct ns_operation *op;
};

struct ns_operation {
	int (*install)(const struct thread_s *target,
		       struct ns_group_s *ns_group,
		       struct base_cred *cred,
		       struct ns_common *ns);
	int (*destroy)(struct ns_common *ns);
};

static inline unsigned int
ns_common_id_of(struct ns_common *ns)
{
	return ns->id;
}

static inline int
ns_common_get(struct ns_common *ns)
{
	return raw_refcnt_nr_get(&ns->refcnt);
}

static inline void
ns_common_put(struct ns_common *ns)
{
	int cnt = raw_refcnt_nr_put(&ns->refcnt);
	if (cnt == 0) {
		int err = ns->op->destroy(ns);
		if (err != E_HM_OK) {
			hm_error("Destroy ns failed, err=%s\n", hmstrerror(err));
		}
	}
}

static inline int
ns_common_init(struct ns_common *ns,
	       struct ns_operation *op,
	       unsigned int id)
{
	mem_zero_b(ns, sizeof(struct ns_common));

	ns->id = id;
	ns->op = op;
	vatomic32_init(&ns->flags, 0);
	raw_refcnt_nr_init(&ns->refcnt, 0);
	raw_refcnt_nr_init(&ns->attach_cnt, 1);
	return E_HM_OK;
}

static inline void
ns_common_set_valid(struct ns_common *ns)
{
	vatomic32_or(&ns->flags, NS_COMMON_IS_VALID);
}

static inline bool
ns_common_is_valid(struct ns_common *ns)
{
	unsigned int flags = vatomic32_read(&ns->flags);
	return (flags & NS_COMMON_IS_VALID) == NS_COMMON_IS_VALID;
}

int nsid_init(void);
int nsid_alloc(unsigned int *nsid);
void nsid_free(unsigned int nsid);
int ns_pool_insert(struct ns_common *ns);
void ns_pool_remove(struct ns_common *ns);
struct ns_common *ns_pool_get_ns(unsigned int nsid);
int ns_pool_attach_ns(unsigned int nsid);
int ns_pool_detach_ns(unsigned int nsid);

#endif
