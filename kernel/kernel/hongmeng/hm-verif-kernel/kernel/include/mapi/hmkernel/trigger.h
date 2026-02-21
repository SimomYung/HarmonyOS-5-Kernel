/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions of trigger exported for module.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 16 16:10:54 2019
 */

#ifndef MAPI_HMKERNEL_TRIGGER_H
#define MAPI_HMKERNEL_TRIGGER_H

#include <lib/dlist.h>
#include <uapi/hmkernel/trigger.h>
#include <hmkernel/kernel.h>
#include <hmkernel/reclock.h>

/*
 * The state transition for arm, fire, receive and done:
 *
 *                          arm            +---------+          fire
 *               +------------------------>|  ARMED  +------------------------+
 *               |  event is not arrived   +--+------+    listener is busy    |
 *               |                            |   ^                           |
 *               |                    receive |   | done                      |
 *               |                            v   |                           v
 *    init   +---+---+                     +------+--+       receive      +-------+
 * --------->| FREE  |                     |RECEIVED |<-------------------+ FIRED |
 *           +---+---+                     +---------+                    +-------+
 *               |                              ^                             ^
 *               |                              | receive                     |
 *               |                              |                             |
 *               |          fire           +----+----+          fire          |
 *               +------------------------>| LOADING +------------------------+
 *                  listener is not armed  +---------+    listener is busy
 *
 *
 * The state transition for break:
 *                                                                     +----------+
 *                                +-------+                        +---+  FIRED   |
 *                            +---+ ARMED |                        |   +----------+
 *                            |   +-------+                        |
 *           +------+  break  |                +---------+  break  |   +----------+
 *           | FREE |<--------+                | LOADING |<--------+---+ LOADING  |
 *           +------+         |                +---------+         |   +----------+
 *                            |   +-------+                        |
 *                            +---+ FREE  |                        |   +----------+
 *                                +-------+                        +---+ RECEIVED |
 *                                                                     +----------+
 */
ENUM(5, trigger_source_state_e, TRIGGER_SOURCE_STATE_,
	FREE,		/* Trigger is not install */
	LOADING,	/* Trigger is not install but event has arrived */
	ARMED,		/* Trigger is started and ready to fire */
	FIRED,		/* Trigger is fired, wait to be seen */
	RECEIVED	/* Receiver has known the fired event */
)

struct trigger_listener_s;
struct trigger_source_s;

struct trigger_source_ops {
	void (*done)(struct trigger_source_s *s,
		     struct trigger_listener_s *l);
	int (*read)(struct trigger_source_s *s,
		    void __user *buf, unsigned long buf_sz);
	void (*drop)(struct trigger_source_s *s);
	bool (*affi_check)(struct trigger_source_s *s,
			   struct trigger_listener_s *l,
			   unsigned int *val);
};

struct trigger_source_s {
	enum trigger_source_state_e state;
	int retval; /* retval reported to listener */
	struct trigger_source_ops *ops;

	struct dlist_node fired_link; /* linked in listener's pending list */
	struct dlist_node listener_link; /* linked in listener's watching list */
	struct trigger_listener_s *listener; /* linker pointer */
	bool droppable;
	bool pending_fire; /* pending a received source which has be fired */
	void *opaque; /* auxiliary data for each source */
#ifdef CONFIG_SMP
	struct rec_spinlock_s *lock;
#endif
};

void __trigger_source_init(struct trigger_source_s *trigger_source,
			   struct trigger_source_ops *ops,
			   bool droppable, int retval);

static inline void
trigger_source_init(struct trigger_source_s *trigger_source,
		    struct trigger_source_ops *ops,
		    bool droppable)
{
	__trigger_source_init(trigger_source, ops, droppable, 0);
}

static inline void
trigger_source_flush_retval(struct trigger_source_s *trigger_source, int retval)
{
	trigger_source->retval = retval;
}

/* get trigger listener from trigger source */
struct trigger_listener_s *
trigger_source_to_listener(struct trigger_source_s *s);

/* called by real event source */
void trigger_source_fire(struct trigger_source_s *s);

void trigger_source_break(struct trigger_source_s *s);

#ifdef CONFIG_SMP
/*
 * we need old lock to do unlock when change the lock, e.g., when
 * trigger_source_break, source's lock change from lsn's lock to global
 * lock again, and we should unlock the old one.
 */
struct rec_spinlock_s *trigger_source_lock(struct trigger_source_s *s);

static inline void trigger_source_unlock(struct rec_spinlock_s *s_lock)
{
	rec_spinlock_unlock(s_lock);
}
#else
static inline struct rec_spinlock_s *
trigger_source_lock(struct trigger_source_s *s)
{
	UNUSED(s);
	return NULL;
}

static inline void trigger_source_unlock(struct rec_spinlock_s *s_lock)
{
	UNUSED(s_lock);
}
#endif

/* Install trigger source to listener */
int __trigger_source_arm(struct trigger_source_s *s,
			 struct trigger_listener_s *l,
			 int retval, bool drop);

static inline int trigger_source_arm(struct trigger_source_s *s,
				     struct trigger_listener_s *l,
				     int retval)
{
	return __trigger_source_arm(s, l, retval, false);
}

struct actv_pool_s;
/*
 * trigger_source_check_lsn are used to check
 * whether the listener tcb is blocked at futex.
 */
void
trigger_source_check_lsn(struct trigger_source_s *src,
			 struct actv_pool_s *pool);

#endif
