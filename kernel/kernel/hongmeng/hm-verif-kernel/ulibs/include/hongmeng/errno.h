/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of ulibs errno
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 30 17:34:28 2018
 */
#ifndef ULIBS_HONGMENG_ERRNO_H
#define ULIBS_HONGMENG_ERRNO_H

#include <hmkernel/errno.h>
#include <lib/errno.h>

extern int hmerrno2posix(int error);
extern int posix2hmerrno(int error);

/*
 * To avoid problems caused by EHMERROR being returned to POSIX
 * by using the hmerrno2posix function, define EHMMERROR as EINVAL(22).
 * Do not import the POSIX header file into kernel
 */
#ifndef EHMERROR
#define EHMERROR    22
#endif

/*
 * These error numbers are used for vsyscall restart and should be handled
 * within libvsyscall.
 * Use the same values as Linux for better compatibility.
 */
#define ERESTARTSYS 512
#define ERESTARTNOINTR 513
/* Restart if signal has no handler, e.g. SIGSTOP. */
#define ERESTARTNOHAND 514
/* No ioctl cmd */
#define ENOIOCTLCMD 515
/* Special handling is needed to restart, e.g. nanosleep. */
#define ERESTARTBLOCK 516

enum {
	__POS_ERRNO_E_HM_USER_START = __POS_ERRNO_E_HM_KERNEL_MAX,
#define __HM_EDEF(name, posix, desc)  __POS_ERRNO_##name,
#define HM_EDEF(...)    __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...)  __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_user.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF
};

/*
 * NOTE: Don't use __E_HM_XXX directly, use ERR_AS_ENUM(E_HM_XXX)
 * or ERR_AS_INT(E_HM_XXX).
 */
enum {
#define __HM_EDEF(name, posix, desc) __##name = __HM_MKERR(0, __POS_ERRNO_##name),
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_user.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF
};

#define __HM_EDEF(name, posix, desc) static const int name = (int)__##name;
#define HM_EDEF(...)   __HM_EDEF(__VA_ARGS__)
#define HM_EDEF_P(...) __HM_EDEF(__VA_ARGS__)
#include <hmkernel/__errno_user.h>
#undef HM_EDEF_P
#undef HM_EDEF
#undef __HM_EDEF
#endif
