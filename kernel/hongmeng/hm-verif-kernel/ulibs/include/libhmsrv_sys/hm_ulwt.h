/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header of hm ulwt feature
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 12 18:21:32 2023
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SILK_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SILK_H

#include <sched.h>
#include <stdint.h>
#include <sys/types.h>

#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/ulwt/monitor.h>
#include <hmkernel/ulwt/kinfo.h>

#define ULWTFD_MAX_EVENT_FIELDS		(9)
#define ULWT_CGROUP_NAME ((ULWTFD_MAX_EVENT_FIELDS) * 4)
#define ULWT_THREAD_NAME (16)

#define ULWT_EVT_READ_ONCE_MAX	(10)

/* This define the layout of ulwt infopage. */

enum ulwt_sched_state_type {
	ULWT_STATE_READY,
	ULWT_STATE_RUNING,
	ULWT_STATE_BLOCKED,
	ULWT_STATE_ACTIVATED, // ULWT not use
	ULWT_STATE_DEAD,
	ULWT_STATE_UNKNOWN,
};

enum ulwt_block_reason_type {
	ULWT_NO_BLOCK,
	ULWT_BLOCK_FUTEX,
};

typedef struct ulwtmgr_cgroup {
	char name[ULWT_CGROUP_NAME];
} shared_cgroup_t;

typedef struct shared_state_s {
	int ulwt_state;
	int ulwt_block;
	uint64_t ulwt_sched_cnt;
} shared_state_t;

typedef struct shared_attr_s {
	/* which bound/occupied with the wu */
	int xstream_tid;
	unsigned int bind_reason;
} shared_attr_t;

typedef struct shared_zone_s {
	shared_state_t state;
	shared_attr_t attr;
	shared_cgroup_t cgroup;
} shared_zone_t;

static inline int hm_ulwt_zone_state_of(const shared_zone_t *zone)
{
	return zone->state.ulwt_state;
}

static inline void hm_ulwt_zone_set_state(shared_zone_t *zone, int state)
{
	zone->state.ulwt_state = state;
}

static inline int hm_ulwt_zone_block_of(const shared_zone_t *zone)
{
	return zone->state.ulwt_block;
}

static inline void hm_ulwt_zone_set_block(shared_zone_t *zone, int block)
{
	zone->state.ulwt_block = block;
}

static inline uint64_t hm_ulwt_zone_sched_cnt_of(const shared_zone_t *zone)
{
	return zone->state.ulwt_sched_cnt;
}

static inline void hm_ulwt_zone_inc_sched_cnt(shared_zone_t *zone)
{
	zone->state.ulwt_sched_cnt++;
}

static inline int hm_ulwt_zone_xstream_tid_of(shared_zone_t *zone)
{
	return zone->attr.xstream_tid;
}

static inline void hm_ulwt_zone_set_xstream_tid(shared_zone_t *zone, int tid)
{
	zone->attr.xstream_tid = tid;
}

static inline int hm_ulwt_zone_bind_reason_of(const shared_zone_t *zone)
{
	return zone->attr.bind_reason;
}

static inline void hm_ulwt_zone_set_bind_reason(shared_zone_t *zone, int reason)
{
	zone->attr.bind_reason = reason;
}

static inline const char *hm_ulwt_zone_cgroup_of(const shared_zone_t *zone)
{
	return zone->cgroup.name;
}

static inline int hm_ulwt_zone_set_cgroup(shared_zone_t *zone, const char *cgroup, uint32_t size)
{
	return (strncpy_s(zone->cgroup.name, ULWT_CGROUP_NAME, cgroup, size) == 0 ? E_HM_OK : E_HM_INVAL);
}

/* ulwt domain event types */
enum ulwt_event_type {
	ULWT_EVT_WAKEUP,
	ULWT_EVT_SIGNAL_WAKEUP,
	ULWT_EVT_UPDATE_CGROUP,
	ULWT_EVT_UPDATE_THREAD_NAME,
	ULWT_EVT_SIGNAL_BIND,
	ULWT_EVT_SIGNAL_UNBIND,
	ULWT_EVT_POST_XSTREAM_EXIT,
	ULWT_EVT_BIND_WU_XS,  // bind a wu to a xs exclusively
	ULWT_EVT_UPDATE_SCHED,
	ULWT_EVT_SCHED_UNBIND,
	ULWT_EVT_MAX,
};

