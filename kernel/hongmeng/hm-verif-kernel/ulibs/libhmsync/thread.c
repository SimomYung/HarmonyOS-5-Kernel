/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Basic interfaces of all thread models
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */

#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>
#include <libhmsync/thread.h>
#include <libsysif/sysmgr/api.h>
#include <hmasm/lsyscall.h>

void hm_thread_exit(int exitcode)
{
	(void)lsyscall_syscall1(__NR_exit, (long)exitcode);
}

void thread_setup_config(struct hm_clone_req *req, const void *stack_vaddr,
			 const void *tls_vaddr, const void *entry,
			 const void *args, unsigned int clone_flags,
			 const void *ptid, const void *ctid,
			 unsigned int hm_flags)
{
	req->args = (unsigned long long)(uintptr_t)args;
	req->entry = (unsigned long long)(uintptr_t)entry;
	req->child_stack = (unsigned long long)(uintptr_t)stack_vaddr;
	req->newtls = (unsigned long long)(uintptr_t)tls_vaddr;
	req->ctid = (unsigned long long)(uintptr_t)ctid;
	req->ptid = (unsigned long long)(uintptr_t)ptid;
	req->clone_flags = clone_flags;
	req->hm_flags = hm_flags;
}

int hm_thread_set_lt_group(int tid, int group)
{
	return actvcall_hmcall_thread_set_lt_group(tid, group);
}

int hm_thread_set_lt_boost(int tid, int boost)
{
	return actvcall_hmcall_thread_set_lt_boost(tid, boost);
}
