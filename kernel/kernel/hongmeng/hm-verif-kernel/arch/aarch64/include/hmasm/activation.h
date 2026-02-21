/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 26 10:47:11 2018
 */

#ifndef A64_ASM_ACTIVATION_H
#define A64_ASM_ACTIVATION_H

#include <lib/string.h>

#include <hmasm/ucontext.h>
#include <hmasm/exception.h>
#include <hmasm/ctrlmem/supervs.h>
#include <hmasm/activation/upcall.h>

#include <hmkernel/errno.h>
#include <hmkernel/strict.h>
#include <hmkernel/jump_label.h>
#include <mapi/hmasm/activation.h>
#include <mapi/uapi/hmkernel/ipc/hmsignal.h>
#include <hmkernel/activation/unblock.h>

/*
 * UNSET: The compat state is not set yet
 * NOCOMPAT: The actv is not compat
 * COMPAT: The actv is compat
 *
 * For the cap actv, set the actv compat state during
 * the actv initialization.
 * For the tcb base actv, set it when the tcb is
 * created or thread flush uctx by the uapps.
 *
 *                      +----+                    +----+
 *                      |    v   (only for base)  |    v
 * +-------+         +--+----+--+  set compat   +-+----+-+
 * | UNSET |         | NOCOMPAT | ------------> | COMPAT |
 * +-------+-------->+----------+ <-----x------ +---+----+
 *         |                        E_HM_INVAL      ^
 *         |                    (cap actv failed)   |
 *         |                    (base actv succeed) |
 *         |                                        |
 *         +----------------------------------------+
 */
/* identify if the actv in the compat mode */
#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
ENUM(5, __arch_actv_conf_compat_state, ACTV_COMPAT_STATE_,
	UNSET,
	NOCOMPAT,
	COMPAT,
	ILP32,
	FAKE_COMPAT, /* Used to run 32-bit programs on cpus that do not support 32-bit */
);
#endif

struct arch_actv_conf_s {
#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
	enum __arch_actv_conf_compat_state state;
#endif
};

#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
static inline void
arch_actv_conf_set_state(struct arch_actv_conf_s *arch_actv_conf,
			 enum __arch_actv_conf_compat_state state)
{
	arch_actv_conf->state = state;
}

static inline enum __arch_actv_conf_compat_state
arch_actv_conf_compat_state_of(const struct arch_actv_conf_s *arch_actv_conf)
{
	return arch_actv_conf->state;
}
#endif

struct __arch_actv_fpu_s {
	/*
	 * Record the fpu_regs allocated for actv:
	 * - base_actv: Use the memory of tcb_s to allocate a fpu_regs
	 *              for the base_actv.
	 * - emrg_actv: Currently, there is no fpu_regs for the emrg_actv.
	 *
	 * - cap actv: Use the remained memory of the cap activation to
	 *             allocated a fpu_regs for the actv in the actvpool.
	 */
	struct arch_fpu_regs *p_fpu_regs;
};

struct arch_actv_s {
	struct __arch_actv_fpu_s fpu;
	struct actv_unblock_s *p_unblock;

#ifdef CONFIG_AARCH64_SVE
	struct arch_sve_regs *p_sve_regs;
#endif

#ifdef CONFIG_PAC
	bool strict_pac;
#endif
};

static inline struct arch_fpu_regs *
arch_actv_fpu_regs_of(struct arch_actv_s *arch_actv)
{
	return arch_actv->fpu.p_fpu_regs;
}

static inline struct actv_unblock_s *
arch_actv_unblock_of(struct arch_actv_s *arch_actv)
{
	return arch_actv->p_unblock;
}

static inline const struct actv_unblock_s *
arch_actv_unblock_of_const(const struct arch_actv_s *arch_actv)
{
	return arch_actv->p_unblock;
}

struct arch_actv_tcb_base_s {
	/*
	 * To expand the available memory in actv_s, use the memory of tcb_s
	 * to allocate a fpu_regs to save/restore the fpu context of base actv.
	 */
#ifdef CONFIG_KOBJ_THREAD
	struct arch_fpu_regs *fpu_regs;
#else
	struct arch_fpu_regs fpu_regs;
#endif

	/* SVE Registers uses huge memory. We use KOBJ's memory. */
#ifdef CONFIG_AARCH64_SVE
	struct arch_sve_regs *p_sve_regs;
#endif

	struct actv_unblock_s unblock;
};

struct tcb_init_attr_s;
int arch_actv_init_tcb_base(struct arch_actv_s *arch_actv,
			    struct arch_actv_tcb_base_s *arch_tcb_base,
			    const struct tcb_init_attr_s *attr);
