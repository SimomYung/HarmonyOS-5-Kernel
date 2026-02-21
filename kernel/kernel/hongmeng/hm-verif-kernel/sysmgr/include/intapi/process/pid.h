/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Pid function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 7 15:24:35 2018
 */
#ifndef SYSMGR_INTAPI_PROCESS_PID_H
#define SYSMGR_INTAPI_PROCESS_PID_H

#include <stdint.h>
#include <unistd.h> /* for pid_t */
#include <lib/dlist.h>
#include <libalgo/idr.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv_id.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <hongmeng/types.h>
#include <hmkernel/capability.h>
#include <aapi/hmsysmgr/process/pid.h>

#define TID_IS_PID		0x1U
#define TID_IS_VALID		0x2U
#define TID_IS_KILLABLE		0x4U
#define TID_IS_PIDNS_DEAD	0x8U
#define MAX_TID_COUNT		(CONFIG_SYSMGR_MAX_THREADS)
#define PIDNS_ADDING (1U << 31)

struct thread_s;
struct pid_ns;

struct pidpool {
	struct idr idr;

	raw_atomic_int_t tid_max;
	int tid_max_min;
	int tid_max_max;
	unsigned long max_pid_slots;
	unsigned int alloc_flag;
	struct raw_mutex lock;
};

enum pid_type {
	PIDTYPE_SID,
	PIDTYPE_PID,
	PIDTYPE_PGID,
	PIDTYPE_MAX,
};

/*
 * utid represents the id number of tid and the associated pid ns of this number.
 * The tid_s has a utid in every pid ns in which this tid_s is visible.
 */
struct utid {
	pid_t nr;				/* number of id */
	struct pid_ns *ns;			/* associated pid ns of this number */
};

/*
 * tid_s is a common struct of pid and tid, it can represent the number of tid
 * associated with a thread, or the number of pid/pgid/sid. In the tid case,
 * if the thread is main thread of a process, it represents both pid and tid,
 * otherwise, tid_s shall be malloced by thread create, and it is not contained
 * by a pid_s struct. In the pid/pgid/sid case, the pointer *thread points to
 * NULL.
 * The numbers array is a variable-length array of utid. The size of array is
 * determined by the level of pid ns in which the tid_s is alloced. The memory
 * of tid_s is alloced when alloc tid.
 */
struct tid_s {
	struct raw_refcnt_nr refcnt_nr;		/* refcnt of tid_s */
	struct thread_s *thread;			/* pointer to thread related with tid */
	/*
	 * mutex to protect thread ptr in tid_s, and PIDTYPE_PID dlist in pid_s,
	 * will be replaced by ebr later
	 */
	struct raw_mutex mutex;
	unsigned int flags;			/* flags of tid_s */
	unsigned int level;			/* pid ns level of this tid */
	struct utid numbers[1];			/* variable-length array of utid */
};

#ifdef CONFIG_PIDFD_SUPPORT
/* for pidfd notifications */
struct pidfd_notify_s {
	struct raw_mutex pidfd_notify_mutex;
	bool notified;
	struct dlist_node polllist_head;
};
#endif

struct pid_s {
	/*
	 * 1. To reduce space occupation of pid_s, reuse heads[0] as the pointer
	 * pointing to sid if pid_s is a pgid.
	 * 2. The pointer sid is only valid if pid_s is a pgid but not sid.
	 * If sid is not NULL, the pid_s is a valid pgid and the sid points to the
	 * sid that the pgroup belongs to.
	 */
	union {
		struct dlist_node heads[PIDTYPE_MAX];
		struct pid_s *sid;
	};

#ifdef CONFIG_PIDFD_SUPPORT
	struct pidfd_notify_s pidfd_notify;
#endif

	/*
	 * this mutex is only valid if pid_s is a sid, do not operate it if not so.
	 * All group and session operation must do with this mutex locked.
	 */
	struct raw_mutex mutex;

