/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ULWT kinfo which shares info from kernel to user
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 30 14:40:36 2024
 */

#ifndef MAPI_HMKERNEL_ULWT_KINFO_H
#define MAPI_HMKERNEL_ULWT_KINFO_H

#include <lib/refcnt.h>
#include <hmkernel/types.h>
#include <hmkernel/seqlock.h>
#include <hmkernel/klocks.h>
#include <hmkernel/spinlock.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hmkernel/atomic.h>

#include <uapi/hmkernel/ulwt/kinfo.h>

struct CNode_s;
struct tcb_s;

enum ulwt_kinfo_thread_state_s {
	/* the state is set to unregistered by default after init */
	ULWT_KINFO_TCB_STATE_UNREGISTERED = 0,
	ULWT_KINFO_TCB_STATE_RUNNING,
	ULWT_KINFO_TCB_STATE_SLEEPING,
	ULWT_KINFO_TCB_STATE_BLOCKED,
	ULWT_KINFO_NR_TCB_STATES,
};

static const char *g_ulwt_kinfo_thread_state_str[] = {
	"UNREGISTERED",
	"RUNNING",
	"SLEEPING",
	"BLOCKED"
};

static const char *g_ulwt_kinfo_thread_state_abnormal = "ABNORMAL";

static inline const char *ulwt_kinfo_thread_state_str_of(enum ulwt_kinfo_thread_state_s state)
{
	const char *s = NULL;
	if ((int)state < ULWT_KINFO_NR_TCB_STATES && (int)state >= 0) {
		s = g_ulwt_kinfo_thread_state_str[(int)state];
	} else {
		s = g_ulwt_kinfo_thread_state_abnormal;
		klog_error("ulwt: state=%d out of bound\n", (int)state);
	}
	return s;
}

struct ulwt_kinfo_thread_data_s {
	unsigned int domain;
	enum ulwt_kinfo_thread_state_s state;
	struct dlist_node blockaware_tcb_node;
};

#ifdef CONFIG_TSM_FFRT

#define ULWT_KINFO_BLOCKAWARE_MAX_DOMAIN_ID 15
#define ULWT_KINFO_CACHELINE_SIZE 64

struct ulwt_kinfo_blockaware_domaininfo_s {
	unsigned int nr_running;
	unsigned int nr_sleeping;
	unsigned int nr_blocked;
};
struct ulwt_kinfo_blockaware_domaininfo_area_s {
	struct ulwt_kinfo_blockaware_domaininfo_s localinfo[ULWT_KINFO_BLOCKAWARE_MAX_DOMAIN_ID + 1];
	struct ulwt_kinfo_blockaware_domaininfo_s globalinfo;
};
struct ulwt_kinfo_page_s {
	struct __seqcnt_s domaininfo_seq;
	struct ulwt_kinfo_blockaware_domaininfo_area_s info_area;
};

struct ulwt_kinfo_watermark_s {
	unsigned int low;
	unsigned int high;
};
struct ulwt_kinfo_blockaware_wakeupcond_s {
	struct ulwt_kinfo_watermark_s local[ULWT_KINFO_BLOCKAWARE_MAX_DOMAIN_ID + 1];
	struct ulwt_kinfo_watermark_s global;
	bool check_ahead;
};

enum ulwt_kinfo_blockaware_mode_e {
	ULWT_KINFO_BLOCKAWARE_MODE_UNINIT,
	ULWT_KINFO_BLOCKAWARE_MODE_TCB,
	ULWT_KINFO_BLOCKAWARE_MODE_FD,
	ULWT_KINFO_BLOCKAWARE_NR_MODES,
};

struct ulwt_monitorfd_ctx {
	vatomic32_t refcnt;
	/*
	 * This lock protects the whole monitorfd_ctx and it is initialized when the kinfo_metadata
	 * is initialized. Other members are initialized when opening/closing the monitorfd via
	 * init_monitorfd_ctx.
	 */
	struct kspinlock_s lock;
	struct dlist_node poll_list;
	struct dlist_node cpu_local_node;
	int monitorfd;
	vatomic32_t state;
};