void arch_actv_fini_tcb_base(const struct arch_actv_tcb_base_s *arch_tcb_base);
int arch_actv_init_pool(struct arch_actv_s *arch_actv);

static inline void
arch_set_thread_area(struct arch_uctx *uctx, void *tls_base)
{
	__REG_TPID(&uctx->regs)		= (u64)(ptr_to_ulong(tls_base));
	__REG_TLS_K(&uctx->regs)	= (u64)(ptr_to_ulong(tls_base));
}

static inline void
arch_actv_conf_init(__maybe_unused struct arch_actv_conf_s *arch_actv_conf)
{
	/* ACTV_COMPAT_STATE_UNSET, need do nothing */
}

static inline void
arch_actv_uctx_set_cred(struct arch_uctx *uctx, unsigned long credential)
{
	/* ARG1 is used to pass credential */
	REG_ARG1(&uctx->regs) = credential;
}

static inline void
arch_actv_uctx_set_sender(struct arch_uctx *uctx,
			  u32 caller_cnode_idx, u32 xact_cnode_idx)
{
	unsigned long sender = ((unsigned long)xact_cnode_idx << 32u) +
				(unsigned long)caller_cnode_idx;

	REG_ARG0(&uctx->regs)	= sender;
}

static inline void
arch_actv_uctx_set_rpc_info(struct arch_uctx *uctx,
			    const struct arch_uctx *caller_uctx,
			    unsigned long rpc_info)
{
	REG_RPCINFO(&uctx->regs) =
		arch_ctrlmem_vspace_supervs_fix_rpc_info(uctx, caller_uctx, rpc_info);
}

static inline void
arch_actv_uctx_set_exp_info(struct arch_uctx *uctx,
			    u64 abort_status, u64 abort_addr, u64 elr)
{
	REG_ARG2(&uctx->regs)	= abort_status;
	REG_ARG3(&uctx->regs)	= abort_addr;
	REG_ARG4(&uctx->regs)	= elr;
}

static inline void
arch_actv_uctx_set_upcall_args(struct arch_uctx *uctx,
			       const union __arch_actv_upcall_args_u *args)
{
	REG_ARG2(&uctx->regs)	= args->buf[__ARCH_ACTV_UPCALL_ARG0];
	REG_ARG3(&uctx->regs)	= args->buf[__ARCH_ACTV_UPCALL_ARG1];
	REG_ARG4(&uctx->regs)	= args->buf[__ARCH_ACTV_UPCALL_ARG2];
}

static inline void
arch_actv_upcall_args_set_pool_page_free(union __arch_actv_upcall_args_u *args, u64 pfn)
{
	args->pool_page_free.pfn = pfn;
}

static inline void
arch_actv_upcall_args_set_hmsignals(union __arch_actv_upcall_args_u *args,
				    struct __hmsigmask hmsignals,
				    u64 timer_cref, u32 restartable_flags)
{
	args->hmsignal.hmsignals	= hmsignals;
	args->hmsignal.timer_cref	= timer_cref;
	args->hmsignal.restartable_flags = restartable_flags;
}

static inline void
arch_actv_upcall_args_set_vsfault(union __arch_actv_upcall_args_u *args,
				  u64 vspace_cref, unsigned long credential)
{
	args->vsfault.vspace_cref = vspace_cref;
	args->vsfault.credential = (u64)credential;
}

static inline void
arch_actv_upcall_args_set_sigmask_args(union __arch_actv_upcall_args_u *args, u64 sigmask)
{
	args->set_sigmask.sigmask = (__u64)sigmask;
}

static inline void
arch_actv_upcall_args_set_epoll_recheck(union __arch_actv_upcall_args_u *args, int fd, u32 events, u32 cnode_idx)
{
	args->ep_recheck.cnode = (__u32)cnode_idx;
	args->ep_recheck.events = (__u32)events;
	args->ep_recheck.fd = (__u32)fd;
}

static inline void
actv_upcall_args_set_select(union __arch_actv_upcall_args_u *args, u32 select_type, u64 tcb_cref)
{
	args->select.type = select_type;
	args->select.tcb_cref = tcb_cref;
}

static inline void
arch_actv_upcall_args_set_ppoll(union __arch_actv_upcall_args_u *args,
				u64 fds, __u32 nfds, cref_t tcb_cref)
{
	args->eventpoll_ppoll.fds = fds;
	args->eventpoll_ppoll.nfds = nfds;
	args->eventpoll_ppoll.tcb_cref = (u64)tcb_cref;
}

static inline void
arch_actv_upcall_args_set_common_file_close(union __arch_actv_upcall_args_u *args,
					    u32 fd, u32 cnode_idx, u32 rpccnt)
{
	args->common_file_close.fd = fd;
	args->common_file_close.cnode_idx = cnode_idx;
	args->common_file_close.rpccnt = rpccnt;
}

