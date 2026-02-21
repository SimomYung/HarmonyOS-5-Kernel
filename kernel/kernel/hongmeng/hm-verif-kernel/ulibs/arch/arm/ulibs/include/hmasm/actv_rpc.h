/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Actv rpc
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 20 16:39:48 2019
 */
#ifndef ARM_ULIBS_ASM_ACTV_RPC_H
#define ARM_ULIBS_ASM_ACTV_RPC_H

#include <hmkernel/capability.h>
#include <libstrict/strict.h>
#include <hongmeng/compiler.h>
#include <hongmeng/errno.h>
#include <stdbool.h>

#include <hmasm/ucontext.h>

#define __RPC_MASK(n)		((1UL << (n)) - 1UL)
#define __RPC_ALIGN(v, a)	(((v) + (a) - 1u) & (~((a) - 1u)))

#ifndef __HOST_LLT__
# define __RPC_DEF_RETVAR(t, n)	register t n asm ("r0")
# define __RPC_FAST_SYSCALL(g, x, r)	\
	asm volatile ("svc %1" : "=r" (r) : "i" (__FAST_SYSCALL_RPC_GROUP##g##_##x))
# define __RPC_CAP_SYSCALL(x, r)       \
	asm volatile ("svc %1" : "=r" (r) :     \
			"i" (__CAP_SYSCALL_SVCNO(hmobj_ActivationPool, ActivationPool##x##Call)))

#ifdef __HOST_UT__
extern unsigned long test_sp;
# define __RPC_CURR_SP test_sp
#else
# define __RPC_CURR_SP		({register unsigned long ____sp; \
				  asm volatile ("mov %0, sp" : "=r" (____sp)); ____sp;})
#endif
#else
/* for LLT compiling */
# define __RPC_DEF_RETVAR(t, n)		register t n
# define __RPC_FAST_SYSCALL(g, x, r)	r = 0
# define __RPC_CAP_SYSCALL(x, r)	r = 0
# define __RPC_CURR_SP 0UL
#endif

/*
 * When using capcall: prototype of client actvcall:
 * (unsigned long long rref (r0, r1),
 *  unsigned long long rpcinfo (r2, r3),
 *  unsigned long user_buf ([sp]) ...);
 *
 * Prototype of server handler:
 * (unsigned long long rpcinfo (r0, r1),
 *  unsigned long credential (r2),
 *  unsigned long __unused1 (r3),
 *  unsigned long __unused1 ([sp]),...);
 */
#define __RPC_CAPCALL_HDLR_UNUSED_ARGS	\
	unsigned long __rpc_capcall_hdlr_unused_1, unsigned long __rpc_capcall_hdlr_unused_2
#define __RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT __rpc_capcall_hdlr_unused_1, __rpc_capcall_hdlr_unused_2

static inline __always_inline unsigned long long
__arch_actv_xact_rpc_info_gen(bool fwd,
			      bool allow_refwd,
			      unsigned long callno,
			      unsigned long buf_sz,
			      unsigned long stk_sz_ul)
{
	return __RPC_INFO_ENCODE_XACT(fwd, allow_refwd, callno, buf_sz, stk_sz_ul);
}

/* encode register r1 and r2 when doing RPC call. */
static inline __always_inline
unsigned long long __arch_actv_rpc_info_gen(unsigned long callno,
					    unsigned long buf_sz,
					    unsigned long stk_sz_ul)
{
	return __arch_actv_xact_rpc_info_gen(false, false, callno, buf_sz, stk_sz_ul);
}

/* Compute stack size according to aapcs */
struct __arch_actv_stksz_state {
	/* Next Core Register Number */
	unsigned int ncrn;
	unsigned long stk_size;
};

/* avoid using bool because we are not sure if stdbool is usable */
static inline __always_inline
struct __arch_actv_stksz_state
__arch_actv_rpc_stksz_init(int is_capcall, int is_fallback)
{
	/*
	 * For arm32, ncrn is 3 because leading 3 registers are allocated
	 * for buf and rpcinfo.
	 * For fallback call, extra ptr and size are used, take 2 arguments.
	 */
	struct __arch_actv_stksz_state ret;

	if (!is_capcall && !is_fallback) {
		ret.ncrn	= 3u;
		ret.stk_size	= 0UL;
	} else if (!is_capcall && is_fallback) {
		ret.ncrn	= 4u;
		ret.stk_size	= 4UL;
	} else if (is_capcall && !is_fallback) {
		ret.ncrn	= 4u;
		ret.stk_size	= 4UL;
	} else {
		/* is_capcall && is_fallback */
		ret.ncrn	= 4u;
		ret.stk_size	= 12UL;
	}
	return ret;
}

#define __arch_actv_rpc_stksz_alignment(type)	\
	({ struct { type ____v; unsigned char ____c; } ____x; (sizeof(____x) - sizeof(type)) > 4UL ? 8UL : 4UL; })

static inline __always_inline
struct __arch_actv_stksz_state
__arch_actv_rpc_stksz_append(struct __arch_actv_stksz_state s,
			     unsigned long argsize, unsigned long alignment)
{
	unsigned long argsz = __RPC_ALIGN(argsize, 4u);
	unsigned long align = (alignment > 4UL) ? 8UL : 4UL;

	/* fix alignment */
	if ((s.ncrn < 4u) && ((s.ncrn * 4u) % align != 0UL)) {
		s.ncrn++;
	} else {
		if (s.stk_size % align != 0UL) {
			s.stk_size += 4u;
		}
	}

	if ((4u - s.ncrn) * 4u >= argsz) {
		s.ncrn++;
	} else {
		argsz -= (4u - s.ncrn) * 4u;
		s.ncrn = 4u;
		s.stk_size += argsz;
	}
	return s;
}

static inline __always_inline
unsigned long __arch_actv_rpc_stksz_done(struct __arch_actv_stksz_state s)
{
	/* align up to 4 bytes is OK */
	return __RPC_ALIGN(s.stk_size, 4u);
}

/*
 * Note:
 *  It's used to align the 64-bit server stack when fetch args for 32-bit apps.
 *  Thus it's useless in arm32, as the server & the client both be 32bit.
 */
static inline __always_inline
unsigned int
__arch_actv_rpc_stk_cursor_align(unsigned int cursor, unsigned long align)
{
	do {
		((void)(cursor));
		((void)(align));
	} while (0);
	return E_HM_NOSYS;
}

#define __ARCH_ACTV_RPC_PASS_ARG(a, l)	a
#define __ARCH_ACTV_RPC_RECV_ARG(t, a)	a

struct arch_actv_local {
	union {
		void *rply_buf;
		unsigned long kcall_retval;
	};
	void *recv_buf;
	unsigned long rply_recv_buf_sz;
	int (*init)(struct arch_actv_local *);
	unsigned long rpcinfo;
	void **action_table;
	unsigned long action_table_sz;
	unsigned long __unused;
	struct arch_rpc_regs backup_regs;
};

raw_static_assert(sizeof(struct arch_actv_local) % 8u == 0u,
		  size_of_arch_actv_local_should_aligned_to_8);

extern void arch_actv_rpc_entry(struct arch_actv_local *local);
extern void arch_actv_pure_rpc_entry(void *attr);
extern void arch_actv_pure_wait_rpc_entry(void *attr);
extern void arch_actv_pure_pre_init_rpc_entry(void *attr);

static inline unsigned long arch_atcv_read_tls_register(void)
{
#ifndef __HOST_LLT__
	unsigned long tls_reg;
	asm volatile ("mrc p15, 0, %0, c13, c0, 3" : "=r" (tls_reg));
	return tls_reg;
#else
	extern unsigned long tls_reg_tpid;
	return tls_reg_tpid;
#endif
}

extern void arch_actv_emrg_entry(void *attr);

#endif