	/*
	 * this mutex is only valid if pid_s is a pgid, to proctect pgid->sid pointer.
	 * do not operate it if not so.
	 */
	struct raw_mutex pgid_mutex;

	/*
	 * tid_s should be the last element of pid_s, as it ends with a variable-length
	 * array
	 */
	struct tid_s tid;
};

void pidpool_init(struct pidpool *pool);

void pid_alloctor_init(void);
void free_tid(struct tid_s *tid);
struct tid_s *malloc_tid(unsigned int level);
void free_pid(struct pid_s *pid);
struct pid_s *malloc_pid(unsigned int level);

struct tid_s *tid_alloc(struct pid_ns *ns);
void tid_put(struct tid_s *tid);
void pid_put(struct pid_s *pid);

struct pid_s *pid_alloc(struct pid_ns *ns);
struct pid_s *pid_alloc_reserved(pid_t pid_nr);
struct tid_s *find_get_tid_s_ex(struct pid_ns *ns, pid_t tid_nr,
				bool ignore_invalid_tid);
struct tid_s *find_get_tid_s(struct pid_ns *ns, pid_t tid_nr);
struct pid_s *find_get_pid_s(struct pid_ns *ns, pid_t pid);

void tid_s_set_ns_ptr(struct tid_s *tid, struct pid_ns *ns);
void pid_s_set_ns_ptr(struct pid_s *pid, struct pid_ns *ns);

int pidpool_get_tid_max(void);
void pidpool_set_tid_max(int tid_max);
int pidpool_get_tid_max_max(void);
int pidpool_get_tid_max_min(void);
int pidpool_set_last_pid(struct pidpool *pool, int last_id);

static inline unsigned int tid_s_level_of(struct tid_s *tid)
{
	return tid->level;
}

static inline unsigned int pid_s_level_of(struct pid_s *pid)
{
	return tid_s_level_of(&pid->tid);
}

static inline struct pid_ns *
tid_ns_of_level(struct tid_s *tid, unsigned int level)
{
	return level <= tid_s_level_of(tid) ? tid->numbers[level].ns : NULL;
}

static inline struct pid_ns *
pid_ns_of_level(struct pid_s *pid, unsigned int level)
{
	return tid_ns_of_level(&pid->tid, level);
}

static inline struct pid_ns *
tid_ns_of(struct tid_s *tid)
{
	return tid_ns_of_level(tid, tid->level);
}

static inline struct pid_ns *pid_ns_of(struct pid_s *pid)
{
	return tid_ns_of(&pid->tid);
}

static inline pid_t tid_s_nr_of_level(struct tid_s *tid, unsigned int level)
{
	return tid->numbers[level].nr;
}

static inline pid_t tid_s_nr_of(struct tid_s *tid)
{
	return tid_s_nr_of_level(tid, tid->level);
}

static inline pid_t pid_s_nr_of_level(struct pid_s *pid, unsigned int level)
{
	return (level <= pid->tid.level) ? pid->tid.numbers[level].nr : PID_NONE;
}

static inline pid_t pid_s_nr_of(struct pid_s *pid)
{
	return tid_s_nr_of(&pid->tid);
}

static inline struct raw_mutex *tid_s_mutex_of(struct tid_s *tid)
{
	return &tid->mutex;
}

static inline void tid_s_set_level(struct tid_s *tid, unsigned int level)
{
	tid->level = level;
}

static inline void pid_s_set_level(struct pid_s *pid, unsigned int level)
{
	tid_s_set_level(&pid->tid, level);
}

static inline void tid_s_set_nr(struct tid_s *tid, unsigned int level, pid_t nr)
{
	tid->numbers[level].nr = nr;
}

static inline void pid_s_set_nr(struct pid_s *pid, unsigned int level, pid_t nr)
{
	tid_s_set_nr(&pid->tid, level, nr);
}

static inline void tid_s_set_thread(struct tid_s *tid, struct thread_s *thread)
{
	tid->thread = thread;
}