static inline void
arch_actv_upcall_args_set_close_on_exec(union __arch_actv_upcall_args_u *args,
					u32 cnode_idx)
{
	args->close_on_exec.cnode_idx = cnode_idx;
}

static inline void
actv_upcall_args_set_rpccnt(union __arch_actv_upcall_args_u *args,
			    u64 common_file, int fd_type)
{
	args->rpccnt.common_file = common_file;
	args->rpccnt.fd_type = fd_type;
}

/*
 * Reduce to unsigned long. NOTE that we only use 32 bits
 * when passing RPCINFO..
 *
 * #define FAST_ACTV_SYSCALL_FETCH_ARG(e, i, b) \
 *	__FAST_ACTV_SYSCALL_ARG_PROTO(FAST_SYSCALL_ARGS, i, b, e)
 * for fastpath.c -> rpc_callinfo_of
 *
 * #define DEFINE_FAST_ACTV_SYSCALL(name, ...) \
 *	__FAST_ACTV_SYSCALL_ARG_PROTO(FAST_SYSCALL_PROTO, __VA_ARGS__, name)
 * for rpc.c
 *
 * #define FAST_ACTV_SYSCALL(name, ...) \
 *	__FAST_ACTV_SYSCALL_ARG_PROTO(FAST_SYSCALL_DEFINE, __VA_ARGS__, name)
 * for syscall.h
 */
#define __FAST_ACTV_SYSCALL_ARG_PROTO(m, i, b, ...)	\
	m##2(__VA_ARGS__, unsigned long, i,		\
	     unsigned long, b)
#define __FAST_ACTV_FALLBACK_SYSCALL_ARG_PROTO(m, i, b, r, rsz, ...)\
	m##4(__VA_ARGS__, unsigned long, i,	\
	     unsigned long, b,			\
	     unsigned long, r,			\
	     unsigned long, rsz)

#define __CAP_ACTV_SYSCALL_ARG_PROTO(m, i, b, ...)	\
	m##2(__VA_ARGS__, unsigned long, i,		\
	     unsigned long, b)
#define __CAP_ACTV_FALLBACK_SYSCALL_ARG_PROTO(m, i, b, r, rsz, ...)\
	m##4(__VA_ARGS__, unsigned long, i,	\
	     unsigned long, b,			\
	     unsigned long, r,			\
	     unsigned long, rsz)
#define ARCH_ACTV_ARG_UNUSED

/*
 * Reduce to unsigned long. NOTE that we only use 32 bits
 * when passing RPCINFO..
 *
 * #define FAST_ACTV_SYSCALL_FETCH_ARG_COMPAT(e, i, b) \
 *	__FAST_ACTV_SYSCALL_ARG_PROTO_COMPAT(FAST_SYSCALL_ARGS, i, b, e)
 * for fastpath.c -> rpc_callinfo_of_compat
 *
 * #define DEFINE_FAST_ACTV_SYSCALL_COMPAT(name, ...) \
 *	__FAST_ACTV_SYSCALL_ARG_PROTO_COMPAT(FAST_SYSCALL_PROTO, __VA_ARGS__, name)
 * for rpc.c
 *
 * #define FAST_ACTV_SYSCALL_COMPAT(name, ...) \
 *	__FAST_ACTV_SYSCALL_ARG_PROTO_COMPAT(FAST_SYSCALL_DEFINE, __VA_ARGS__, name)
 * for syscall.h
 */
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
/* For a32 big endian compatible */
#define __FAST_ACTV_SYSCALL_ARG_PROTO_COMPAT(m, i, b, ...)\
		m##3(__VA_ARGS__, unsigned long, __arg_unused,	\
				  unsigned long, i,		\
				  unsigned long, b)
#define __FAST_ACTV_FALLBACK_SYSCALL_ARG_PROTO_COMPAT(m, i, b, r, rsz, ...)\
		m##5(__VA_ARGS__, unsigned long, __arg_unused,	\
				  unsigned long, i,		\
				  unsigned long, b,		\
				  unsigned long, r,		\
				  unsigned long, rsz)
#define __CAP_ACTV_SYSCALL_ARG_PROTO_COMPAT(m, i, b, ...)\
		m##3(__VA_ARGS__, unsigned long, __arg_unused,	\
				  unsigned long, i,		\
				  unsigned long, b)
#define __CAP_ACTV_FALLBACK_SYSCALL_ARG_PROTO_COMPAT(m, i, b, r, rsz, ...)\
		m##5(__VA_ARGS__, unsigned long, __arg_unused,	\
				  unsigned long, i,		\
				  unsigned long, b,		\
				  unsigned long, r,		\
				  unsigned long, rsz)
