/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of fd
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 17:03:05 2019
 */

#ifndef LIBHMSRV_FS_FD_H
#define LIBHMSRV_FS_FD_H

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/types.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_scopedptr.h>
#include <fs_mutex.h>
#include <fs_server.h>
#include <vfs.h>

enum {
	NORMAL_FD,
	DEVICE_FD_CHR,
	DEVICE_FD_BLK,
	FIFO_FD,
	EVENT_FD,
	BINDER_FD,
	VPIPE_FD,
	EPOLL_FD_SINK,
	SEHARMONY_NO_DEVICE_FD,
	PERF_FD,
	KCOV_FD,
	IO_URING_FD,
	DEVICE_FD_CHR_SELF_VA,
	DEV_ZERO_FD,
	UNKNOWN_FD,
};

#define calc_offset(ptr, base)	ptr_to_ulong((ptr) - (base))

#endif	/* !LIBHMSRV_FS_FD_H */
