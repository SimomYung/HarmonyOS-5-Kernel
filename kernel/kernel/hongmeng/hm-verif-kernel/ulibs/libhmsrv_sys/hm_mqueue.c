/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Implementation of sysif server mqueue
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 19 14:00:34 2018
 */
#include <errno.h>
#include <hmkernel/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <malloc.h>
#include <libhmsrv_sys/hm_mqueue.h>
#include <libmem/utils.h>
#include <libstrict/strict.h>
#include <hmasm/lsyscall.h>
#include <syscall.h>

int hm_mqueue_open(const char *name, int oflag, mode_t mode, const struct mq_attr *attr)
{
	return (int)lsyscall_syscall4(__NR_mq_open, (long)ptr_to_ulong(name),
							 (long)oflag, (long)mode, (long)ptr_to_ulong(attr));
}

int hm_mqueue_timedsend(mqd_t mqd, char *msg_ptr, size_t msg_len,
			unsigned int msg_prio, const struct timespec *abs_timeout)
{
	return (int)lsyscall_syscall5(__NR_mq_timedsend, (long)mqd, (long)ptr_to_ulong(msg_ptr),
							 (long)msg_len, (long)msg_prio, (long)ptr_to_ulong(abs_timeout));
}

int hm_mqueue_timedrecv(mqd_t mqd, const char *msg_ptr, size_t msg_len,
			unsigned int *msg_prio, const struct timespec *abs_timeout)
{
	return (int)lsyscall_syscall5(__NR_mq_timedreceive, (long)mqd, (long)ptr_to_ulong(msg_ptr),
			 (long)msg_len, (long)ptr_to_ulong(msg_prio), (long)ptr_to_ulong(abs_timeout));
}

int hm_mqueue_getsetattr(mqd_t mqd, const struct mq_attr *new_attr,
			 struct mq_attr *old_attr)
{
	return (int)lsyscall_syscall3(__NR_mq_getsetattr, (long)mqd,
				(long)ptr_to_ulong(new_attr), (long)ptr_to_ulong(old_attr));
}

int hm_mqueue_close(mqd_t mqd)
{
	return (int)lsyscall_syscall1(__NR_mq_close, (long)mqd);
}

int hm_mqueue_unlink(const char *name)
{
	return (int)lsyscall_syscall1(__NR_mq_unlink, (long)ptr_to_ulong(name));
}

/*
 * Deal with client's mqueue chmod request
 *
 * @params name [I] Mqueue name acquired
 *
 * @params mode [I] Provide the new mode of mqueue for changeing dac rights
 *
 * @return Returns E_HM_OK on success. On any error, returns an errno.
 */
int hm_mqueue_chmod(const char *name, mode_t mode)
{
	return actvcall_hmcall_mqueue_chmod(name, mode);
}
