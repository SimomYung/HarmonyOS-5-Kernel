/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of process in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <udk/process.h>

#include <devhost/backend.h>
#include <devhost/security.h>

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
