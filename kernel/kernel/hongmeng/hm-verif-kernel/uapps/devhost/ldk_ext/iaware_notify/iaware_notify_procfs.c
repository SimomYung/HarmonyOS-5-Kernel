/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: procfs for iaware notify
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 23 17:04:22 2023
 */

#include <libcrt/hmcrt.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libdevhost/procfs.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include "iaware_notify.h"
#include "iaware_notify_api.h"
#include <devhost/file.h>
#include <devhost/memory.h>
#include <devhost/plugin.h>
#include <devhost/log.h>
#include "iaware_notify_policy.h"

#define MAX_STR_LEN 256

static int parse_iaware_notify_policy(char *buffer, unsigned long size)
{
	int ret = E_HM_OK;

	if (size > MAX_STR_LEN) {
		ret = E_HM_INVAL;
	} else if (strstr(buffer, "key")) {
		ret = set_thread_name_whitelist(buffer);
	} else {
		hm_debug("iaware notify policy format mismatch\n");
		ret = E_HM_INVAL;
	}

	return ret;
}

static int iaware_notify_policy_write(const struct kernfs_entry *entry,
					 const struct libdh_kernfs_rw_args *args,
					 unsigned long *wsize)
{
	UNUSED(entry);
	int ret = 0;
	char *buf = NULL;

	if (args == NULL || wsize == NULL) {
		return E_HM_INVAL;
	}

	buf = malloc((size_t)args->size);
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	ret = devhost_pal_copy_from_user(buf, ulong_to_ptr(args->ubuf, void), args->size);
	if (ret < 0) {
		hm_error("get data from xactv failed, %s\n", hmstrerror(ret));
		free(buf);
		return E_HM_POSIX_FAULT;
	}

	buf[args->size - 1] = '\0';
	ret = parse_iaware_notify_policy(buf, args->size);
	*wsize = args->size;
	free(buf);

	if (ret != E_HM_OK) {
		hm_error("failed to set the value of sched_aux_key,%s\n",hmstrerror(ret));
	}

	return ret;
}

static struct libdh_kernfs_ops iaware_notify_policy_fops __read_mostly = {
	.read = NULL,
	.write = iaware_notify_policy_write,
	.poll = NULL,
};

int devhost_iaware_notify_procfs_init(void *arg)
{
	UNUSED(arg);
	int ret = 0;
	struct libdh_procfs_args args = {
		.ops = NULL,
		.ctx = NULL,
		.mode = 0644,
	};

	ret = libdh_procfs_create_file("iaware_notify_policy", &iaware_notify_policy_fops, &args, NULL);
	if (ret < 0) {
		hm_error("dh iaware notify procfs, init vmallocinfo failed\n");
	}

	return ret;
}
