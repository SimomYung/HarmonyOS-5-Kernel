/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: X latency accounting feature
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 14:36:05 2024
 */
#ifndef SYSYMGR_INSPECTOR_XACCT_H
#define SYSYMGR_INSPECTOR_XACCT_H

#include <libhmsync/uspinlock.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmelf/symbol.h>
#include <lib/dlist.h>
#include <lib/utils.h>

#include <intapi/procfs/seq_file.h>
#include <intapi/thread/thread.h>

/* xacct data struct */
struct xacct {
	struct dlist_node node;
	uint64_t begin;
	struct thread_s *t;
	struct xacct *old_xacct;
	uint64_t last;
	struct xacct_data *xd;
	uint64_t *lat_slot;
	uint64_t *cnt_slot;
	uint32_t slot_num;
	uint32_t flag;
};

/* xacct_data data struct */
struct xacct_data {
	raw_atomic_ullong_t *data;
	const char *scene_name;
	const char **slot_names;
	const char **metric_names;
	const char **action_names;
	int *action_to_slot;
	void(*collector_cb)(struct xacct *, uint64_t lat);
	void(*timeout_cb)(struct xacct *);
	raw_atomic_ullong_t max_lat;
	raw_atomic_ullong_t max_lat_time;
	uint32_t data_size; // size of data
	uint32_t slot_num; // size of slot_names
	uint32_t metric_num; // size of metric_names
	uint32_t action_num; // sizeof action_names and action_to_slot
};

struct xacct_data_helper {
	raw_atomic_ullong_t *data;
	const char *scene_name; /* NULL is not allowed */
	int *action_to_slot;
	const char **slot_names;
	int *slot; /* do not need to be persist, NULL is allowed */
	const char **metric_names; /* shoule be persist, NULL is NOT allowed */
	const char **action_names; /* shoule be persist, NULL is allowed */
	void (*collector_cb)(struct xacct *, uint64_t);
	void (*timeout_cb)(struct xacct *);
	uint32_t data_size;
	uint32_t slot_num;
	uint32_t metric_num;
	uint32_t action_num;
};

#define NR_LAT_LV 6

#define XD_HELPER(se, me, ac, co, to) \
	static raw_atomic_ullong_t g_##se##data[(ARRAY_SIZE(se) + 1) * ARRAY_SIZE(me) * NR_LAT_LV]; \
	static int g_##se##action_to_slot[ARRAY_SIZE(ac)]; \
	static const char *g_##se##slot_names[ARRAY_SIZE(se)]; \
	const struct xacct_data_helper se##_helper = { \
		.data		= (raw_atomic_ullong_t *)g_##se##data, \
		.data_size	= (ARRAY_SIZE(se) + 1) * ARRAY_SIZE(me) * NR_LAT_LV, \
		.action_to_slot	= (int *)g_##se##action_to_slot, \
		.slot_names	= (const char **)g_##se##slot_names, \
		.scene_name	= #se, \
		.slot		= (se), \
		.slot_num	= ARRAY_SIZE(se), \
		.metric_names	= (me), \
		.metric_num	= ARRAY_SIZE(me), \
		.action_names	= (ac), \
		.action_num	= ARRAY_SIZE(ac), \
		.collector_cb	= (co), \
		.timeout_cb	= (to), \
	}

#define XD_HELPER_NO_ACTION(se, me, co, to) \
	static uint64_t g_##se##data[ARRAY_SIZE(me) * NR_LAT_LV]; \
	const struct xacct_data_helper se##_helper = { \
		.data		= (raw_atomic_ullong_t *)g_##se##data, \
		.data_size	= ARRAY_SIZE(me) * NR_LAT_LV, \
		.action_to_slot	= NULL, \
		.slot_names	= NULL, \
		.scene_name	= #se, \
		.slot		= NULL, \
		.slot_num	= 0, \
		.metric_names	= (me), \
		.metric_num	= ARRAY_SIZE(me), \
		.action_names	= NULL, \
		.action_num	= 0, \
		.collector_cb	= (co), \
		.timeout_cb	= (to), \
	}

enum xacct_metric {
	XACCT_LAT,
	XACCT_CNT,
	XACCT_METRIC_MAX,
};

#define XACCT_SINVAL (-1)

/* xacct_mgr data struct */
#define XM_NAME_SIZE 16
struct xacct_mgr {
	struct dlist_node node;
	const char name[XM_NAME_SIZE];
	struct xacct_data *xd;
	uint32_t scene_num;
	uint32_t flag;
	uint32_t fault_inject;
	void (*switch_show_cb)(struct seq_file_s *, struct xacct_mgr *);
};