enum ulwt_kinfo_monitorfd_state_e {
	ULWT_KINFO_MONITORFD_NOT_TOUCHED,
	ULWT_KINFO_MONITORFD_ON_POSTPROC,
	ULWT_KINFO_MONITORFD_TOUCHED,
};

struct ulwt_monitorfd_poll_node {
	struct dlist_node list_node;
	u64 key;
	int fd;
	int epfd;
};

struct ulwt_kinfo_metadata_s {
	/*
	 * The per-CNode kobj_process->ulwt_kinfo_lock protects:
	 * - the initializing state of kinfo_metadata and kinfopage;
	 * - `cond` and `blocked_monitor_tcb_list` member in kinfo_metadata;
	 * - all writes to kinfo page must be mutual exclusive.
	 */
	struct ulwt_kinfo_blockaware_wakeupcond_s cond;
	struct dlist_node blocked_monitor_tcb_list;
	struct pgstr_s *kinfopage_pgstr;
	struct tcb_s *unique_blockaware_mon_tcb;
	enum ulwt_kinfo_blockaware_mode_e mode;
	bool creating_monitorfd;
	struct ulwt_monitorfd_ctx *monitorfd_ctx;
};

/* This function is called in kobj_thread_ctor. */
static inline void ulwt_kinfo_thread_data_init(struct ulwt_kinfo_thread_data_s *thread_data)
{
	thread_data->domain = 0;
	thread_data->state = ULWT_KINFO_TCB_STATE_UNREGISTERED;
}

static inline void ulwt_kinfo_init_monitorfd_ctx(struct ulwt_monitorfd_ctx *ctx)
{
	/* refcnt is set to 1 since it's opened(open-close pair) */
	__refcnt_set(&ctx->refcnt, (u32)1);
	kspinlock_init(&ctx->lock, LOCK_keventfd_ctx);
	dlist_init(&ctx->poll_list);
	ctx->monitorfd = -1;
	vatomic32_init(&ctx->state, ULWT_KINFO_MONITORFD_NOT_TOUCHED);
}

struct ulwt_kinfo_cpu_local_s {
	struct dlist_node blockaware_mon_tcb_list;
	struct dlist_node monitorfd_wakeup_list;
	bool init;
} __attribute__((aligned(ULWT_KINFO_CACHELINE_SIZE)));

bool ulwt_cnode_is_kinfo_inited(struct CNode_s *cnode);
int ulwt_kinfo_init(struct CNode_s *cnode, unsigned long ukobjulwt,
		    unsigned long kinfopage_uva);
int ulwt_kinfo_destroy(struct CNode_s *cnode);
int ulwt_kinfo_blockaware_register(struct CNode_s *cnode, struct tcb_s *caller_tcb,
				   unsigned int domain);
int ulwt_kinfo_blockaware_unregister(struct CNode_s *cnode, struct tcb_s *caller_tcb);
int ulwt_kinfo_blockaware_wait(struct CNode_s *cnode, struct tcb_s *caller_tcb,
			       unsigned long waitcond_uva);
int ulwt_kinfo_blockaware_wake(struct CNode_s *cnode);
int ulwt_kinfo_blockaware_monitorfd(struct CNode_s *cnode, struct tcb_s *caller,
				    int fd, unsigned long waitcond_uva);
void ulwt_kinfo_block(struct tcb_s *tcb);
void ulwt_kinfo_unblock(struct tcb_s *tcb);
void ulwt_kinfo_postproc(void);
bool ulwt_kinfo_interrupt_waiting(struct tcb_s *tcb);

#ifdef __HOST_LLT__
struct ulwt_kinfo_cpu_local_s *ulwt_kinfo_curr_cpu_local(void);
#endif /* __HOST_LLT__ */

#else /* CONFIG_TSM_FFRT */

struct ulwt_kinfo_metadata_s {};
struct ulwt_kinfo_page_s {};

static inline void ulwt_kinfo_block(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void ulwt_kinfo_unblock(struct tcb_s *tcb)
{
	UNUSED(tcb);
}
#endif /* CONFIG_TSM_FFRT */

#endif /* MAPI_HMKERNEL_ULWT_KINFO_H */
