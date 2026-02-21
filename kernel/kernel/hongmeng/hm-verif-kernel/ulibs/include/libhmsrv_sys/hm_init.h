/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Interface of init actions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 09 09:39:44 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_INIT_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_INIT_H

#include <sys/types.h>

/*
 * The flag bits indicate the additional actions that need to be performed in normal reboot shutdown process.
 */
#define REBOOT_FLAG_TAKE_SNAPSHOT	1U /* bit[0] */
#define REBOOT_FLAG_EMERGENCY		(1U << 1) /* bit[1] */

#define test_reboot_flag_snapshot(flag)			\
	(((flag) & REBOOT_FLAG_TAKE_SNAPSHOT) != 0U)

#define test_reboot_flag_emergency(flag)		\
	(((flag) & REBOOT_FLAG_EMERGENCY) != 0U)

int hminit_notify_service_ready(void);
int hminit_srvmgr_set_dh_pid(pid_t dh_pid);
int hminit_notify_reboot(const char *reboot_reason, unsigned int reboot_flag);
int hminit_notify_shutdown(void);
int hminit_notify_softreboot(void);
int hminit_notify_kexec(void);

#endif
