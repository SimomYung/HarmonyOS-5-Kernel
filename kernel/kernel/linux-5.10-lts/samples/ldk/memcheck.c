// SPDX-License-Identifier: GPL-2.0
#include "memcheck.h"

#include <hongmeng/errno.h>
#include <libsysif/sysmgr/api.h>
#include <hmkernel/ipc/signal.h>
#include <devhost/log.h>

#define MEMCHECK_PID_INVALID		0xFFDEADFF
int liblinux_send_memcheck_signal(int pid, int signum, struct kernel_siginfo *info)
{
	int rc;
	struct __siginfo siginfo;
	siginfo_t *psiginfo = (siginfo_t *)info;

	mem_zero_s(siginfo);

	siginfo.signum = psiginfo->si_signo;
	siginfo.si_code = psiginfo->si_code;
	siginfo.si_type = __SI_FAULT;
	siginfo.__fault.fault_addr = ptr_to_u64(psiginfo->si_addr);

	rc = actvcall_hmcall_signal_memcheck(pid, siginfo, signum);
	if (rc == MEMCHECK_PID_INVALID) {
		dh_error("send signal failed, pid is invaild\n");
	} else if (rc < 0) {
		dh_error("send signal failed, rc=%s\n", hmstrerror(rc));
		rc = -hmerrno2posix(rc);
	}

	return rc;
}
