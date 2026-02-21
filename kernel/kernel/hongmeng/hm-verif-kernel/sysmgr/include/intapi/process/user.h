/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define interface of user
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 17 16:36:24 2019
 */

#ifndef SYSMGR_INTAPI_PROCESS_USER_H
#define SYSMGR_INTAPI_PROCESS_USER_H

#include <stdint.h>

#include <sys/types.h>
#include <libalgo/rbtree.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libstrict/strict.h>

#define UID_STATE_SIZE		5
#define IO_STATS_READ_BYTES	0U
#define IO_STATS_WRITE_BYTES	1U
#define IO_STATS_RCHAR		2U
#define IO_STATS_WCHAR		3U
#define IO_STATS_FSYNC		4U

struct process_s;

struct io_stats {
	vatomic64_t read_bytes;
	vatomic64_t write_bytes;
	vatomic64_t rchar;
	vatomic64_t wchar;
	vatomic64_t fsync;
};

struct user_res {
	struct raw_refcnt_nr refcnt;
	uid_t uid;
	struct rbt_node uid_node;
	raw_atomic_ullong_t nproc;
	raw_atomic_int_t sigpending;
	uint64_t mq_bytes;
#ifdef CONFIG_HMPERF_NG
	vatomic64_t perf_locked_vm;
#endif
	struct io_stats io;
};

#ifdef CONFIG_VFS_FUSION
void process_update_user_io_stats(unsigned int bytes, int type, bool is_fsync);
#else
static inline void process_update_user_io_stats(unsigned int bytes, int type, bool is_fsync)
{
	UNUSED(bytes, type, is_fsync);
}
#endif

#ifdef CONFIG_DAC
void user_init(void);
int process_user_init(struct process_s *process);
void process_user_put(struct process_s *process);
int process_user_copy(struct process_s *old, struct process_s *new);
int process_setuid(struct process_s *process, uid_t ruid);
struct user_res *user_get(struct process_s *process);
void user_put(struct user_res *user);
struct user_res *user_get_ebr(struct process_s *process);
void user_put_ebr(struct user_res *user);
#else
static inline void user_init(void) {}
static inline int process_user_init(struct process_s *process)
{
	UNUSED(process);
	return E_HM_OK;
}
static inline void process_user_put(struct process_s *process)
{
	UNUSED(process);
}
static inline int process_setuid(struct process_s *process, uid_t ruid)
{
	UNUSED(process, ruid);
	return E_HM_OK;
}
static inline int process_user_copy(struct process_s *old, struct process_s *new)
{
	UNUSED(old, new);
	return E_HM_OK;
}
static inline struct user_res *user_get(struct process_s *process)
{
	UNUSED(process);
	return NULL;
}
static inline void user_put(struct user_res *user)
{
	UNUSED(user);
}
static inline struct user_res *user_get_ebr(struct process_s *process)
{
	UNUSED(process);
	return NULL;
}
static inline void user_put_ebr(struct user_res *user)
{
	UNUSED(user);
}

#endif

#endif
