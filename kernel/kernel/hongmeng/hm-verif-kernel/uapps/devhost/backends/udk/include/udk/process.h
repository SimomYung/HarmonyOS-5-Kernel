/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Process interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 20:34:09 2021
 */
#ifndef __UDK_PROCESS_H__
#define __UDK_PROCESS_H__

void udk_kill_process_group(int sid, int signum);
void udk_kill_process(int pid, int signum);

int udk_mysid(void);
int udk_getfgpgrp(int sid);
int udk_setfgpgrp(int sid, int pgrp);
int udk_current_pid(void);
int udk_current_pgid(void);
int udk_getsid(int pid);
int udk_getpgid(int pid);
int udk_session_set_ttyinfo(int sid, const char *name, unsigned int devno);

#endif /* __UDK_PROCESS_H__ */