static inline int tid_get(struct tid_s *tid)
{
	return raw_refcnt_nr_get(&tid->refcnt_nr);
}

static inline int pid_get(struct pid_s *pid)
{
	return tid_get(&pid->tid);
}

/*
 * As sid must be a pgid too, this function will return true in 2 cases:
 * 1. pid is just a pgid but not a sid.
 * 2. pid is a sid, and a pgid obviously.
 */
static inline bool pid_is_pgid(const struct pid_s *pid)
{
	return (pid->heads[PIDTYPE_SID].prev != NULL) ? true : false;
}

/* init pgid only when create pgroup and session */
static inline bool pgid_is_inited(const struct pid_s *pgid)
{
	return (pgid->heads[PIDTYPE_PGID].prev != NULL) ? true : false;
}

/* return true if pid is a sid */
static inline bool pid_is_sid(const struct pid_s *pid)
{
	return (pid->heads[PIDTYPE_SID].next != NULL) ? true : false;
}

/*
 * clear pgid's heads[PIDTYPE_SID]->prev(i.e. *sid) to make
 * it a pid, not a pgid anymore.
 */
static inline void pid_s_clear_pgid(struct pid_s *pgid)
{
	raw_mutex_lock(&pgid->pgid_mutex);
	pgid->heads[PIDTYPE_SID].prev = NULL;
	raw_mutex_unlock(&pgid->pgid_mutex);
}

/*
 * clear sid's heads[PIDTYPE_SID]->prev and next to make
 * it a pid, not a sid anymore.
 */
static inline void pid_s_clear_sid(struct pid_s *sid)
{
	raw_mutex_lock(&sid->pgid_mutex);
	sid->heads[PIDTYPE_SID].prev = NULL;
	sid->heads[PIDTYPE_SID].next = NULL;
	raw_mutex_unlock(&sid->pgid_mutex);
}

static inline void tid_s_set_valid(struct tid_s *tid)
{
	tid->flags |= TID_IS_VALID;
}

static inline void pid_s_set_valid(struct pid_s *pid)
{
	tid_s_set_valid(&pid->tid);
}

static inline bool tid_s_is_valid(const struct tid_s *tid)
{
	return (tid->flags & TID_IS_VALID) == TID_IS_VALID ? true : false;
}

/*
 * tid_s created by pid_alloc() and pid_alloc_reserved() will set this flag,
 * while created by tid_alloc() will not. Never change after set.
 */
static inline void tid_set_is_pid(struct tid_s *tid)
{
	tid->flags |= TID_IS_PID;
}

static inline bool tid_is_pid(const struct tid_s *tid)
{
	return (tid->flags & TID_IS_PID) == TID_IS_PID ? true : false;
}

static inline void tid_set_pid_ns_dead(struct tid_s *tid)
{
	tid->flags |= TID_IS_PIDNS_DEAD;
}

static inline int tid_set_kill_able(struct tid_s *tid)
{
	int ret = E_HM_INVAL;

	raw_mutex_lock(tid_s_mutex_of(tid));
	if ((tid->flags & TID_IS_PIDNS_DEAD) == 0) {
		tid->flags |= TID_IS_KILLABLE;
		ret = E_HM_OK;
	}
	raw_mutex_unlock(tid_s_mutex_of(tid));

	return ret;
}

static inline bool tid_is_kill_able(const struct tid_s *tid)
{
	return (tid->flags & TID_IS_KILLABLE) == TID_IS_KILLABLE ? true : false;
}

static inline struct pid_s *tid2pid(struct tid_s *tid)
{
	struct pid_s *pid = NULL;

	if (tid_is_pid(tid)) {
		pid = container_of(tid, struct pid_s, tid);
	}
	return pid;
}

unsigned long pid_max_slots_of(void);

int pid_val_get_remote(pid_t pid);
int pid_val_put_remote(pid_t pid);
#endif
