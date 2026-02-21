/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: DAMON types
 * Author: Huawei CBG OS Lab
 * Create: Wed Mar 20 2024
 */
#ifndef SYSMGR_DAMON_TYPES_H
#define SYSMGR_DAMON_TYPES_H

/*
 * Own DAMON types file is needed for more fast trace point generation.
 * Here we put only base structures DAMON is using which are inspected
 * inside traces.
 */

#include <lib/dlist.h>

#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_completion.h>
#include <intapi/thread/thread.h>

/* Address range watched [start, end) */
struct damon_addr_region {
	unsigned long start;
	unsigned long end;
};

/* Memory region watched */
struct damon_region {
	/* Associated address region */
	struct damon_addr_region addr_region;
	/* To link regions */
	struct dlist_node list;
	/* An address within region which we probe for access */
	unsigned long access_addr;
	/* Number of accesses */
	unsigned int nr_accesses;
	/* Number of accesses in base points */
	unsigned int nr_accesses_bp;
	/* Aging of the region */
	unsigned int age;
	/* Last reported number of accesses */
	unsigned int nr_reported;
};

/* Watched regions associated with a process */
struct damon_target {
	/* Process memory belongs to */
	struct pid_s *pid;
	/* Regions linked */
	struct dlist_node regions_list;
	/* Link to context */
	struct dlist_node list;
	/* Number of watched regions */
	unsigned int nr_regions;
};

struct damon_context;

/*
 * Operations IDs. We use vaddr for now but need to reserve
 * all implemented in vanilla Linux for userspace compatibility
 * sake, thus use same meaning.
 */
enum damon_ops_id {
	DAMON_OPS_VADDR,  /* Virtual addresses */
	DAMON_OPS_FVADDR, /* Fixed virtual addresses */
	DAMON_OPS_PADDR,  /* Physical addresses */
	NR_DAMON_OPS,
};

/* Low level functions depening on mode of operations (vaddr and etc) */
struct damon_operations {
	/* initialization */
	void (*init)(struct damon_context *context);
	/* update mode related data */
	void (*update)(struct damon_context *context);
	/* preparation stage before access probing */
	void (*prepare_access_monitoring)(struct damon_context *context);
	/* access probing */
	unsigned int (*inspect_accesses)(struct damon_context *context);
	/* test if we should continue monitoring */
	bool (*target_legal)(struct damon_target *t);
};

/* Time parameters shipped from userspace */
struct damon_timing {
	/* Time between cycles pf page access bit probing, usec */
	unsigned long probe_us;
	/* Time between results collection cycles, usec */
	unsigned long collect_us;
	/* Time between operation might be updates, usec */
	unsigned long ops_update_us;
};

/* Monitoring parameters from userspace */
struct damon_params {
	struct damon_timing timing;
	struct {
		unsigned long nr_min;
		unsigned long nr_max;
	} regions;
};

/* Context which describes monitoring kernel thread */
struct damon_context {
	/* monitoring parameters */
	struct damon_params params;

	/* number of probing cycles processed */
	unsigned long probe_us_cnt;
	/* next counter value before collecting results */
	unsigned long collect_us_cnt;
	/* next counter value before operations update */
	unsigned long ops_update_us_cnt;

	/* for waiting until the execution of the kdamond_fn is started */
	struct raw_completion kdamond_started;
	/* for waiting until the execution of the kdamond_fn is stopped */
	struct raw_completion kdamond_stopped;
	/* to notify if the kdamond_fn should be stopped */
	int kdamond_should_stop;

	/* kernel thread which monitors page faults */
	raw_thread_t kdamond;
	/* kernel thread PID */
	pid_t kdamond_tid;
	/* thread id in id generator */
	unsigned int kdamond_id;
	/* order access to kernel thread */
	struct raw_mutex kdamond_lock;

	/* low level operations depending on mode (vaddr and etc) */
	struct damon_operations *ops;

	/* target processes which are monitored */
	struct dlist_node targets_list;
};

/* A query to kdamod threads manager */
struct kdamond_mgr_query {
	/* Where to put thread pointer */
	raw_thread_t *thread;
	/* Thread name to set */
	char *thread_name;
	/* Thread function argument */
	void *arg;
	/* Thread function itself */
	void *(*thread_func)(void *arg);
};

/* kdamond threads manager */
struct kdamond_mgr_data {
	/* Manager which monitors #PF */
	raw_thread_t thread;
	/* Thread sync points and lock */
	struct raw_completion trigger_req;
	struct raw_completion trigger_ack;
	struct raw_mutex lock;
	/* Busy flag */
	bool busy;
	/* Caller query */
	struct kdamond_mgr_query query;
};

#endif /* SYSMGR_DAMON_TYPES_H */
