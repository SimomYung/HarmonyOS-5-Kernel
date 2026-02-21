/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header of libdh user copy
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 16 22:41:07 2020
 */

#ifndef ULIBS_LIBDEVHOST_UACCESS_H
#define ULIBS_LIBDEVHOST_UACCESS_H

#ifndef __user
#define __user
#endif

/*
 * copy data from src of xact owner vspace to dst of current vspace.
 *
 * @param    dst  [I]	virtual address of current vspace
 * @param    src  [I]	virtual address of xact owner vspace
 * @param    n    [I]	copy size
 *
 * @return   E_HM_OK for success, hmerrno for errors.
 */
int libdh_copy_from_user(void *dst, const void __user *src, unsigned long n);

/*
 * copy data from src of current vspace to dst of xact owner vspace.
 *
 * @param    dst  [I]	virtual address of xact owner vspace
 * @param    src  [I]	virtual address of current vspace
 * @param    n    [I]	copy size
 *
 * @return   E_HM_OK for success, hmerrno for errors.
 */
int libdh_copy_to_user(void __user *dst, const void *src, unsigned long n);

/*
 * copy data from src of xact owner vspace to dst of xact owner vspace.
 *
 * @param    dst  [I]	virtual address of xact owner vspace
 * @param    src  [I]	virtual address of xact owner vspace
 * @param    n    [I]	copy size
 *
 * @return   E_HM_OK for success, hmerrno for errors.
 */
int libdh_copy_in_user(void __user *dst, const void __user *src, unsigned long n);

#endif /* ULIBS_LIBDEVHOST_UACCESS_H */
