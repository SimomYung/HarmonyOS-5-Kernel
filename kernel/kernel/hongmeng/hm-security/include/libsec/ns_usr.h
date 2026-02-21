/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: user namespace header file
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 25 15:55:04 CST 2023
 */

#ifndef ULIBS_LIBSEC_NS_USR_H
#define ULIBS_LIBSEC_NS_USR_H
// sys
#include <sys/types.h>
// hongmeng
#include <libsysif/utils.h>
#include <libalgo/rbtree.h>
#include <libhmsync/raw_scopedptr.h>
// security
#include <libhmsrv_sys/hm_coobj.h>

typedef size_t cred_idx_t;
#define UGID_MAP_MAX_NR 5

struct ugid_item {
	uint32_t inner_first;
	uint32_t global_first;
	uint32_t count;
};

struct ugid_map {
	unsigned int nr;
	struct ugid_item items[UGID_MAP_MAX_NR];
};

struct usr_ns_cred {
	struct raw_refcnt refcnt;
	cred_idx_t idx;
	struct ugid_map uid_map;
	struct ugid_map gid_map;
	struct ugid_map projid_map;
	int level;
	uid_t owner;
	gid_t group;
	unsigned long flags;
	void *base;
	cred_idx_t parent_idx;
	struct usr_ns_cred *parent;
};

#endif
