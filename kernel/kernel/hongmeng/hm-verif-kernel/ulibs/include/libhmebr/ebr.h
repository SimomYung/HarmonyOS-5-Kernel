/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declarations for ebr api
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 26 14:05:30 2020
 */
#ifndef ULIBS_LIBHMEBR_EBR_H
#define ULIBS_LIBHMEBR_EBR_H

#include <limits.h>
#include <lib/dlist.h>
#include <vsync/atomic.h>
#include <hmkernel/barrier.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_timer.h>

/*
 * Define before including gdump.h.
 * V_SMR_NODE_BUFF_SIZE is not used in the HM context,
 * and setting it to 1 will reduce the thread object size.
 *
 * VGDUMP_MAX_SYNCERS should be set to the maximum number of
 * threads that reclaim memory. In the case of HM it is always 1.
 */
#define V_SMR_NODE_BUFF_SIZE 1U
#define VGDUMP_MAX_SYNCERS 1U
#include <vsync/smr/gdump.h>
#ifdef CONFIG_CACHELINE_SIZE_L1
#define EBR_ALIGNED_SIZE CONFIG_CACHELINE_SIZE_L1
#else
#define EBR_ALIGNED_SIZE 64
#endif

#define ebr_assign_pointer(ptr, val)		\
do {						\
	if (val) {				\
		smp_write_rel(ptr, val);	\
	} else {				\
		__write_once(ptr, val);		\
	}					\
} while (__false);

/* only suits for scalar type thing */
#define ebr_dereference(ptr) (typeof(ptr))__read_once(ptr)

 /* MPSC stack for callbacks in EBR */
struct cb_stack_entry {
	struct cb_stack_entry *next;
};

struct cb_stack {
	struct cb_stack_entry *head;
};

struct call_ebr_obj;

enum ebr_mcache_type {
	EBR_MCACHE_DEV_BLOCK = 0,
	EBR_MCACHE_FSCACHE,
	EBR_MCACHE_HMFS_NTT,
	EBR_MCACHE_HMFS_MGR,
	EBR_MCACHE_HMFS_GC,
	EBR_MCACHE_DEFAULT,
	EBR_MCACHE_INVALID,
};

struct epoch_set {
	gdump_t gdump;
	struct raw_rwlock rwlock;

	/* lock for initialization */
	struct raw_mutex init_lock;
	raw_thread_key_t key;
	char* name;

	/* pointer to instance of handling ebr callbacks */
	struct call_ebr_obj *call_obj;
	int reclaim_watermark;
	enum ebr_mcache_type mcache_type;
	int init_loc;
	uint32_t refcnt;
} __aligned(EBR_ALIGNED_SIZE);

void smr_rwlock_rdlock(void *arg);
void smr_rwlock_wrlock(void *arg);
void smr_rwlock_unlock(void *arg);

#define EBR_INIT_WITH_PARAM(_name, _watermark, _mcache_type) {	\
	.name = #_name,				\
	.gdump.lock = {				\
		.read_acq	= smr_rwlock_rdlock,	\
		.read_rel	= smr_rwlock_unlock,	\
		.write_acq	= smr_rwlock_wrlock,	\
		.write_rel	= smr_rwlock_unlock,	\
		.arg		= &((_name).rwlock),	\
	},					\
	.rwlock = RAW_RDLOCK_INITIALIZER,	\
	.init_lock = RAW_MUTEX_INITIALIZER,	\
	.key = UINT_MAX,			\
	.refcnt = 0U,				\
	.call_obj = NULL,			\
	.reclaim_watermark = _watermark,	\
	.mcache_type = _mcache_type,		\
	.init_loc = __LINE__,			\
}

#define EBR_RECLAIM_WATERMARK 1000
#define EBR_INIT_WITH_WATERMARK(_name, _watermark) EBR_INIT_WITH_PARAM(_name, _watermark, EBR_MCACHE_DEFAULT)
#define EBR_INIT_WITH_MCACHE(_name, _mcache_type) EBR_INIT_WITH_PARAM(_name, EBR_RECLAIM_WATERMARK, _mcache_type)
#define EBR_INIT(_name) EBR_INIT_WITH_WATERMARK(_name, EBR_RECLAIM_WATERMARK)

struct ebr_entry {
	struct cb_stack_entry stack_entry;
	void (*cb_func)(struct ebr_entry *entry);
};

int ebr_init_with_watermark(struct epoch_set *es, int watermark);
static inline int ebr_init(struct epoch_set *es)
{
	return ebr_init_with_watermark(es, EBR_RECLAIM_WATERMARK);
}

int ebr_register_unit(struct epoch_set *es);
int ebr_unregister_unit(struct epoch_set *es);
int ebr_read_lock(struct epoch_set *es);
int ebr_read_unlock(struct epoch_set *es);
int call_ebr(struct epoch_set *es, struct ebr_entry *entry,
	     void (*cb_func)(struct ebr_entry *));
int call_ebr_obj_create(struct epoch_set *es);

int ebr_thread_create(struct epoch_set *es);

static inline int synchronize_ebr(struct epoch_set *es)
{
	int ret = E_HM_OK;

	if (es == NULL) {
		ret = E_HM_INVAL;
	} else {
		gdump_sync(&es->gdump, hm_thread_yield, hm_usleep);
	}
	return ret;
}

#endif