#else
/* For a32 little endian compatible */
#define __FAST_ACTV_SYSCALL_ARG_PROTO_COMPAT(m, i, b, ...)	\
		m##3(__VA_ARGS__, unsigned long, i,	\
				  unsigned long, __arg_unused,	\
				  unsigned long, b)
#define __FAST_ACTV_FALLBACK_SYSCALL_ARG_PROTO_COMPAT(m, i, b, r, rsz, ...)\
		m##5(__VA_ARGS__, unsigned long, i,	\
				  unsigned long, __arg_unused,	\
				  unsigned long, b,		\
				  unsigned long, r,		\
				  unsigned long, rsz)
#define __CAP_ACTV_SYSCALL_ARG_PROTO_COMPAT(m, i, b, ...)	\
		m##3(__VA_ARGS__, unsigned long, i,	\
				  unsigned long, __arg_unused,	\
				  unsigned long, b)
#define __CAP_ACTV_FALLBACK_SYSCALL_ARG_PROTO_COMPAT(m, i, b, r, rsz, ...)\
		m##5(__VA_ARGS__, unsigned long, i,	\
				  unsigned long, __arg_unused,	\
				  unsigned long, b,		\
				  unsigned long, r,		\
				  unsigned long, rsz)
#endif /* __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ */
#define ARCH_ACTV_COMPAT_ARG_UNUSED UNUSED(__arg_unused)

static inline void
do_arch_actv_uctx_copy_regs(struct arch_uctx *uctx, struct arch_uctx *caller_uctx,
			    unsigned long rpcinfo)
{
	/* ARG1 is credential */
	REG_ARG2(&uctx->regs) = REG_ARG2(&caller_uctx->regs);
	REG_ARG3(&uctx->regs) = REG_ARG3(&caller_uctx->regs);
	REG_ARG4(&uctx->regs) = REG_ARG4(&caller_uctx->regs);
	REG_ARG5(&uctx->regs) = REG_ARG5(&caller_uctx->regs);
	REG_ARG6(&uctx->regs) = REG_ARG6(&caller_uctx->regs);
	REG_ARG7(&uctx->regs) = REG_ARG7(&caller_uctx->regs);
	arch_actv_uctx_set_rpc_info(uctx, caller_uctx, rpcinfo);
}

static inline void
arch_actv_uctx_copy_regs(struct arch_uctx *uctx, struct arch_uctx *caller_uctx,
			 unsigned long rpcinfo)
{
	do_arch_actv_uctx_copy_regs(uctx, caller_uctx, rpcinfo);
}

static inline unsigned long
arch_actv_stk_xfer_sz(const struct arch_uctx *uctx, unsigned long nr_ul)
{
	unsigned long sz = 0UL;

	if ((uctx->flags & ARCH_UCONTEXT_DATA_32BIT) != (__u64)0UL) {
		sz = (nr_ul * 4UL);
	} else {
		sz = (nr_ul * 8UL);
	}
	return sz;
}

#ifdef __HOST_LLT__
static inline void
llt_arch_actv_uctx_fill_rpcinfo(struct arch_uctx *uctx, unsigned long rpcinfo)
{
	REG_ARG0(&uctx->regs) = rpcinfo;
}

static inline void
llt_arch_actv_uctx_fill_membuf(struct arch_uctx *uctx, void __user *membuf)
{
	REG_ARG1(&uctx->regs) = ptr_to_u64(membuf);
}
static inline __u64
llt_arch_actv_upcall_args_get_pp_free(union __arch_actv_upcall_args_u *args)
{
	return args->pool_page_free.pfn;
}
#endif

static inline bool arch_is_actv_cred_invalid(unsigned long credential)
{
	return (credential >= __ARCH_ACTV_CRED_INVAL_MIN);
}

/*
 * lr of actv uctx is used to indicate whether this actv is inited
 * in user space. lr == 0 means not inited, otherwise is inited.
 */
static inline bool
arch_actv_is_user_inited(const struct arch_uctx *uctx)
{
	return (REG_LR(&uctx->regs) != (__u64)0);
}

#ifdef CONFIG_ACTV_SWITCH_PERF_HOOK
extern struct static_key actv_spe_knob;
extern struct static_key actv_brbe_knob;
extern struct static_key actv_perf_knob;
void arch_actv_call_perf_hooks(struct actv_s *actv);
static inline void actv_switch_perf_hook(struct actv_s *actv)
{
	if (static_branch_unlikely(&actv_perf_knob)) {
		arch_actv_call_perf_hooks(actv);
	}
}
#else
static inline void actv_switch_perf_hook(struct actv_s *actv)
{
	UNUSED(actv);
}
#endif

#endif
