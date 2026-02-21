/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: POSIX signal entrypoint in VDSO
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 31 16:03:43 2023
 */

/* include this header first to use hm_signal_defs.h instead of signal.h */
#include <libhmsrv_sys/hm_signal.h>

#include <endian.h>
#include <limits.h>
#include <syscall.h>
#include <lib/utils.h>
#include <hmasm/ucontext.h>
#include <hongmeng/types.h>
#include <hongmeng/compiler.h>

static inline void clear_mem(int *ptr, size_t size)
{
	for (size_t i = 0; i * sizeof(*ptr) < size; ++i) {
		ptr[i] = 0;
	}
}

#if defined(__aarch64__)
static inline void sigreturn(const struct sigframe *frame)
{
	register long x0 asm("x0") = (intptr_t)frame;
	register long x8 asm("x8") = __NR_rt_sigreturn;
	asm volatile("svc #0" :: "r"(x0), "r"(x8) : "memory", "cc");
}

static void vdso_save_fpsimd(ucontext_t *ctx)
{
	struct fpsimd_context *fpctx = NULL;
	uintptr_t addr;

	fpctx = (struct fpsimd_context *)(uintptr_t)(&ctx->uc_mcontext.__reserved);
	fpctx->head.magic = FPSIMD_MAGIC;
	fpctx->head.size = (unsigned int)sizeof(struct fpsimd_context);

	addr = (uintptr_t)(&fpctx->fpsr);
	asm volatile(
		"mov x9, %0\n"
		"mrs x10, fpsr\n"
		"mrs x11, fpcr\n"
		"stp w10, w11, [x9]\n"
		:: "r"(addr) : "x9", "x10", "x11", "memory"
	);

	addr = (uintptr_t)(&fpctx->vregs);
	asm volatile(
		"mov x9, %0\n"
		"stp q0, q1, [x9, #16 * 0]\n"
		"stp q2, q3, [x9, #16 * 2]\n"
		"stp q4, q5, [x9, #16 * 4]\n"
		"stp q6, q7, [x9, #16 * 6]\n"
		"stp q8, q9, [x9, #16 * 8]\n"
		"stp q10, q11, [x9, #16 * 10]\n"
		"stp q12, q13, [x9, #16 * 12]\n"
		"stp q14, q15, [x9, #16 * 14]\n"
		"stp q16, q17, [x9, #16 * 16]\n"
		"stp q18, q19, [x9, #16 * 18]\n"
		"stp q20, q21, [x9, #16 * 20]\n"
		"stp q22, q23, [x9, #16 * 22]\n"
		"stp q24, q25, [x9, #16 * 24]\n"
		"stp q26, q27, [x9, #16 * 26]\n"
		"stp q28, q29, [x9, #16 * 28]\n"
		"stp q30, q31, [x9, #16 * 30]\n"
		:: "r"(addr) : "x9", "memory"
	);
}

static void vdso_restore_fpsimd(const ucontext_t *ctx)
{
	const struct fpsimd_context *fpctx = NULL;
	uintptr_t addr;

	fpctx = (const struct fpsimd_context *)(uintptr_t)(&ctx->uc_mcontext.__reserved);

	addr = (uintptr_t)(&fpctx->fpsr);
	asm volatile(
		"mov x9, %0\n"
		"ldp w10, w11, [x9]\n"
		"msr fpsr, x10\n"
		"msr fpcr, x11\n"
		:: "r"(addr) : "x9", "x10", "x11"
	);

	addr = (uintptr_t)(&fpctx->vregs);
	asm volatile(
		"mov x9, %0\n"
		"ldp q0, q1, [x9, #16 * 0]\n"
		"ldp q2, q3, [x9, #16 * 2]\n"
		"ldp q4, q5, [x9, #16 * 4]\n"
		"ldp q6, q7, [x9, #16 * 6]\n"
		"ldp q8, q9, [x9, #16 * 8]\n"
		"ldp q10, q11, [x9, #16 * 10]\n"
		"ldp q12, q13, [x9, #16 * 12]\n"
		"ldp q14, q15, [x9, #16 * 14]\n"
		"ldp q16, q17, [x9, #16 * 16]\n"
		"ldp q18, q19, [x9, #16 * 18]\n"
		"ldp q20, q21, [x9, #16 * 20]\n"
		"ldp q22, q23, [x9, #16 * 22]\n"
		"ldp q24, q25, [x9, #16 * 24]\n"
		"ldp q26, q27, [x9, #16 * 26]\n"
		"ldp q28, q29, [x9, #16 * 28]\n"
		"ldp q30, q31, [x9, #16 * 30]\n"
		:: "r"(addr) : "x9"
	);
}

