#ifndef ARM_SYSCALL_ARCH_H
#define ARM_SYSCALL_ARCH_H

#define __NEED_uintptr_t
#define __NEED_size_t
#include "hmsyscall.h"
#include <bits/alltypes.h>
#include <bits/syscall.h>
#include <stdbool.h>

#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) 0, __SYSCALL_LL_E((x))

extern size_t __sysinfo;

#ifdef UT
#define __asm_syscall(n, ...) ut_syscall(n, ##__VA_ARGS__)
int ut_syscall(long n, ...);
#else
#define __asm_syscall(...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(r0) : __VA_ARGS__ : "memory"); \
	return r0; \
	} while (0)
#endif

#define __ASM____R7__ __asm__("r7")
#define R7_OPERAND "r"(r7)

static inline long __syscall0(long n)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0");
	__asm_syscall(R7_OPERAND);
}

static inline long __syscall1(long n, long a)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	__asm_syscall(R7_OPERAND, "0"(r0));
}

static inline long __syscall2(long n, long a, long b)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	register long r1 __asm__("r1") = b;
	__asm_syscall(R7_OPERAND, "0"(r0), "r"(r1));
}

static inline long __syscall3(long n, long a, long b, long c)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	register long r1 __asm__("r1") = b;
	register long r2 __asm__("r2") = c;
	__asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2));
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	register long r1 __asm__("r1") = b;
	register long r2 __asm__("r2") = c;
	register long r3 __asm__("r3") = d;
	__asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3));
}

static inline long __syscall5(long n, long a, long b, long c, long d, long e)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	register long r1 __asm__("r1") = b;
	register long r2 __asm__("r2") = c;
	register long r3 __asm__("r3") = d;
	register long r4 __asm__("r4") = e;
	__asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4));
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	register long r1 __asm__("r1") = b;
	register long r2 __asm__("r2") = c;
	register long r3 __asm__("r3") = d;
	register long r4 __asm__("r4") = e;
	register long r5 __asm__("r5") = f;
	__asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5));
}

static inline long __syscall7(long n, long a, long b, long c, long d, long e, long f, long g)
{
	register long r7 __ASM____R7__ = n;
	register long r0 __asm__("r0") = a;
	register long r1 __asm__("r1") = b;
	register long r2 __asm__("r2") = c;
	register long r3 __asm__("r3") = d;
	register long r4 __asm__("r4") = e;
	register long r5 __asm__("r5") = f;
	register long r6 __asm__("r6") = g;
	__asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5), "r"(r6));
}

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__vdso_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6"

#define VDSO_GET_DATA_SYM "vdso_get_data"
#define VDSO_GET_DATA_VER "HongMeng_1.8.0"

#define SYSCALL_FADVISE_6_ARG

#define SYSCALL_IPC_BROKEN_MODE
/* flags undefined are not supported by now */
#undef SYSCALL_IPC_BROKEN_MODE
#undef SYSCALL_FADVISE_6_ARG

#define VDSO_SAFE_COPY "safe_copy"
#define VDSO_SAFE_COPY_ALIGN "safe_copy_align"
#define VDSO_SAFE_COPY_NOPF "safe_copy_nopf"
#define VDSO_SAFE_COPY_VERSION "HongMeng_1.8.0"

#endif
