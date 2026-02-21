/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 3 10:05:35 2020
 */
#include <sys/msg.h>
#include <errno.h>
#include <hmkernel/errno.h>
#include <libhmsrv_sys/hm_ipc_msg.h>
#include <libsysif/sysmgr/api.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libmem/utils.h>
#include <sys/syscall.h>
#include <hmasm/lsyscall.h>

#ifdef CONFIG_SYSVIPC
int hm_ipc_msgget(key_t key, int msgflg)
{
	long rc;

	rc = lsyscall_syscall2(__NR_msgget, (long)key, (long)msgflg);
	if (rc < 0) {
		hm_debug("hmcall_ipc_msgget failed, err: %s\n",
			 hmstrerror(rc));
	}

	return (int)rc;
}

int hm_ipc_msgsnd(int msqid, struct msgbuf *buf, size_t msgsz, int msgflg)
{
	long err = E_HM_OK;

	err = lsyscall_syscall4(__NR_msgsnd, (long)msqid, ptr_to_long(buf), (long)msgsz, (long)msgflg);
	if (err < 0) {
		hm_debug("hm_ipc_msgsnd failed, err: %s\n",
				hmstrerror(err));
	}
	return (int)err;
}

int hm_ipc_msgrcv(int msqid, struct msgbuf *buf, size_t msgsz, long msgtyp, int msgflg)
{
	long rc;

	rc = lsyscall_syscall5(__NR_msgrcv, (long)msqid, ptr_to_long(buf), (long)msgsz, msgtyp, (long)msgflg);
	if (rc < 0) {
		hm_debug("actvcall_hmcall_ipc_msgrcv failed, err: %s\n",
			 hmstrerror(rc));
	}

	return (int)rc;
}

int hm_ipc_msgctl(int msqid, int cmd, struct msqid_ds *buf)
{
	long err = E_HM_OK;

	err = lsyscall_syscall3(__NR_msgctl, (long)msqid, (long)cmd, ptr_to_long(buf));
	if (err < 0) {
		hm_debug("hm_ipc_msgctl failed, err: %s\n",
				hmstrerror(err));
	}
	return (int)err;
}
#else
int hm_ipc_msgget(key_t key, int msgflg)
{
	UNUSED(key, msgflg);

	return E_HM_NOSYS;
}

int hm_ipc_msgsnd(int msqid, struct msgbuf *buf, size_t msgsz, int msgflg)
{
	UNUSED(msqid, buf, msgsz, msgflg);

	return E_HM_NOSYS;
}

int hm_ipc_msgrcv(int msqid, struct msgbuf *buf, size_t msgsz, long msgtyp, int msgflg)
{
	UNUSED(msqid, buf, msgsz, msgtyp, msgflg);

	return E_HM_NOSYS;
}

int hm_ipc_msgctl(int msqid, int cmd, struct msqid_ds *buf)
{
	UNUSED(msqid, cmd, buf);

	return E_HM_NOSYS;
}
#endif
