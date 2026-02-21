/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: linux syscall number
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 23 09:49:43 2023
 */

#ifndef UAPI_HMKERNEL_LSCNO_H
#define UAPI_HMKERNEL_LSCNO_H

/*
 * To distinguish, DEFINE lscno(except compatno) with prefix `__` in the kernel,
 * and CHECK the lscno(except compatno) on the user side.
 */
#ifdef __KERNEL__
#define LSCNO_DEF(lcallno, nr) ____NR_##lcallno = (nr),
#define LSCNO_COMPAT_DEF(lcallno, nr)
enum {
#include <hmasm/lscno_def.h>
};
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF

#define LSCNO_DEF(lcallno, nr) __SYS_##lcallno = (nr),
#define LSCNO_COMPAT_DEF(lcallno, nr)
enum {
#include <hmasm/lscno_def.h>
};
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF
#else
#include <sys/syscall.h>
#include <hmsyscall.h>
#include <hongmeng/hm_lsyscall.h>
#define LSCNO_DEF(lcallno, nr) raw_static_assert(__NR_##lcallno == (nr), lcallno_check_failed);
#define LSCNO_COMPAT_DEF(lcallno, nr)
#include <hmasm/lscno_def.h>
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF
#endif /* __KERNEL__ */

#ifdef CONFIG_COMPAT
#define LSCNO_DEF(lcallno, nr)
#define LSCNO_COMPAT_DEF(lcallno, nr) ____NR_##lcallno##_compat = (nr),
enum {
#include <hmasm/lscno_def.h>
};
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF

#define LSCNO_DEF(lcallno, nr)
#define LSCNO_COMPAT_DEF(lcallno, nr) ____NR_##lcallno##_COMPAT = (nr),
enum {
#include <hmasm/lscno_def.h>
};
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF

#define LSCNO_DEF(lcallno, nr)
#define LSCNO_COMPAT_DEF(lcallno, nr) __SYS_##lcallno##_compat = (nr),
enum {
#include <hmasm/lscno_def.h>
};
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF

#define LSCNO_DEF(lcallno, nr)
#define LSCNO_COMPAT_DEF(lcallno, nr) __SYS_##lcallno##_COMPAT = (nr),
enum {
#include <hmasm/lscno_def.h>
};
#undef LSCNO_COMPAT_DEF
#undef LSCNO_DEF

#define __COMPAT_OF(lcallno) __##lcallno##_compat
#else /* CONFIG_COMPAT */
#define __COMPAT_OF(lcallno) (~0U)
#endif /* CONFIG_COMPAT */
#endif