static void vdso_convert_from_mcontext_t(struct arch_uctx *dst, const mcontext_t *src)
{
	for (size_t i = 0; i < ARRAY_SIZE(src->regs); ++i) {
		__REG_NORMAL(&dst->regs, i) = src->regs[i];
	}
	__REG_SP(&dst->regs) = src->sp;
	__REG_PC(&dst->regs) = src->pc;
	__REG_SPSR(&dst->regs) = src->pstate;
}

static void vdso_convert_to_mcontext_t(mcontext_t *dst, const struct arch_uctx *src)
{
	for (size_t i = 0; i < ARRAY_SIZE(dst->regs); ++i) {
		dst->regs[i] = (unsigned long)__REG_NORMAL(&src->regs, i);
	}
	dst->sp = (unsigned long)__REG_SP(&src->regs);
	dst->pc = (unsigned long)__REG_PC(&src->regs);
	dst->pstate = (unsigned long)__REG_SPSR(&src->regs);
}
#elif defined(__arm__)
static inline void sigreturn(const struct sigframe *frame)
{
	register long r0 asm("r0") = (intptr_t)frame;
	register long r7 asm("r7") = __NR_rt_sigreturn;
	asm volatile("svc #0" :: "r"(r0), "r"(r7) : "memory", "cc");
}

#define VFP_MAGIC 0x56465001
#define VFP_NR_DREGS 32
struct vfp_sigframe {
	unsigned int magic;
	unsigned int size;
	unsigned long long fpregs[VFP_NR_DREGS];
	unsigned int fpscr;
	unsigned int fpexc;
	unsigned int fpinst;
	unsigned int fpinst2;
};

static void vdso_save_fpsimd(ucontext_t *ctx)
{
	struct vfp_sigframe *vfp = NULL;
	uintptr_t addr;

	vfp = (struct vfp_sigframe *)(uintptr_t)(&ctx->uc_regspace);
	vfp->magic = VFP_MAGIC;
	vfp->size = sizeof(struct vfp_sigframe);

	addr = (uintptr_t)(&vfp->fpregs);
	asm volatile(
		".fpu vfpv3\n"
		"mov r1, %0\n"
		"vstmia r1!, {d0-d15}\n"
		"vstmia r1!, {d16-d31}\n"
		"vmrs r0, fpscr\n"
		"str r0, [r1]\n"
		/* Clear the length and stride bits for function entry. */
		"bic r0, r0, #0x370000\n"
		"vmsr fpscr, r0\n"
		:: "r"(addr) : "r0", "r1", "memory"
	);
}

static void vdso_restore_fpsimd(const ucontext_t *ctx)
{
	const struct vfp_sigframe *vfp = NULL;
	uintptr_t addr;

	vfp = (const struct vfp_sigframe *)(uintptr_t)(&ctx->uc_regspace);
	addr = (uintptr_t)(&vfp->fpregs);
	asm volatile(
		".fpu vfpv3\n"
		"mov r1, %0\n"
		"vldmia r1!, {d0-d15}\n"
		"vldmia r1!, {d16-d31}\n"
		"ldr r0, [r1]\n"
		"vmsr fpscr, r0\n"
		:: "r"(addr) : "r0", "r1"
	);
}

#define NR_REGS 17

static void vdso_convert_from_mcontext_t(struct arch_uctx *dst, const mcontext_t *src)
{
	const unsigned long *r = &src->arm_r0;
	for (int i = 0; i < NR_REGS; ++i) {
		__REG_NORMAL(&dst->regs, i) = (__u32)r[i];
	}
}

static void vdso_convert_to_mcontext_t(mcontext_t *dst, const struct arch_uctx *src)
{
	unsigned long *r = &dst->arm_r0;
	for (int i = 0; i < NR_REGS; ++i) {
		r[i] = __REG_NORMAL(&src->regs, i);
	}
}
#else
#error "Arch not supported"
#endif

