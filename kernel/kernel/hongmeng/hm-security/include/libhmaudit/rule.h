/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Interfaces of shared rules of audit
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 1 11:46:29 2021
 */
#ifndef ULIBS_LIBHMAUDIT_RULE_H
#define ULIBS_LIBHMAUDIT_RULE_H
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <lib/dlist.h>
#include <hmkernel/capability.h>
#include <hmkernel/const.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_audit/audit.h>

#define FIELD_NOT_MATCH 0
#define WATCH_IS_MATCH 1
#define WATCH_NOT_MATCH 0
#define RULE_LIST_COUNT HM_AUDIT_NR_FILTERS

struct audit_shared_rule_addr {
	uint16_t shm_id;
	uint16_t shm_off;
};

struct audit_shared_rule_list_node {
	struct audit_shared_rule_addr next;
	struct audit_shared_rule_addr prev;
};

struct audit_shared_cfg_stat {
	bool rule_empty;
};

struct audit_shared_cfg {
	uint16_t rule_count;
	struct audit_shared_rule_list_node list;
};

struct audit_shared_cfg_page {
	struct audit_shared_cfg_stat stat;
	struct audit_shared_cfg cfg[RULE_LIST_COUNT];
};

struct audit_shared_ctrl {
	struct raw_rwlock lock;
};

struct audit_shared_field {
	uint32_t type;
	uint32_t value;
	uint32_t op;
};

struct audit_shared_rule {
	uint64_t rule_id;
	uint32_t action;
	uint32_t mask[HM_AUDIT_BITMASK_SIZE]; /* syscall num */
	uint16_t field_count;
	uint32_t rule_type;
	struct audit_shared_rule_addr watch_addr;
	struct audit_shared_rule_list_node node;
	struct audit_shared_field fields[0];
};

#define AUDIT_SHM_NAME_LEN 64
struct audit_shm {
	uintptr_t base;
	size_t size;
	char name[AUDIT_SHM_NAME_LEN];
};

#define AUDIT_SHM_WATCH_FIELD_MAX_NUM 64u
struct audit_shared_watch_field {
	cref_t caller_cref;
	bool is_valid;
	int me_idx;
	dev_t dev_id;
	dev_t devno;
	uint32_t index;
	int type;
	int perm;
};

struct audit_shared_watch_field_page {
	uint32_t free_count;
	struct raw_rwlock lock;
	struct audit_shared_watch_field field[AUDIT_SHM_WATCH_FIELD_MAX_NUM];
};

struct audit_filter_rule;
struct audit_rule;
struct audit_match;
struct audit_watch;

void audit_shared_rule_fini(void);
int audit_shared_rule_init(bool create);
int audit_filter_rule_fill(struct audit_filter_rule *f_rule, unsigned list, bool append);
struct raw_rwlock *audit_shared_rule_list_lock(unsigned list);
void *audit_shared_rule_comm_nolock(const struct audit_shared_rule_addr *addr, uint32_t rule_size);
int audit_shared_rule_iterate(unsigned list, struct audit_match *match);
int audit_shared_rule_add(struct audit_rule *rule, bool append);
void audit_shared_rule_del(struct audit_rule *rule);
int audit_shared_watch_rule_add(struct audit_watch *watch);
void audit_shared_watch_rule_del(struct audit_watch *watch);

static inline struct audit_shared_rule *audit_shared_rule_nolock(const struct audit_shared_rule_addr *addr)
{
	return (struct audit_shared_rule *)audit_shared_rule_comm_nolock(addr,
						 (uint32_t)sizeof(struct audit_shared_rule));
}

int audit_rule_watch_path_len(const char *path,
			      const struct audit_shared_rule_addr *addr,
			      size_t *path_len);

#endif