#define ULWT_EVT_BIND_SECURITY_SHIFT	0U
#define ULWT_EVT_BIND_SCHED_SHIFT	1U
#define ULWT_EVT_BIND_REASON_SECURITY	(1 << ULWT_EVT_BIND_SECURITY_SHIFT)
#define ULWT_EVT_BIND_REASON_SCHED	(1 << ULWT_EVT_BIND_SCHED_SHIFT)
#define ULWT_INHERIT_BIND_REASONS	ULWT_EVT_BIND_REASON_SECURITY

typedef struct ulwt_sched_attr_s {
#ifdef CONFIG_THREAD_FINE_LOAD
	int qos;
#endif
	int policy;
	int priority;
} ulwt_sched_attr_t;

struct ulwt_event {
	enum ulwt_event_type type;
	int tid;
	unsigned long long wu_desc_addr;
	union {
		unsigned int fields[ULWTFD_MAX_EVENT_FIELDS];
		struct {
			int timer_tid;
			unsigned long long timer_cref;
		} signal_wakeup;
		struct {
			char cgroup[ULWT_CGROUP_NAME];
		} update_cgroup;
		struct {
			char name[ULWT_THREAD_NAME];
		} update_thread_name;
		struct {
			unsigned int reason;
		} update_bind_reason;
		struct {
			int domain_id;
		} update_sched;
	};
};

struct sigtimedwait_end_info {
	int futex_ret;
	int timer_tid;
	uint64_t timer_cref;
};

struct sigprocmask_info {
	int how;
	uint64_t nset;
	uint64_t oset;
	uint64_t size;
};

#define NORMAL_SCHED_DOMAIN_NR		3

#ifdef CONFIG_THREAD_FINE_LOAD
#define QOS_SCHED_DOMAIN_NR		10
#define SCHED_DOMAIN_NR			(NORMAL_SCHED_DOMAIN_NR + QOS_SCHED_DOMAIN_NR)
#else
#define SCHED_DOMAIN_NR			NORMAL_SCHED_DOMAIN_NR
#endif

#define SCHED_DOMAIN_ID_INVAILD		(-1)

/* normal domain config */
#define HIGH_SCHED_DOMAIN_PRIO_MAX	(-12)
#define MID_SCHED_DOMAIN_PRIO_MAX	2
#define LOW_SCHED_DOMAIN_PRIO_MAX	HM_PRIORITY_NICE_MAX
#define HIGH_SCHED_DOMAIN_ID		0
#define MID_SCHED_DOMAIN_ID		1
#define LOW_SCHED_DOMAIN_ID		2

static inline bool ulwt_occupy_conditions(int policy)
{
	int real_policy = (unsigned int)policy & (~__U(SCHED_RESET_ON_FORK));
	return real_policy != SCHED_OTHER;
}

static inline int hm_ulwt_scheduler_to_normal_domain_id(int policy, int priority)
{
	if (!ulwt_occupy_conditions(policy)) {
		if (priority < HIGH_SCHED_DOMAIN_PRIO_MAX) {
			return HIGH_SCHED_DOMAIN_ID;
		} else if(priority < MID_SCHED_DOMAIN_PRIO_MAX) {
			return MID_SCHED_DOMAIN_ID;
		} else {
			return LOW_SCHED_DOMAIN_ID;
		}
	}
	return SCHED_DOMAIN_ID_INVAILD;
}

static inline bool hm_ulwt_check_domain_id_vaild(int domain_id)
{
	return domain_id >=0 && domain_id < SCHED_DOMAIN_NR;
}

#ifdef CONFIG_THREAD_FINE_LOAD
static inline int hm_ulwt_qos_to_qos_domain_id(int qos)
{
	int domain_id = SCHED_DOMAIN_ID_INVAILD;
	if (qos >= 0 && qos < QOS_SCHED_DOMAIN_NR) {
		domain_id = qos;
	}
	return domain_id;
}
#endif

static inline int hm_ulwt_sched_attr_to_domain_id(ulwt_sched_attr_t *attr)
{
	int domain_id = SCHED_DOMAIN_ID_INVAILD;
#ifdef CONFIG_THREAD_FINE_LOAD
	int qos_domain_id = hm_ulwt_qos_to_qos_domain_id(attr->qos);
	if (qos_domain_id == SCHED_DOMAIN_ID_INVAILD) {
		domain_id = hm_ulwt_scheduler_to_normal_domain_id(attr->policy, attr->priority);
	} else {
		domain_id = qos_domain_id + NORMAL_SCHED_DOMAIN_NR;
	}
#else
	domain_id = hm_ulwt_scheduler_to_normal_domain_id(attr->policy, attr->priority);
#endif
	return domain_id;
}
#endif
