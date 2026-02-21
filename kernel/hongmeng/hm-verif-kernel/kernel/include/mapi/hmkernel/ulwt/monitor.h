/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ULWT thread monitor
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 26 20:26:17 2023
 */
#ifndef MAPI_HMKERNEL_ULWT_MONITOR_H
#define MAPI_HMKERNEL_ULWT_MONITOR_H

#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <hmkernel/strict.h>

struct epoll_kobj_s;
struct fd_s;
struct CNode_s;
struct tcb_s;

#ifdef CONFIG_ULWT

#include <hmkernel/klocks.h>
#include <lib/vsync_atomic.h>
#include <uapi/hmkernel/ulwt/monitor.h>
#include <hmkernel/ctrlmem/kobjs/vspace.h>
#include <hmkernel/pgstr/pgstr.h>

#define ULWT_MONITOR_DISABLED_DOMAIN 0x7FFFUL
#define ULWT_MONITOR_LLT_IOBLOCK_FLAG	0x1000
#define ULWT_MONITOR_NR_EPOLL_ATTACH_SLOTS     4UL

struct ulwt_mon_tcb_data_s {
	/* do not send notification */
	u32 no_exit_notification : 1;
	u32 mon_domain : 15;
	u32 vec_idx : 16;
};

enum ulwt_mon_block_reason {
	ULWT_MON_BLOCK_NOBLOCK,
	ULWT_MON_BLOCK_WORKER_WAIT,
	ULWT_MON_BLOCK_HELPER_MONITOR,
	ULWT_MON_BLOCK_SYNCCALL_MONITOR,
	ULWT_MON_BLOCK_SYNCCALL_WORKER,
	ULWT_MON_BLOCK_HELPER_EPROXY,
	ULWT_MON_BLOCK_NR_BLOCK_REASON,
};

/* Each sched domain has several worker KLTs, these workers have 3 states.
 * 1 running: The worker runs ulwts in user mode normally.
 * 2 waiting: The worker failed to find an ulwt to run since the rq is empty.
 *            Thus, the worker is blocked in kernel via mon_wait.
 * 3 blocked: The worker is blocked together with the ulwt running on it
 *            due to blocking syscalls like ppoll/select.
 */
struct ulwt_mon_domaindata_s {
	/* the list of the waiting worker KLTs */
	struct dlist_node wait_list;
	unsigned int nr_running;
	unsigned int nr_waiting;
	unsigned int nr_blocked;
	bool creating;
};

enum ulwt_mon_synccall_state {
	ULWT_MON_SYNCCALL_FREE,
	ULWT_MON_SYNCCALL_MONITOR,
	ULWT_MON_SYNCCALL_SYSMGR,
	ULWT_MON_SYNCCALL_NR_STATES,
};

struct ulwt_mon_metadata_s {
	/* monitor page */
	struct pgstr_s *monpage_pgstr;
	struct pgstr_s *infopage_pgstr;

	/* worker KLT management */
	struct ulwt_mon_domaindata_s domains[ULWT_MONITOR_NR_DOMAINS];
	/* map from domain_prio_seq id to domain id */
	unsigned int domain_prio_seq[ULWT_MONITOR_NR_DOMAINS];
	/* map from domain id to domain_prio_seq id */
	unsigned int domain_to_prio_seq_idx[ULWT_MONITOR_NR_DOMAINS];
	unsigned int limit;

	/* helper_monitor and worker KLT creation */
	bool in_create_list;
	struct dlist_node create_node;
	struct dlist_node helper_monitor_wait_list;
	unsigned int create_domains_mask;
	unsigned int *create_mask_addr;
	struct tcb_s *helper_monitor;

	/* epoll user-mode blocking */
	struct dlist_node epfd_list;
	struct dlist_node ready_epfd_list;
	struct dlist_node helper_eproxy_wait_list;
	unsigned int nr_ready_epfd;

	/* synccall info */
	enum ulwt_mon_synccall_state synccall_state;
	struct dlist_node synccall_monitor_wait_list;
	struct dlist_node synccall_worker_wait_list;
	struct tcb_s *synccall_lock_owner;
};

union ulwt_value_per_cacheline_s {
	vatomic32_t value;
	unsigned char cacheline[ULWT_MONITOR_CACHELINE_SIZE];
} __attribute__((aligned(ULWT_MONITOR_CACHELINE_SIZE)));

/* Here we cannot directly use __ulwt_mon_page_s from uapi.
 * This is because we use atomic type from libvsync in ulwt_value_per_cacheline_s and this atomic type
 * cannot be used in uapi since libvsync cannot be imported there.
 * Therefore, there are 2 versions (mapi and uapi) of ulwt_value_per_cacheline_s and ulwt_mon_page_s.
 */
struct ulwt_mon_page_s {
	/* offset = 0 bytes */
	union ulwt_value_per_cacheline_s sem[ULWT_MONITOR_NR_DOMAINS];
	/* offset = 832 bytes */
	int epfd_array[ULWT_MONITOR_EPOLL_NUM];
	/* offset = 896 bytes */
	unsigned int create_mask;
} __attribute__((aligned(PAGE_SIZE)));

struct ulwt_mon_epoll_attach_slot_s {
	struct fd_s *fde;
	struct CNode_s *cnode;
	int epfd;
	bool in_ready_list;
	struct dlist_node list_node;
	struct dlist_node ready_list_node;
};

