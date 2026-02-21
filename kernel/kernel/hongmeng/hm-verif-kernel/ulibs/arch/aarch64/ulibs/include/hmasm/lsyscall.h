/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lsyscall defination for arm64
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 07 10:25:48 2023
 */

#ifndef AARCH64_LSYSCALL_SYSCALL_H
#define AARCH64_LSYSCALL_SYSCALL_H

extern long arch_lsyscall_entry0(long __sysno, ...);
extern long arch_lsyscall_entry1(long __sysno, ...);
extern long arch_lsyscall_entry2(long __sysno, ...);
extern long arch_lsyscall_entry3(long __sysno, ...);
extern long arch_lsyscall_entry4(long __sysno, ...);
extern long arch_lsyscall_entry5(long __sysno, ...);
extern long arch_lsyscall_entry6(long __sysno, ...);
extern long arch_lsyscall_entry7(long __sysno, ...);

static inline long lsyscall_syscall0(long scno)
{
	return arch_lsyscall_entry0(scno);
}

static inline long lsyscall_syscall1(long scno, long a)
{
	return arch_lsyscall_entry1(scno, a);
}

static inline long lsyscall_syscall2(long scno, long a, long b)
{
	return arch_lsyscall_entry2(scno, a, b);
}

static inline long lsyscall_syscall3(long scno, long a, long b, long c)
{
	return arch_lsyscall_entry3(scno, a, b, c);
}

static inline long lsyscall_syscall4(long scno, long a, long b, long c, long d)
{
	return arch_lsyscall_entry4(scno, a, b, c, d);
}

static inline long lsyscall_syscall5(long scno, long a, long b, long c, long d,
				     long e)
{
	return arch_lsyscall_entry5(scno, a, b, c, d, e);
}

static inline long lsyscall_syscall6(long scno, long a, long b, long c, long d,
				     long e, long f)
{
	return arch_lsyscall_entry6(scno, a, b, c, d, e, f);
}

static inline long lsyscall_syscall7(long scno, long a, long b, long c, long d,
				    long e, long f, long g)
{
	return arch_lsyscall_entry7(scno, a, b, c, d, e, f, g);
}

#endif
