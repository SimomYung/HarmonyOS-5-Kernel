/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost startup flow
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_H__
#define __DEVHOST_H__

#include <lib/dlist.h>
#include <devhost/plugin.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/sched/priority.h>

#include <libdevhost/devhost.h>
#include "devhost_backend.h"

#define DH_IRQ_TNAME_MAX	16UL

/* devhost thread name */
#define DH_ACTV_ELASTIC_TNAME	"dh-actv-elastic"
#define DH_IRQ_AFF_TNAME	"dh-irq-bak"
#define DH_IRQ_TNAME		"dh-irq-bind"
#define DH_KVIC_TNAME		"dh-kvic"
#define DH_TIMER_TNAME		"dh-timer"
#define DH_NETTX_TNAME		"dh-net-tx"
#define DH_DEFAULT_WORKQ_TNAME	"dh-wq"
#define DH_SMPCALL_WORKQ_TNAME	"dh-smpwq"
#define DH_BIO_WORKQ_TNAME	"dh-biowq"
#define DH_SETTIME_TNAME	"dh-settime"

/* Ensure that smpcall has a lower priority than migration */
#define DH_SMPCALL_PRIO (HM_PRIORITY_DH_NORMAL - 2)
#define DH_THREAD_SCHED_LEVEL_MAX 2

/* backend init call */
int init_backend_dllib(const char* backend);

int devhost_ap_init(cref_t *ap_cref);
void devhost_pm_ap_table_init(void);
int devhost_irq_setup(void);
void devhost_irq_ap_table_init(void);
void devhost_prepare_pm_actvs(void);
int devhost_file_mapping_init(void);
int devhost_do_postinit(void);

int devhost_actvpool_elastic(void);
void devhost_init_actvs_create(void);

#ifdef CONFIG_SEC_SERVICE
int devhost_reg_sec_mgmt(void);
#endif

__cpuset_t devhost_proc_default_affinity(void);
__cpuset_t devhost_get_default_affinity(void);

static inline void backend_sec_syscall_enter(void) { backend_syscall_enter(SYSCALL_ENTER_FULL_INFO); }
static inline void backend_nosec_syscall_enter(void) { backend_syscall_enter(SYSCALL_ENTER_NO_SEC); }
static inline void backend_nomm_syscall_enter(void) { backend_syscall_enter(SYSCALL_ENTER_NO_MM); }

#define DEFINE_SYSCALL_NOSEC_HDLR(name, ...) \
	DEFINE_ACTVHDLR_HOOK_ALS(name, backend_nosec_syscall_enter, backend_syscall_exit, __VA_ARGS__)

#define DEFINE_SYSCALL_HDLR(name, ...) \
	DEFINE_ACTVHDLR_HOOK_ALS(name, backend_sec_syscall_enter, backend_syscall_exit, __VA_ARGS__)

#define DEFINE_SYSCALL_NOMM_HDLR(name, ...) \
	DEFINE_ACTVHDLR_HOOK_ALS(name, backend_nomm_syscall_enter, backend_syscall_exit, __VA_ARGS__)

#define DEFINE_SYSCALL_NO_ENTER_HDLR(name, ...) \
	DEFINE_ACTVHDLR_POSTHOOK_ALS(name, backend_syscall_exit, __VA_ARGS__)

#define DEFINE_SYSCALL_HDLR_LONG(name, ...) \
	DEFINE_ACTVHDLR_HOOK_LONG_ALS(name, backend_sec_syscall_enter, backend_syscall_exit, __VA_ARGS__)

#define DEFINE_SYSCALL_NOSEC_HDLR_LONG(name, ...) \
	DEFINE_ACTVHDLR_HOOK_LONG_ALS(name, backend_nosec_syscall_enter, backend_syscall_exit, __VA_ARGS__)

enum devhost_barrier_level {
	DEVHOST_BARRIER_LEVEL_PRE,
	DEVHOST_BARRIER_LEVEL_CORE,
	DEVHOST_BARRIER_LEVEL_POST,
	DEVHOST_BARRIER_LEVEL_MAX,
};

struct devhost_barrier {
	struct dlist_node node;
	int (*barrier_fn)(void *arg);
	void *arg;
	enum devhost_barrier_level level;
};

/*
 * Register a barrier that will be called before backend is initialized.
 *
 *  @param	barrier_fn	[I] barrier function
 *  @param	arg		[I] parameters to the barrier function
 *  @param	level		[I] barrier level, it determines the order
 *				    in which the barrier is executed
 *
 *  @return   E_HM_OK for success, hmerrno for errors
 */
int devhost_register_barrier(int (*barrier_fn)(void *arg), void *arg,
			     enum devhost_barrier_level level);

bool dh_is_root(void);
int devhost_sec_chk_dispatch(int callno, unsigned long long sender, unsigned long credential);
extern unsigned int g_dh_self_cnode_idx;

#endif /* __DEVHOST_H__ */
