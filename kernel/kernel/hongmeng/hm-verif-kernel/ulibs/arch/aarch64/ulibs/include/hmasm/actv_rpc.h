/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 20 16:39:36 2019
 */
#ifndef AARCH64_ULIBS_ASM_ACTV_RPC_H
#define AARCH64_ULIBS_ASM_ACTV_RPC_H

#include <hmkernel/capability.h>
#include <hmkernel/types.h>
#include <hongmeng/compiler.h>
#include <stdbool.h>

#include <hmasm/ucontext.h>

#define __RPC_MASK(n)		((1UL << (n)) - 1UL)
#define __RPC_ALIGN(v, a)	(((v) + (a) - 1u) & (~((a) - 1u)))

#ifndef __HOST_LLT__
# define __RPC_DEF_RETVAR(t, n)	register t n asm ("x0")
# define __RPC_FAST_SYSCALL(g, x, r)	\
	asm volatile ("svc %1" : "=r" (r) : "i" (__FAST_SYSCALL_RPC_GROUP##g##_##x))
# define __RPC_CAP_SYSCALL(x, r)	\
	asm volatile ("svc %1" : "=r" (r) :	\
			"i" (__CAP_SYSCALL_SVCNO((unsigned int)hmobj_ActivationPool, (unsigned int)ActivationPool##x##Call)))

#ifdef __HOST_UT__
extern unsigned long test_sp;
# define __RPC_CURR_SP		test_sp
#else
# define __RPC_CURR_SP		({register unsigned long ____sp; \
				  asm volatile ("mov %0, sp" : "=r" (____sp)); ____sp;})
#endif
#else
/* for LLT compiling */
# define __RPC_DEF_RETVAR(t, n)		register t n
# define __RPC_FAST_SYSCALL(g, x, r)	r = 0
# define __RPC_CAP_SYSCALL(x, r)	r = 0
# define __RPC_CURR_SP		0UL
#endif

/*
 * When using capcall: prototype of client actvcall:
 * (unsigned long long rref (r0), unsigned long long rpcinfo (r1), unsigned long user_buf (r2) ...);
 *
 * Prototype of server handler:
 * (unsigned long long rpcinfo (r0), unsigned long credential (r1), unsigned long __unused (r2), ...);
 */
#define __RPC_CAPCALL_HDLR_UNUSED_ARGS	unsigned long __rpc_capcall_hdlr_unused
#define __RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT __rpc_capcall_hdlr_unused

/*
 * When client and server use different data model, more specificly,
 * client: ILP32, server: LP64
 *
 * The server should ignore part of the args in the prototype as we
 * fetch the args from the callee's stack.
 *
 * According to the aapcs, arm32 only has r0-r3
 * which are used to pass argument values into a subroutine.
 *
 * When using actvcall:
 * prototype of client actvcall:
 *  For fallback call:
 *  (unsigned long long rpcinfo (r0, r1),
 *   unsigned long send_buf (r2),
 *   unsigned long rply_buf (r3),
 *   unsigned long rply_buf_size, __ARGS__ ([sp]) ...);	<-- ignore the `rply_buf_size` in the stack
 *
 *  For normal call:
 *  (unsigned long long rpcinfo (r0, r1),
 *   unsigned long buf (r2),
 *   __ARGS__ (r3 / [sp]) ...);				<-- ignore nothing in the stack
 *
 * When using capcall:
 * prototype of client actvcall:
 * For fallback call:
 * (unsigned long long rref (r0, r1),
 *  unsigned long long rpcinfo (r2, r3),
 *  unsigned long send_buf,
 *  unsigned long rply_buf,
 *  unsigned long rply_buf_size, __ARGS__ ([sp]) ...);	<-- ignore first three unused args in the stack
 *
 * For nomal call:
 * (unsigned long long rref (r0, r1),
 *  unsigned long long rpcinfo (r2, r3),
 *  unsigned long buf __ARGS__ ([sp]) ...);		<-- ignore the `buf` in the stack
 *
 * In summary, there are three cases:
 *  1. ignore nothing
 *  2. ignore one arg
 *  3. ignore three args
 *
 * Each arg occupies the __SIZEOF_LONG__ under ILP32, i.e., 4bytes.
 */
#define __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE	0UL
#define __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE	4UL
#define __RPC_ACTVCALL_HDLR_UNUSED_ARGS_THREE	12UL

static inline __always_inline unsigned long long
__arch_actv_xact_rpc_info_gen(bool fwd,
			      bool allow_refwd,
			      unsigned long callno,
			      unsigned long buf_sz,
			      unsigned long stk_sz_ul)
{
	return __RPC_INFO_ENCODE_XACT(fwd, allow_refwd, callno, buf_sz, stk_sz_ul);
}

/* encode register x1 when doing RPC call. */
static inline __always_inline
unsigned long long __arch_actv_rpc_info_gen(unsigned long callno,
					    unsigned long buf_sz,
					    unsigned long stk_sz_ul)
{
	return __arch_actv_xact_rpc_info_gen(false, false, callno, buf_sz, stk_sz_ul);
}

/* Compute stack size according to aapcs */
struct __arch_actv_stksz_state {
	/* Next General-purpose Register Number */
	unsigned int ngrn;
	unsigned long stk_size_arg;
	unsigned long stk_size_str;
};

static inline __always_inline
struct __arch_actv_stksz_state
__arch_actv_rpc_stksz_init(int is_capcall, int is_fallback)
{
	/*
	 * For arm64, ngrn is 2 because leading 2 registers are allocated
	 * for buf and rpcinfo
	 * For fallback call, extra ptr and size are used, take 2 registers.
	 */
	struct __arch_actv_stksz_state ret;

	if (is_capcall == 0 && is_fallback == 0) {
		ret.ngrn	 = 2u;
		ret.stk_size_arg = 0UL;
		ret.stk_size_str = 0UL;
	} else if (is_capcall == 0 && is_fallback != 0) {
		ret.ngrn	 = 4u;
		ret.stk_size_arg = 0UL;
		ret.stk_size_str = 0UL;
	} else if (is_capcall != 0 && is_fallback == 0) {
		ret.ngrn	 = 3u;
		ret.stk_size_arg = 0UL;
		ret.stk_size_str = 0UL;
	} else {
		/* is_capcall && is_fallback */
		ret.ngrn	 = 5u;
		ret.stk_size_arg = 0UL;
		ret.stk_size_str = 0UL;
	}

	return ret;
}

#define __arch_actv_rpc_stksz_alignment(type)	\
	({ struct { type ____v; unsigned char ____c; } ____x; (sizeof(____x) - sizeof(type)) > 8UL ? 16UL : 8UL; })

static inline __always_inline
struct __arch_actv_stksz_state
__arch_actv_rpc_stksz_append(struct __arch_actv_stksz_state s,
			     unsigned long argsize, unsigned long alignment)
{
	unsigned long argsz = argsize;
	unsigned long align = alignment;
	if (argsz > 16u) {
		/*
		 * 16UL for frame. If there are large structures in arguments list,
		 * the stack of caller:
		 *                  ._______.
		 *  --> low address | arg7  |
		 *                  |_______|
		 *                  | arg8  |
		 *                  |_______|
		 *  --> frame       |  x29  |
		 *                  |_______|
		 *                  |  LR   |    |
		 *                  |_______|    |
		 *                  | struct|    V
		 *                  |  ...  |  high addr
		 *                  |  ...  |
		 *                  |_______|
		 */
		if (s.stk_size_str == 0UL) {
#ifdef __clang__
			s.stk_size_str = 24UL;
#else
			s.stk_size_str = 16UL;
#endif
		}
		s.stk_size_str += __RPC_ALIGN(argsz, 16u);
		argsz = 8u;
		align = 8u;
	}

	align = (align > 8UL) ? 16UL : 8UL;
	/* fix alignment */
	if ((s.ngrn < 8u) && ((s.ngrn * 8u) % ((unsigned int)align) != 0u)) {
		s.ngrn++;
	} else {
		if (s.stk_size_arg % align != 0UL) {
			s.stk_size_arg += 8u;
		}
	}

	argsz = __RPC_ALIGN(argsz, 8u);
	if (((8u - s.ngrn) * 8u) >= ((unsigned int)argsz)) {
		s.ngrn += ((unsigned int)argsz) / 8u;
	} else {
		/* once we skip to the stack, set ngrn to 8 */
		s.ngrn = 8u;
		s.stk_size_arg += argsz;
	}
	return s;
}

static inline __always_inline
unsigned long __arch_actv_rpc_stksz_done(struct __arch_actv_stksz_state s)
{
	return __RPC_ALIGN(s.stk_size_arg + s.stk_size_str, 16u);
}

static inline __always_inline
unsigned long __a64_actv_curr_stack(void)
{
#ifndef __HOST_LLT__
# ifndef __clang__
	/*
	 * clang reports uninitialized unsigned long.
	 * In #else branch compiler doesn't understand the "mov ..." insn
	 * so doesn't reason correct dependency. Fall back when the first
	 * way is not usable.
	 */
	register unsigned long sp asm ("sp");
# else
	unsigned long sp = 0UL;

	asm volatile ("mov %0, sp" : "=r" (sp));
# endif
#else
	const unsigned long sp = 0UL;
#endif
	return sp;
}

/*
 * NOTE:
 *   The alignment is 4 or 8 according to the aapcs, it's used to align
 *   the server stack when fetch args for 32-bit apps, not intend for
 *   the 64-bit client which shares the same data model with the server.
 */
static inline __always_inline
unsigned int
__arch_actv_rpc_stk_cursor_align(unsigned int cursor, unsigned long alignment)
{
	unsigned int cur;
	unsigned long align = (alignment > 4UL) ? 8UL : 4UL;

	/* cursor align up to align */
	cur = __RPC_ALIGN(cursor, (unsigned int)align);

	return cur;
}

/* AAPCS: if size of data larger than 16, it should be copied to memory and pass a pointer. */
#define __ARCH_ACTV_RPC_PASS_ARG(a, l)	__builtin_choose_expr(sizeof(a) > 16u, \
						(unsigned long)(&l) - __a64_actv_curr_stack(), \
						a)

/*
 * When server actv recving arguments using bridge handler, it builts a frame like this:
 *
 * --> low address  ._______.  <-- SP after prologe
 *                  | arg7  |  <-- copied from caller provided stack
 *                  | arg8  |
 *                  |_______|
 *                  | x29   |  <-- __builtin_frame_address(0)
 *                  | LR    |  <-- x29 is set to sp before entering this function
 *                  |_______|      bridge handler has nonoframe optimization option so this is mandatory
 *                  | arg7  |  <-- x29 real position
 *                  |_______|
 *                  | arg8  |
 *                  |_______|
 *                  |  ---  |  <-- caller x29, useless     compiler thinks callee x29 points to this addr
 *                  |_______|
 *                  |  ---  |  <-- caller LR, useless
 *                  |_______|
 *                  | struct|
 *                  |  ...  |
 *                  |  ...  |
 * --> high address |_______|
 *
 * The size of memory that used for recving stack args is 256 bytes under aarch64.
 *
 */
/*
 * If the argument type is a composite type which size is larger than 16bytes,
 * we should check the offset of the argument.
 *
 *                  +-----+ <-- stk_max_sz
 *                  |     |
 *                  | ... | <-- invalid offset
 *                  ._____. <-- stk_sz
 *                  |     |
 *                  |     | <-- offset1
 *                  |     | <-- offset2
 *                  | ... |
 * --> low address  +-----+ <-- stk_start
 */
#define __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#define __ARCH_ACTV_RPC_RECV_ARG(t, a)	__builtin_choose_expr(sizeof(a) > 16u,		\
			({								\
				((ptr_to_ulong(&a) <= __stacksz) && 			\
				 (ptr_to_ulong(&a) + sizeof(a) <= __stacksz)) ? 	\
				(ptr_to_ulong(&a) + __p_prev_frame[0]) : __p_prev_frame[0]; \
			 }),								\
			a)

/* stack arg size encoded in the rpcinfo, 5bits */
#define __ARCH_ACTV_STACK_ARG_SIZE	(sizeof(unsigned long) * 32UL - 1UL)

struct arch_actv_local {
	/*
	 * put rply_buf and rply_recv_buf_sz together
	 * so we can load 2 registers use ldp
	 */
	union {
		void *rply_buf;
		unsigned long kcall_retval;
	};
	unsigned long rply_recv_buf_sz;
	void *recv_buf;
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
	unsigned long long tls_reg;
	asm volatile ("mrs %0, TPIDR_EL0" : "=r" (tls_reg));
	return (unsigned long)tls_reg;
#else
	extern unsigned long tls_reg_tpid;
	return (unsigned long)tls_reg_tpid;
#endif
}

extern void arch_actv_emrg_entry(void *attr);

#endif
