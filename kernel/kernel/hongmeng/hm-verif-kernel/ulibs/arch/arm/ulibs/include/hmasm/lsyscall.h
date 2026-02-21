/*
  * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
  * Description: lsyscall for arm32
  * Author: Huawei OS Kernel Lab
  * Create: Fri Apr 07 10:23:19 2023
  */
#ifndef ARM_LSYSCALL_SYSCALL_H
#define ARM_LSYSCALL_SYSCALL_H

extern long arch_lsyscall_entry(long __sysno, ...);

static inline long lsyscall_syscall0(long scno)
{
	return arch_lsyscall_entry(scno);
}

static inline long lsyscall_syscall1(long scno, long a)
{
	return arch_lsyscall_entry(scno, a);
}

static inline long lsyscall_syscall2(long scno, long a, long b)
{
	return arch_lsyscall_entry(scno, a, b);
}

static inline long lsyscall_syscall3(long scno, long a, long b, long c)
{
	return arch_lsyscall_entry(scno, a, b, c);
}

static inline long lsyscall_syscall4(long scno, long a, long b, long c, long d)
{
	return arch_lsyscall_entry(scno, a, b, c, d);
}

static inline long lsyscall_syscall5(long scno, long a, long b, long c, long d,
				     long e)
{
	return arch_lsyscall_entry(scno, a, b, c, d, e);
}

static inline long lsyscall_syscall6(long scno, long a, long b, long c, long d,
				     long e, long f)
{
	return arch_lsyscall_entry(scno, a, b, c, d, e, f);
}

static inline long lsyscall_syscall7(long scno, long a, long b, long c, long d, long e, long f, long g)
{
	return arch_lsyscall_entry(scno, a, b, c, d, e, f, g);
}

#endif
