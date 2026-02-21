/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Klog level definition
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 14:36:23 2019
 */

#ifndef UAPI_HMKERNEL_KLOG_LEVEL_H
#define UAPI_HMKERNEL_KLOG_LEVEL_H

#define KLOG_PREFIX_BUF_SIZE	128
#define KLOG_CONTENT_BUF_SIZE	512

#define KLOG_LEVEL_UNSPECIFIED UINT_MAX
#define KLOG_LEVEL_NR 6

enum klog_level {
	KLOG_DEBUG,
	KLOG_INFO,
	KLOG_WARN,
	KLOG_ERROR,
	KLOG_PANIC,
	KLOG_NOLOCK,
	KLOG_INVALID,
};

static inline enum klog_level enum_klog_level_from_uint(unsigned int uint_level)
{
	enum klog_level ret;
	switch (uint_level) {
	case (unsigned int)KLOG_DEBUG:
		ret = KLOG_DEBUG;
		break;
	case (unsigned int)KLOG_INFO:
		ret = KLOG_INFO;
		break;
	case (unsigned int)KLOG_WARN:
		ret = KLOG_WARN;
		break;
	case (unsigned int)KLOG_ERROR:
		ret = KLOG_ERROR;
		break;
	case (unsigned int)KLOG_PANIC:
		ret = KLOG_PANIC;
		break;
	case (unsigned int)KLOG_NOLOCK:
		/* fallthrough */
	default:
		ret = KLOG_PANIC;
		break;
	}
	return ret;
}

#endif
