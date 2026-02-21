/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of process in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <limits.h>
#include <udk/process.h>
#include <udk/iolib.h>
#include <udk/log.h>

#include <devhost/backend.h>
#include <devhost/security.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>

#include <intapi/process/proc_info_status.h>

void udk_kill_process_group(int sid, int signum)
{
	devhost_kill_process_group(sid, signum);
}

void udk_kill_process(int pid, int signum)
{
	devhost_kill_process(pid, signum);
}

int udk_mysid(void)
{
	return devhost_mysid();
}

int udk_getfgpgrp(int sid)
{
	return devhost_getfgpgrp(sid);
}

int udk_setfgpgrp(int sid, int pgrp)
{
	return devhost_setfgpgrp(sid, pgrp);
}

int udk_current_pid(void)
{
	return devhost_current_pid();
}

int udk_current_pgid(void)
{
	return devhost_current_pgid();
}

int udk_getsid(int pid)
{
	return devhost_getsid(pid);
}

int udk_getpgid(int pid)
{
	return devhost_getpgid(pid);
}

int udk_session_set_ttyinfo(int sid, const char *name, unsigned int devno)
{
	return devhost_session_set_ttyinfo(sid, name, devno);
}

int udk_get_caller_process_exe_path(char *buf, unsigned int buf_size)
{
	char *tmp_path = NULL;
	int ret, err, len;

	tmp_path = udk_malloc(PATH_MAX);
	if (tmp_path == NULL) {
		udk_error("alloc tmp_path failed\n");
		return E_HM_NOMEM;
	}

	ret = process_current_exe_path(tmp_path);
	if (ret > 0) {
		len = PATH_MAX - ret;
		if (buf_size >= (size_t)(unsigned)len) {
			err = memcpy_s(buf, buf_size, tmp_path + (size_t)ret, (size_t)(len));
			if (err != 0) {
				udk_error("memcpy_s failed: %d\n", err);
				ret = E_HM_POSIX_FAULT;
			} else {
				ret = E_HM_OK;
			}
		} else {
			ret = E_HM_RANGE;
		}
	}

	udk_free(tmp_path);
	return ret;
}

unsigned int udk_get_caller_process_uid(void)
{
	return (unsigned int)process_current_uid();
}