struct ulwt_mon_epoll_attach_s {
	struct ulwt_mon_epoll_attach_slot_s slots[ULWT_MONITOR_NR_EPOLL_ATTACH_SLOTS];
	bool ulwt_enabled;
};

struct ulwt_mon_helper_eproxy_args_s {
	struct tcb_s *tcb;
	int retval;
};

struct ulwt_mon_cpu_local_s {
	bool init;
	bool set;
	struct dlist_node mon_wake_list;
	struct dlist_node mon_create_list;
	struct ulwt_mon_helper_eproxy_args_s helper_eproxy_records[ULWT_MONITOR_NR_EPOLL_ATTACH_SLOTS];
	unsigned int nr_helper_eproxy;
} __attribute__((aligned(ULWT_MONITOR_CACHELINE_SIZE)));

static inline bool ulwt_worker_is_enabled(struct ulwt_mon_tcb_data_s data)
{
	return data.mon_domain != ULWT_MONITOR_DISABLED_DOMAIN;
}

int ulwt_mon_init(struct CNode_s *cnode, unsigned long ukobjulwt,
		  unsigned long limit, unsigned long monitor_page_uva,
		  unsigned long infopage_uva);
bool ulwt_cnode_is_ulwt_inited(struct CNode_s *cnode);
void ulwt_mon_destroy(struct CNode_s *cnode);
void ulwt_mon_post_proc(void);
bool ulwt_mon_interrupt_waiting(struct tcb_s *tcb);
void ulwt_mon_block(struct tcb_s *tcb);
void ulwt_mon_unblock(struct tcb_s *tcb);
void ulwt_mon_epoll_kobj_init_cnode(struct epoll_kobj_s *epoll, struct CNode_s *cnode);
void ulwt_mon_epoll_kobj_init_epfd(struct epoll_kobj_s *epoll, int epfd);
void ulwt_mon_epoll_kobj_init_fde(struct epoll_kobj_s *epoll, struct fd_s *fde);
void ulwt_mon_epoll_kobj_destroy(struct epoll_kobj_s *epoll);
void ulwt_mon_fdtable_close_epfd(struct fd_s *fde);
void ulwt_mon_fdtable_dup_epfd(struct fd_s *old_fde, struct fd_s *new_fde, int new_epfd);
void ulwt_mon_epoll_wakeup_all_epfds(struct epoll_kobj_s *epoll);
bool ulwt_mon_epoll_check_attach_slots(struct epoll_kobj_s *epoll);
int ulwt_mon_synccall_lock(struct CNode_s *cnode, struct tcb_s *caller_tcb);
int ulwt_mon_synccall_unlock(struct CNode_s *cnode, struct tcb_s *caller_tcb);
int ulwt_mon_update_vec_idx(struct tcb_s *caller, int idx);
int ulwt_mon_ulwt_tid_of(struct tcb_s *caller);
int ulwt_mon_curr_tid_of(struct tcb_s *caller);
int ulwt_mon_ulwt_vtid_of(struct tcb_s *caller);
int ulwt_mon_curr_vtid_of(struct tcb_s *caller);
void ulwt_mon_curr_thread_id_of(struct tcb_s *caller, u32 *tid, u32 *vtid);
int ulwt_mon_get_curr_tid(struct tcb_s *caller, u32 *tid, u32 *vtid);
int ulwt_domain_prio_seq_update(struct CNode_s *cnode, const unsigned int __user *domain_prio_seq);
#ifdef __HOST_LLT__
struct timer_s;
void ulwt_mon_wait_handler_impl(struct timer_s *timer);
#endif

#else /* CONFIG_ULWT */

struct ulwt_mon_tcb_data_s {};
struct ulwt_mon_page_s {};
struct ulwt_mon_metadata_s {};

static inline int ulwt_mon_init(struct CNode_s *cnode,
				unsigned long ukobjulwt, unsigned long limit, unsigned long monitor_page_uva,
				unsigned long infopage_uva)
{
	UNUSED(cnode, ukobjulwt, limit, monitor_page_uva);
	return E_HM_NOSYS;
}

static inline void ulwt_mon_destroy(struct CNode_s *cnode)
{
	UNUSED(cnode);
}

static inline void ulwt_mon_block(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void ulwt_mon_unblock(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void ulwt_mon_epoll_kobj_init_cnode(struct epoll_kobj_s *epoll, struct CNode_s *cnode)
{
	UNUSED(epoll, cnode);
}

static inline void ulwt_mon_epoll_kobj_init_epfd(struct epoll_kobj_s *epoll, int epfd)
{
	UNUSED(epoll, epfd);
}

static inline void ulwt_mon_epoll_kobj_init_fde(struct epoll_kobj_s *epoll, struct fd_s *fde)
{
	UNUSED(epoll, fde);
}

static inline void ulwt_mon_epoll_kobj_destroy(struct epoll_kobj_s *epoll)
{
	UNUSED(epoll);
}

static inline void ulwt_mon_fdtable_close_epfd(struct fd_s *fde)
{
	UNUSED(fde);
}

static inline void ulwt_mon_fdtable_dup_epfd(struct fd_s *old_fde, struct fd_s *new_fde, int new_epfd)
{
	UNUSED(old_fde, new_fde, new_epfd);
}

static inline void ulwt_mon_epoll_wakeup_all_epfds(struct epoll_kobj_s *epoll)
{
	UNUSED(epoll);
}

#endif /* CONFIG_ULWT */

#endif