/* Keep in sync with __siginfo_k_to_u */
static void vdso_convert_to_siginfo_t(siginfo_t *dst, const struct __siginfo *src)
{
	dst->si_signo = (int)src->signum;
	dst->si_errno = (int)src->si_errno;
	dst->si_code = (int)src->si_code;
	clear_mem((int *)dst->__pad, sizeof(dst->__pad));

	if (src->si_type == __SI_RT) {
		dst->si_value.sival_ptr = (void *)(uintptr_t)src->sig_value.sival_ptr;
	} else if (src->si_type == __SI_CHLD) {
		dst->si_pid = src->__sigchld.pid;
		dst->si_uid = src->__sigchld.uid;
		dst->si_status = src->__sigchld.status;
	} else if (src->si_type == __SI_FAULT) {
		dst->si_addr = (void *)(uintptr_t)src->__fault.fault_addr;
	} else if (src->si_type == __SI_KILL) {
		dst->si_pid = src->__sigkill.pid;
		dst->si_uid = src->__sigkill.uid;
	} else if (src->si_type == __SI_SYS) {
		dst->si_call_addr = (void *)(uintptr_t)src->__sigsys.call_addr;
		dst->si_syscall = src->__sigsys.syscall;
		dst->si_arch = src->__sigsys.arch;
	} else {
		/* do nothing */
	}
}

static void vdso_convert_to_ucontext_t(ucontext_t *dst, struct sigframe *src)
{
	clear_mem((int *)dst, sizeof(*dst));

#if (SIZE_MAX == UINT32_MAX) && (__BYTE_ORDER == __BIG_ENDIAN)
	unsigned long *ss = ptr_to_type(&src->sigprocmask, unsigned long *);
	dst->uc_sigmask.__bits[0] = ss[1];
	dst->uc_sigmask.__bits[1] = ss[0];
#else
	uint64_t *ss = ptr_to_type(&dst->uc_sigmask, uint64_t *);
	*ss = src->sigprocmask;
#endif

	vdso_convert_to_mcontext_t(&dst->uc_mcontext, &src->uctx);
	if (src->siginfo.si_type == __SI_FAULT) {
		dst->uc_mcontext.fault_address = (unsigned long)src->siginfo.__fault.fault_addr;
	}
}

static void vdso_convert_from_ucontext_t(struct sigframe *dst, ucontext_t *src)
{
#if (SIZE_MAX == UINT32_MAX) && (__BYTE_ORDER == __BIG_ENDIAN)
	unsigned long *ss = ptr_to_type(&dst->sigprocmask, unsigned long *);
	ss[0] = src->uc_sigmask.__bits[1];
	ss[1] = src->uc_sigmask.__bits[0];
#else
	uint64_t *ss = ptr_to_type(&src->uc_sigmask, uint64_t *);
	dst->sigprocmask = *ss;
#endif

	vdso_convert_from_mcontext_t(&dst->uctx, &src->uc_mcontext);
}

/* The layout of vdso_sigframe should be compatible with rt_sigframe. */
struct vdso_sigframe {
	siginfo_t info;
	ucontext_t ctx;
};

void vdso_sigentry(struct sigframe *frame)
{
	struct vdso_sigframe p;

	vdso_convert_to_siginfo_t(&p.info, &frame->siginfo);
	vdso_convert_to_ucontext_t(&p.ctx, frame);
	if ((frame->action.flags & UNI_SA_NOFPSIMD) == 0) {
		vdso_save_fpsimd(&p.ctx);
	}

#ifndef __HOST_UT__
	vdso_sigtramp1((uintptr_t)(&p), frame);
#else
	vdso_sigtramp2((uintptr_t)(&p), frame);
#endif
}

typedef void (*sig_handler_t)(int, siginfo_t *, ucontext_t *);

void __nocfi vdso_sigtramp2(uintptr_t arg, struct sigframe *frame)
{
	struct vdso_sigframe *p = (struct vdso_sigframe *)arg;
	uint64_t flags = frame->action.flags;
	void *handler = (void *)(uintptr_t)frame->action.handler;
	int signum = (int)frame->siginfo.signum;

	if ((flags & SA_SIGINFO) == 0) {
		void (*h)(int) = (void (*)(int))(uintptr_t)handler;

		h(signum);
	} else {
		sig_handler_t h = (sig_handler_t)(uintptr_t)handler;

		h(signum, &p->info, &p->ctx);

		vdso_convert_from_ucontext_t(frame, &p->ctx);
	}

	if ((flags & UNI_SA_NOFPSIMD) == 0) {
		vdso_restore_fpsimd(&p->ctx);
	}

#ifndef __HOST_UT__
	sigreturn(frame);

	__builtin_trap();
	__builtin_unreachable();
#endif
}