#define DEFINE_XACCT(__xacct_type, __scene_num) \
	static struct xacct_data g_##__xacct_type##_data[(__scene_num)]; \
	static struct xacct_mgr g_##__xacct_type##_mgr = { \
		.node = { \
			.prev = &g_##__xacct_type##_mgr.node, \
			.next = &g_##__xacct_type##_mgr.node, \
		}, \
		.name = #__xacct_type, \
		.xd = g_##__xacct_type##_data, \
		.scene_num = (__scene_num), \
		.flag = XM_FLAG_ENABLE, \
		.fault_inject = false, \
		.switch_show_cb = xm_switch_show_cb_def, \
	}; \
	raw_static_assert(sizeof(#__xacct_type) <= XM_NAME_SIZE, size_of_name_too_large)

#define XM_FLAG_ENABLE	(0x1U << 0U)
#define XM_FLAG_TRACE	(0x1U << 1U)
#define XM_FLAGS	((0x1U << 8U) - 1)
#define XM_FLAG_XACCT_OWNER		(0x1U << 8U)

#define XT_REPORT_MAX_LEN 512
#define XT_REPORT_RATE_CNT 1
#define XT_REPORT_RATE_MS ((100 * 60) * 1000) /* report 1 times per 100 minutes */
#define XT_REPORT_COMM_LEN 16
#define XT_START_FRAME 1
#define XT_REPORT_STACK_LAYER 8

enum report_reason {
	ALLOC_LATENCY = 0,
	ALLOC_FAILED,
	LARGE_ORDER,
	REASON_MAX_COUNT,
};

enum report_occur_type {
	DEFAULT_TYPE = 0,
	HIGH_ORDER,
	OCCUR_MAX_TYPE,
};

struct xacct_report_s {
	struct raw_work work;
	char comm[XT_REPORT_COMM_LEN];
	char msg[XT_REPORT_MAX_LEN];
	vatomic8_t report_flag;
	pid_t pid;
	struct hmlog_ratelimit_state *rate;
	struct symbol_entry ent;
	enum report_reason reason;
	uintptr_t stack[XT_REPORT_STACK_LAYER];
	raw_atomic_long_t occur_times[OCCUR_MAX_TYPE];
};

/* xacct core functions */
struct xacct *xacct_current_xacct(const char **metric_names);
void xacct_init(struct xacct *xacct, struct xacct_mgr *xm, struct xacct_data *xd);
void xacct_metric_init(struct xacct *xacct, uint64_t *lat_slot, uint64_t *cnt_slot, uint32_t slot_num);
void xacct_start(struct xacct *xacct);
void xacct_action_start(struct xacct *xacct, int action);
uint64_t xacct_action_end(struct xacct *xacct, int action);
uint64_t xacct_end(struct xacct *xacct);
int xacct_module_init(void);
bool xacct_is_ready(const struct xacct *xacct, uint32_t *fault, const char **metric_names);
struct xacct_report_s *fetch_xacct_report_work(enum report_reason reason);
void xacct_report_add_work(struct xacct_report_s *work_info);
void xacct_init_flag(struct xacct_report_s *work_info);
int xacct_report_work_ready(uint32_t paf, enum report_reason reason);
int xacct_report_stack(char *buf, int *buf_len, enum report_reason reason);
int xacct_report_zonefree(char *buf, int *buf_len, uint32_t curr_order);

#ifdef CONFIG_XACCT
int xacct_report_module_init(void);
#else
static inline int xacct_report_module_init(void)
{
	hm_warn("xacct report does not init\n");
	return E_HM_OK;
}
#endif

#ifdef CONFIG_XACCT_FAULT_INJECT
#define XACCT_LOOP_DELAY_OFFSET (22)
static inline void xacct_loop_delay(int count)
{
	unsigned long long cnt;
	if (count < 0) {
		return;
	}
	cnt = ((unsigned long long)count) << XACCT_LOOP_DELAY_OFFSET;
	while (cnt != 0) {
		asm volatile ("");
		cnt--;
	}
}
#else
static inline void xacct_loop_delay(__maybe_unused unsigned long count) { }
#endif

#define XACCT_NSEC_PER_SEC 1000000000ULL
#define XACCT_NSEC_PER_MSEC 1000000U
#define XACCT_MSEC_PER_SEC 1000U
#define XACCT_SEC_PER_MIN 60
#define XACCT_NS_FROM_SPEC(time) (((time).tv_sec * 1000000000LL + (time).tv_nsec))

#define XACCT_ENTRY(xacct_type, xa) \
	container_of((xa), struct xacct_type, xacct)

/* xacct data functions */
void xacct_data_slot_inc(struct xacct_data *xd, uint32_t slot, uint32_t metric_type,
			 uint64_t lat, uint64_t val);
void xacct_data_scene_inc(struct xacct_data *xd, uint32_t metric, uint64_t lat, uint64_t val);
void xacct_data_action_collector(struct xacct *xacct, int action, uint64_t lat);
void xacct_data_collector(struct xacct *xacct, uint64_t lat);
void xd_collector_cb_def(struct xacct *xacct, uint64_t lat);

/* xacct show functions */
int xacct_show(char *data, size_t data_len, struct xacct_mgr *xm);
void xs_timeout_cb_def(struct xacct *xacct);

/* xacct manager functions */
int xacct_mgr_read(char *data, size_t data_len);
int xacct_mgr_write(uint64_t pos, const char *src, size_t size, size_t *wsize);
int xacct_mgr_init(struct xacct_mgr *xm, struct xacct_data_helper *xdh, uint32_t scene_num);
void xm_switch_show_cb_def(struct seq_file_s *seq, struct xacct_mgr *xm);

/* xacct watchdog functions */
uint32_t xacct_watchdog_timeout_get(void);
int xacct_watchdog_timeout_set(uint32_t val);

#ifdef CONFIG_USE_PREEMPT_PROCESS
static inline void xacct_watchdog_lock(struct uspinlock_s *usl)
{
	uspinlock_lock(usl);
}
static inline void xacct_watchdog_unlock(struct uspinlock_s *usl)
{
	uspinlock_unlock(usl);
}
#else
static inline void xacct_watchdog_lock(struct raw_mutex *mutex)
{
	raw_mutex_lock(mutex);
}
static inline void xacct_watchdog_unlock(struct raw_mutex *mutex)
{
	raw_mutex_unlock(mutex);
}
#endif
#endif
