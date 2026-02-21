// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_utils.h for kernel sg
 * Create: 2022-07-23
 */

#ifndef _HW_KERNEL_SG_UTILS_H_
#define _HW_KERNEL_SG_UTILS_H_

#include <linux/buffer_head.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/ratelimit.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>

#define KSG_SUCCESS (0)
#define KSG_ERRCODE (1)
#define KSG_NO_PERMISSION (2)

#define KSG_LIST_INIT 1
#define KSG_LIST_UNINIT 0

#define SPLIT_STR ";"

#define KSG_NUMBER_DECIMAL 10
#define KSG_NUMBER_MAX_LEN 12
#define KSG_NUMBER_MAX_ALLOW 0xFFFFFFFFFFFF

#define KSG_INFO_MAXLEN   1024

#define KSG_ERROR "[Error]"
#define ksg_log_error(tag, fmt, args...) \
	pr_err_ratelimited("%s[%s][%s] " fmt "\n", KSG_ERROR, \
		tag, __func__, ##args)

#define KSG_WARNING "[Warning]"
#define ksg_log_warning(tag, fmt, args...) \
	pr_warn_ratelimited("%s[%s][%s] " fmt "\n", KSG_WARNING, \
		tag, __func__, ##args)

#define KSG_TRACE "[Trace]"
#define ksg_log_trace(tag, fmt, args...) \
	pr_info_ratelimited("%s[%s][%s] " fmt "\n", KSG_TRACE, \
		tag, __func__, ##args)

#ifdef CONFIG_HW_KERNEL_SG_ENG_DEBUG

#define KSG_DEBUG "[Debug]"
#define ksg_log_debug(tag, fmt, args...) \
	printk_ratelimited("%s[%s][%s] " fmt "\n", KSG_DEBUG, \
		tag, __func__, ##args)

#else
#define ksg_log_debug(tag, fmt, args...) no_printk(fmt, ##args)
#endif

#endif /* _HW_KERNEL_SG_UTILS_H_ */
