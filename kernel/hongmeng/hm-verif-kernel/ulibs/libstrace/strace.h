/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Strace interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 16 11:50:11 2020
 */

#ifndef ULIBS_LIBSTRACE_STRACEE_H
#define ULIBS_LIBSTRACE_STRACEE_H

#include <libstrace/strace.h>
#include <libhmsrv_sys/hm_mqueue.h>
#include <hmasm/registers.h>

#define STRACE_IPC_SCNO 		7
#define STRACE_MSG_EXITED		0x1
#define STRACE_MSG_SYSCALL 		0x2
#define STRACE_MAX_MQ_NAME_LEN		64
#define STRACE_MAX_SYSCALL_NARGS	6
#define STRACE_MAX_MQUEUE_MSG		256

struct strace_ipc_msg {
	int type;
	long pid;
	int nargs;
	long ret;
	struct timespec enter_ts;
	struct timespec exit_ts;
	struct arch_regs regs;
};

#endif
