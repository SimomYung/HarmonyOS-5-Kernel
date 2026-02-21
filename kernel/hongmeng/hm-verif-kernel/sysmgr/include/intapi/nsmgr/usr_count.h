/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Structures and interfaces for user count
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 12 14:35:21 2023
 */

#ifndef SYSMGR_INCLUDE_NSMGR_USER_COUNT_H
#define SYSMGR_INCLUDE_NSMGR_USER_COUNT_H

#include <lib/utils.h>
#include <lib/dlist.h>
#include <hongmeng/types.h>
#include <libhmsync/atomic.h>

struct usr_ns;

enum usr_count_type {
	USR_COUNT_USER_NAMESPACES,
	USR_COUNT_INOTIFY_INSTANCES,
	USR_COUNT_INOTIFY_WATCHES,
	USR_COUNT_FANOTIFY_GROUPS,
	USR_COUNT_FANOTIFY_MARKS,
	USR_COUNT_MAX,
};

struct usr_count {
	struct dlist_node list;
	struct usr_ns *ns;
	uid_t uid;
	int count;
	raw_atomic_int_t usr_counts[USR_COUNT_MAX];
};

struct usr_count *usr_count_inc(struct usr_ns *ns, uid_t uid, enum usr_count_type type);
void usr_count_dec(struct usr_count *usr_count, enum usr_count_type type);

struct usr_count *usr_count_inc_current(enum usr_count_type type);

int ns_root_usr_get_count_max(enum usr_count_type type);
void ns_root_usr_set_count_max(enum usr_count_type type, int max_count);

int procfs_max_user_namespaces_read(char *data, size_t data_len);
int procfs_max_user_namespaces_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
int procfs_max_inotify_instances_read(char *data, size_t data_len);
int procfs_max_inotify_instances_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
int procfs_max_inotify_watches_read(char *data, size_t data_len);
int procfs_max_inotify_watches_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
int procfs_max_fanotify_groups_read(char *data, size_t data_len);
int procfs_max_fanotify_groups_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
int procfs_max_fanotify_marks_read(char *data, size_t data_len);
int procfs_max_fanotify_marks_write(uint64_t pos, const char *src, size_t size, size_t *wsize);

#endif
