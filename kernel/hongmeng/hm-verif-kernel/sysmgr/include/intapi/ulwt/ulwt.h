/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Declare ulwt interface
 * Author: Huawei OS Kernel Lab
 * Create: Tues Jan 10 18:21:32 2022
 */
#ifndef SYSMGR_INTAPI_ULWT_H
#define SYSMGR_INTAPI_ULWT_H

#include <stdint.h>
#include <libalgo/id_generator.h>
#include <libhmsync/raw_mutex.h>
#include <lib/dlist.h>
#include <libsec/cred_common.h>
#include <hmkernel/ctrlmem/kobjs/ulwt.h>
#include <libhmactv/actv.h>
#include <intapi/process/proc_prctl.h>

#include <libhmsrv_sys/hm_ulwt.h>

#define ULWT_DOMAIN_HASH_TABLE_ORDER (7)
#define ULWT_DOMAIN_HASH_TABLE_SIZE (1UL<<ULWT_DOMAIN_HASH_TABLE_ORDER)

#define ULWT_PROCESS_MAX_EVENT 512

enum ulwt_sync_cred_type_e {
	ULWT_SYNC_CRED_TYPE_NON = 0,
	ULWT_SYNC_CRED_TYPE_SETUID,
	ULWT_SYNC_CRED_TYPE_SETRESUID,
	ULWT_SYNC_CRED_TYPE_SETRESGID,
	ULWT_SYNC_CRED_TYPE_SETGROUPS,
};

struct sync_cred {
	struct base_cred cred;
	enum ulwt_sync_cred_type_e type;
	struct raw_mutex mutex;
	int futex;
	struct raw_refcnt sync_cnt;
};

struct process_ulwt_event_s {
	struct raw_mutex event_mutex;
	/* Reading ulwtfd will wait on event_cnt until ulwt event available */
	int event_cnt;
	struct dlist_node events;
};

struct process_ulwt_kernel_resources_s {
	struct raw_mutex mutex;
	/* The following members are protected by mutex */
	int monitor_sysmgr_kshmd;
	int monitor_u_kshmd;
	uintptr_t monitor_page_uva;
	struct __ctrlmem_kobj_ulwt_s *ukobjulwt;
	int kinfopage_sysmgr_kshmd;
	int kinfopage_u_kshmd;
	uintptr_t kinfopage_uva;
	struct __ctrlmem_kobj_ulwt_s *ukobjulwt_kinfo;
};

struct process_ulwtinfo_s {
	int infopage_sysmgr_kshmd;
	struct ulwt_infopage_s *infopage;

	int infopage_u_kshmd;
	struct ulwt_infopage_s *u_infopage;
	raw_atomic_int_t u_infopage_state;

	struct id_generator *vec_id_gen;

	struct process_ulwt_event_s event_info;

	struct {
		struct dlist_node pending_ulwt_list;
		unsigned long sig_entry;
	} signal;

	struct process_ulwt_kernel_resources_s kern_resources;

	struct sync_cred sync_cred;

	int trigger_kill_nr;
	raw_atomic_int_t nr_klt;
	raw_atomic_int_t nr_ulwt;
};

typedef struct signal_state_s signal_state_t;
typedef struct signal_fsm_s signal_fsm_t;

enum signal_state_id_e {
	SIGNAL_STATE_ID_INVALID = 0x0,
	SIGNAL_STATE_ID_IDLE = 0x1,
	SIGNAL_STATE_ID_WAIT_BIND = 0x2,
	SIGNAL_STATE_ID_PRE_DO_SIGNAL = 0x4,
	SIGNAL_STATE_ID_DESTROYED = 0x8,
};

enum silk_role_e {
	SILK_ROLE_INVALID = 0x0,
	SILK_ROLE_ULWT = 0x1,
	SILK_ROLE_WORKER = 0x2,
	SILK_ROLE_HELPER = 0x3,
};

struct signal_state_s {
	enum signal_state_id_e id;
	int valid_next_state;
	const char *name;
	int (*deliver_hmsignal)(struct thread_s *thread, bool has_thread_alive, int hmsignum, struct thread_s *pair);
	void (*bound)(struct thread_s *thread, struct thread_s *pair);
	void (*do_signal)(struct thread_s *thread, struct thread_s *pair);
	void (*sig_restore)(struct thread_s *thread, unsigned long long wu_desc_addr, unsigned long long sigmask);
	void (*destroy)(struct thread_s *thread);
};

struct signal_fsm_s {
	const signal_state_t *state;
	/*
	 * 1. ulwt->pair is proxy klt
	 * 2. klt->pair is current deal with ulwt
	 */
	struct thread_s *pair;
};

struct proxy_timer {
	int tid;
	unsigned long long cref;
};

struct thread_ulwt_s {
	enum silk_role_e role;
	bool is_vip;
	int bind_futex; // for ULWT_EVT_BIND_WU_XS
	int update_sched_futex; // for ULWT_EVT_UPDATE_SCHED
	struct raw_mutex ulwt_sched_lock;
	unsigned long long wu_desc_addr;
	/* The index of thread_ulwt_vec which indicates current ulwt for thread. */
	int ulwt_vec_idx;
	struct {
		__u32 cpuset;
	} sched;

	uint64_t domain_attr;

	struct {
		struct raw_mutex mutex;
		struct dlist_node pending_ulwt_node;
		signal_fsm_t fsm;
		bool is_sigfutexwait;
		struct thread_s *self_sig_klt;
		struct __hmsigmask pending_hmsignal;
	} signal;
	struct proxy_timer timer;
};
#endif
