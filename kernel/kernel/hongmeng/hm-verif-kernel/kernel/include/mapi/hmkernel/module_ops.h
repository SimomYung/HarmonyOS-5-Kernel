/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Definition of module operations
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 25 10:00:31 2019
 */

#ifndef MAPI_HMKERNEL_MODULE_OPS_H
#define MAPI_HMKERNEL_MODULE_OPS_H

#include <lib/dlist.h>
#include <hmkernel/rq.h>
#include <hmkernel/tcb.h>
#include <hmkernel/uaccess.h>
#include <hmkernel/compiler.h>
#include <hmkernel/scheduler.h>
#include <hmkernel/tcb/struct.h>

#include <uapi/hmkernel/module_ops.h>
#include <uapi/hmkernel/sched_module.h>

struct tcb_s;
struct actv_s;
struct capability_s;
struct module_s;
struct capcall_info_s;
struct scheduler_s;
struct trigger_listener_s;

struct module_ops_s {
	struct dlist_node list;

	char name[CAP_MODULE_NAME_LEN];
	struct module_s *module_addr;
	/* modules ops abstraction. */
	long (*modif_handler)(struct tcb_s *caller);
	int (*module_init)(const void *addr);
	int (*module_destroy)(struct module_s *mod);

	int (*module_ioctl)(struct capcall_info_s *callinfo,
		      unsigned long cmd,
		      const void __user *args, size_t args_size,
		      void __user *result, size_t result_size);
};

struct module_s {
	struct capability_s *pcap;
	struct module_ops_s *mops;
};

void module_ops_register(struct module_ops_s *mops);
struct module_ops_s* module_ops_lookup(const char *name);
struct CNode_s *module_callinfo_caller_of(struct capcall_info_s *callinfo);
struct tcb_s *module_tcb_acquire(struct tcb_s *caller_tcb, cref_t thread_cref);
void module_tcb_release(struct tcb_s *tcb);
void module_scheduler_release(const struct scheduler_s *scheduler);
struct scheduler_s *
module_scheduler_acquire(const struct capcall_info_s *callinfo, cref_t scheduler_cref);
struct trigger_listener_s *
module_tcb_listener_of(struct tcb_s* tcb);
int module_curr_caller_perm_check(const struct capcall_info_s *callinfo, u64 req_perm);

static inline __always_inline int
module_copy_from_user(void *dst, size_t dst_size, const void __user *src,
		      size_t src_size)
{
	int err;
	if ((src == NULL) || (src_size != dst_size)) {
		err = E_HM_INVAL;
	} else {
		err = (int)copy_from_user_faultin(dst, src, src_size);
	}
	return err;
}

static inline __always_inline int
module_copy_to_user(void __user *dst, size_t dst_size, const void *src,
		    size_t src_size)
{
	int err;
	if ((src == NULL) || (src_size != dst_size)) {
		err = E_HM_INVAL;
	} else {
		err = (int)copy_to_user_faultin(dst, src, src_size);
	}
	return err;
}

#ifdef CONFIG_MODULE_SCHED_CLASS
struct rec_spinlock_s;
extern struct krec_ticketlock_s *sched_rq_lock_of_hook(const struct scheduler_s *scheduler,
						    unsigned int rq, unsigned int cpu);
extern void sched_scheduler_init(struct scheduler_s *scheduler);
extern void sched_scheduler_destroy(const struct scheduler_s *scheduler);
extern void enqueue_hook(struct tcb_s *tcb, unsigned int flags);
extern void dequeue_hook(struct tcb_s *tcb);
extern void sched_switch_scheduler_hook(unsigned int rq, struct tcb_s *tcb,
					const struct scheduler_s *scheduler);

extern int tcb_switch_rq_hook(struct tcb_s *tcb, unsigned int rq, const void __user *args, size_t args_size);

extern struct tcb_s *
pick_next_hook(unsigned int cpu);

extern int sched_tcb_get_prio(const struct tcb_s *tcb);
extern void sched_tcb_get_info(const struct tcb_s *tcb, char *output, unsigned int len);
extern void sched_tcb_pack_info(const struct tcb_s *tcb, unsigned char *sched_info, unsigned int len);
extern void sched_tcb_fill_extra_sched_info(struct tcb_s *tcb, char *output);
extern bool sched_check_misfit_hook(struct tcb_s *tcb, unsigned int cluster_id);

extern void
sched_tcb_init(struct tcb_s *tcb, struct tcb_s *parent, const struct tcb_init_attr_s *attr);

extern void sched_tcb_exit(struct tcb_s *tcb);
extern void sched_tcb_cleanup(const struct tcb_s *tcb);

JAM_EXTERN_INLINE void
sched_tick(unsigned int cpu, struct tcb_s *tcb);

extern void
sched_migrate(unsigned int curr_cpu, unsigned int event);
extern void
sched_out_hook(struct tcb_s *tcb, unsigned int cpu);
extern void
sched_switch_hook(struct tcb_s *prev_tcb, struct tcb_s *next_tcb, unsigned int cpu);
extern void
sched_in_hook(struct tcb_s *tcb, unsigned int cpu);
extern void
sched_pre_resched(struct tcb_s *tcb);
extern void
sched_post_resched(struct tcb_s *tcb, unsigned int cpu);
extern bool sched_irq_resched_check(struct tcb_s *tcb);
extern void sched_consume_ipi_func(unsigned int flags, bool in_ipi);
extern bool sched_tcb_is_low_profile(const struct tcb_s *tcb);
extern struct scheduler_s *sched_tcb_scheduler_of(const struct tcb_s *tcb);
#ifdef CONFIG_TCB_TICKLESS
extern void tcb_tickless_prepare_env_hook(const struct tcb_s *tcb);
#endif

