/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Definition of sysif server mqueue
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 19 14:00:34 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MQUEUE_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_MQUEUE_H

#include <sys/stat.h>
#include <mqueue.h>
#include "hm_stat.h"

#define MQUEUEMGR_NAME_MAX 32

#define HM_MQUEUE_NAME_MAX_LEN MQUEUEMGR_NAME_MAX
#define HM_MQUEUE_MAX_PRIO     32768
#define HM_MQUEUE_MAX_BYTES    819200

#define MQUEUE_MIN_MSG_NUM     1
#define MQUEUE_DFLT_MSG_NUM    10
#define MQUEUE_HARD_MSG_NUM    65536

#define MQUEUE_MIN_MSG_SIZE    128
#define MQUEUE_DFLT_MSG_SIZE   8192
#define MQUEUE_HARD_MSG_SIZE   16777216

#define MQUEUE_DFLT_MAX_NUM    256
#define MQUEUE_MAX_OPEN_NUM    1024

enum hm_mqueue_req_id {
	HM_MQUEUE_OPEN,
	HM_MQUEUE_SEND,
	HM_MQUEUE_RECV,
	HM_MQUEUE_CLOSE,
	HM_MQUEUE_INVALID,
};

int hm_mqueue_open(const char *name, int oflag, mode_t mode,
		   const struct mq_attr *attr);
int hm_mqueue_timedsend(mqd_t mqd, char *msg_ptr, size_t msg_len,
			unsigned int msg_prio, const struct timespec *abs_timeout);
int hm_mqueue_timedrecv(mqd_t mqd, const char *msg_ptr, size_t msg_len,
			unsigned int *msg_prio, const struct timespec *abs_timeout);
int hm_mqueue_getsetattr(mqd_t mqd, const struct mq_attr *new_attr,
			 struct mq_attr *old_attr);
int hm_mqueue_close(mqd_t mqd);
int hm_mqueue_unlink(const char *name);
int hm_mqueue_chmod(const char *name, mode_t mode);

#endif
