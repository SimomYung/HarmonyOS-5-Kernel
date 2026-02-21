/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Strace interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 16 11:50:11 2020
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <securec.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_timer.h>
#ifdef __HOST_LLT__
#include <../../sysmgr-llt/test_strace_stub.h>
#else
#include <libhmsrv_sys/hm_thread.h>
#endif
#include "strace.h"

bool __is_strace_enabled = false;
static long strace_tracer_pid = 0;
static mqd_t strace_tracee_mqid;
static char strace_mq[STRACE_MAX_MQ_NAME_LEN] = {0};

static int set_tracer_pid(long pid)
{
	int rc;

	if (pid < 0) {
		rc = E_HM_INVAL;
	} else {
		strace_tracer_pid = pid;
		rc = E_HM_OK;
	}

	return rc;
}

static int init_tracee_mq(long pid)
{
	int rc;
	struct mq_attr attr;
	mode_t mode = (unsigned)S_IRUSR | (unsigned)S_IWUSR | (unsigned)S_IRGRP |
		      (unsigned)S_IWGRP | (unsigned)S_IROTH | (unsigned)S_IWOTH;

	rc = set_tracer_pid(pid);
	if (rc == E_HM_OK) {
		attr.mq_flags   = 0;
		attr.mq_maxmsg  = STRACE_MAX_MQUEUE_MSG;
		attr.mq_curmsgs = 0;
		attr.mq_msgsize = (long)sizeof(struct strace_ipc_msg);
		/*
		 * in attach way, strace_mq should be initialized by tracee
		 * the first time tracee invokes a syscall.
		 */
		rc = snprintf_s(strace_mq, STRACE_MAX_MQ_NAME_LEN,
				STRACE_MAX_MQ_NAME_LEN - 1, "strace%ld", pid);
		if (rc < 0) {
			rc = E_HM_INVAL;
		} else {
			strace_tracee_mqid = hm_mqueue_open(strace_mq, O_RDWR,
							    mode, &attr);
			if (strace_tracee_mqid < 0) {
				rc = (int)strace_tracee_mqid;
			} else {
				rc = E_HM_OK;
			}
		}
	}

	return rc;
}

static int close_tracee_mq(void)
{
	int rc = E_HM_OK;
	/* mqueue neet to be unlinked by tracer process */
	if (strace_tracee_mqid >= 0) {
		rc = hm_mqueue_close(strace_tracee_mqid);
		strace_tracee_mqid = (mqd_t)-1;
	}
	return rc;
}

static int clock_get_time(struct timespec *ts)
{
#ifdef __HOST_LLT__
	int err;
	uint64_t tock = 0ULL;
	struct timespec time_copy;

	err = sysfast_tock_read(CLOCK_MONOTONIC, ptr_to_type(&tock, __u64 *));
	if (err >= 0) {
		err = tocks_to_timespec(tock, &time_copy);
	}
	if (err == E_HM_OK) {
		if (safe_copy((void *)ts, (void *)&time_copy,
		    sizeof(struct timespec)) != 0) {
			err = E_HM_POSIX_FAULT;
		}
	}

	return err;
#else
	return hm_clock_gettime(CLOCK_MONOTONIC, ts);
#endif
}

static int send_syscall_msg(struct strace_ipc_msg *msg)
{
	int rc = E_HM_OK;
	bool need_retry = false;

	if ((strace_tracee_mqid < 0) || (msg == NULL)) {
		rc = E_HM_INVAL;
	} else {
		rc =  hm_mqueue_timedsend(strace_tracee_mqid,
					  (char * restrict)msg,
					  sizeof(*msg), 0xf, NULL);
		if (rc < 0 && strace_tracer_pid > 0) {
			/* retry send msg */
			need_retry = true;
		}
	}

	if (need_retry) {
		rc = init_tracee_mq(strace_tracer_pid);
		if (rc >= 0) {
			rc = hm_mqueue_timedsend(strace_tracee_mqid,
						 (char * restrict)msg,
						 sizeof(*msg), 0xf, NULL);
		}
	}
	return rc;
}

int hm_strace_on(long pid)
{
	/*
	 * No need to add multi thread lock for strace_trace_flag.
	 * It is only set by stracer process.
	 */
	__is_strace_enabled = true;

	int rc = init_tracee_mq(pid);
	return rc;
}

int hm_strace_off(void)
{
	__is_strace_enabled = false;

	int rc = close_tracee_mq();
	return rc;
}

void hm_strace_entry(enum strace_enter_state state, long scno, long ret_val,
		  struct strace_entry_info *info, va_list va)
{
	int rc;

	if (info != NULL && state == STRACE_ENTERING) {
		(void)clock_get_time(&(info->ts));
	}
	if (info != NULL && state == STRACE_EXITING) {
		int i;
		va_list strace_va;
		va_copy(strace_va, va);
		struct strace_ipc_msg syscall_msg;
		syscall_msg.pid = hm_thread_gettid();
		syscall_msg.nargs = STRACE_MAX_SYSCALL_NARGS;
		syscall_msg.type = STRACE_MSG_SYSCALL;

		syscall_msg.enter_ts.tv_sec = info->ts.tv_sec;
		syscall_msg.enter_ts.tv_nsec = info->ts.tv_nsec;
		rc = clock_get_time(&(syscall_msg.exit_ts));
		if (rc >= 0) {
			syscall_msg.ret = ret_val;
			syscall_msg.regs.__regs[STRACE_IPC_SCNO] = (unsigned long)scno;
			for (i = 0; i < STRACE_MAX_SYSCALL_NARGS; i++) {
				syscall_msg.regs.__regs[i] = (unsigned long)va_arg(strace_va, unsigned long);
			}
			(void)send_syscall_msg(&syscall_msg);
		}
		va_end(strace_va);
	}

	return;
}