extern int
sched_lt_set_conf_direct(struct tcb_s *caller, struct sched_lt_args_s *args, unsigned long args_size);

extern int
sched_tcb_set_attr(const struct tcb_s *caller, struct tcb_s *target,
		   const void __user *args, size_t args_size);
extern int
sched_tcb_set_attr_direct(struct tcb_s *target, struct sched_module_args_s *args,
			  size_t args_size);

extern int
sched_tcb_get_attr(const struct tcb_s *caller, struct tcb_s *target,
		   __u32 flag, void __user *result, size_t result_size);

#else

static inline bool
sched_irq_resched_check(struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}

#define sched_consume_ipi_func(...)

static inline void
sched_scheduler_init(struct scheduler_s *scheduler)
{
	UNUSED(scheduler);
}

static inline void
sched_scheduler_destroy(const struct scheduler_s *scheduler)
{
	UNUSED(scheduler);
}

static inline void
enqueue_hook(struct tcb_s *tcb, unsigned int flags)
{
	UNUSED(tcb, flags);
}
static inline void
dequeue_hook(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void
sched_switch_scheduler_hook(unsigned int rq, struct tcb_s *tcb,
			    const struct scheduler_s *scheduler)
{
	UNUSED(rq, tcb, scheduler);
}

static inline int
tcb_switch_rq_hook(struct tcb_s *tcb, unsigned int rq, const void __user *args, size_t args_size)
{
	UNUSED(tcb, rq, args, args_size);
	return E_HM_OK;
}

static inline struct tcb_s *
pick_next_hook(unsigned int cpu)
{
	UNUSED(cpu);
	return NULL;
}

static inline int
sched_tcb_get_prio(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return 0;
}

static inline void
sched_tcb_get_info(const struct tcb_s *tcb, char *output, unsigned int len)
{
	UNUSED(tcb, output, len);
}

static inline void
sched_tcb_fill_extra_sched_info(struct tcb_s *tcb, char *output)
{
	UNUSED(tcb, output);
}

static inline bool
sched_check_misfit_hook(struct tcb_s *tcb, unsigned int capacity_level)
{
	UNUSED(tcb, capacity_level);
	return false;
}

static inline void
sched_tcb_init(struct tcb_s *tcb, struct tcb_s *parent, const struct tcb_init_attr_s *attr)
{
	UNUSED(tcb, parent, attr);
}

static inline void sched_tcb_exit(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void sched_tcb_cleanup(const struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void
sched_tick(unsigned int cpu, struct tcb_s *tcb)
{
	UNUSED(cpu);

	if (!sched_need_module_rq(tcb_rq_of(tcb))) {
		/* kernel adopts RR policy with one-tick timeslice. */
		resched_curr(tcb, SCHED_FLAG_YIELD);
	}
}

static inline void
sched_migrate(unsigned int curr_cpu)
{
	UNUSED(curr_cpu);
}

static inline void
sched_out_hook(struct tcb_s *tcb, unsigned int cpu)
{
	UNUSED(tcb, cpu);
}

static inline void
sched_switch_hook(struct tcb_s *prev_tcb, struct tcb_s *next_tcb, unsigned int cpu)
{
	UNUSED(prev_tcb, next_tcb, cpu);
}

static inline void
sched_in_hook(struct tcb_s *tcb, unsigned int cpu)
{
	UNUSED(tcb, cpu);
}

static inline void
sched_pre_resched(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void
sched_post_resched(struct tcb_s *tcb, unsigned int cpu)
{
	UNUSED(tcb, cpu);
}

static inline int
sched_lt_set_conf_direct(struct tcb_s *caller, struct sched_lt_args_s *args, unsigned long args_size)
{
	UNUSED(caller, args, args_size);
	return E_HM_OK;
}

static inline int
sched_tcb_set_attr(const struct tcb_s *caller, struct tcb_s *target, const void __user *args, size_t args_size)
{
	UNUSED(caller, target, args, args_size);
	return E_HM_OK;
}
static inline int
sched_tcb_set_attr_direct(struct tcb_s *target, struct sched_module_args_s *args, size_t args_size)
{
	UNUSED(target, args, args_size);
	return E_HM_OK;
}

static inline int
sched_tcb_get_attr(const struct tcb_s *caller, struct tcb_s *target,
			__u32 flag, void __user *result, size_t result_size)
{
	UNUSED(caller, target, flag, result, result_size);
	return E_HM_OK;
}

static inline bool
sched_tcb_is_low_profile(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}

static inline struct scheduler_s *sched_tcb_scheduler_of(const struct tcb_s *tcb)
{
	return sched_current_scheduler_of_cpu(tcb_cpu_of(tcb));
}

#ifdef CONFIG_TCB_TICKLESS
static inline void tcb_tickless_prepare_env_hook(const struct tcb_s *tcb)
{
	UNUSED(tcb);
}
#endif

#endif

#ifdef __HOST_LLT__
int
llt_cap_modules_init(struct CNode_s *root_cnode);
#endif
#endif
