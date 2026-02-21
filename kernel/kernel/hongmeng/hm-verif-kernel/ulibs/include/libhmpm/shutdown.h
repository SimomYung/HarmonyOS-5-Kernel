/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for hongmeng system shutdown native api
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 11:45:32 2019
 */

#ifndef ULIBS_LIBHMPM_SHUTDOWN_H
#define ULIBS_LIBHMPM_SHUTDOWN_H

#define REBOOT_CMD_LEN	92
struct reboot_cmd {
	unsigned int reboot_reason;
	char string[REBOOT_CMD_LEN];
};

/* Ordering of the states must not be changed. */
enum hm_system_state {
	HM_SYSTEM_STATE_BOOTING,
	HM_SYSTEM_STATE_RUNNING,
	HM_SYSTEM_STATE_POWER_OFF,
	HM_SYSTEM_STATE_RESTART,
	HM_SYSTEM_STATE_SUSPEND,
};

int hm_shut_down(void);
int hm_shutdown_timedout(long timeout_ms);
int hm_softreboot(void);
int hm_kexec_timedout(long timeout_ms);
int hm_reboot(const char *cmd);
int hm_reboot_timedout(const char *cmd, unsigned int reboot_reason, long timeout_ms);
int hm_system_state(enum hm_system_state *state);

#endif
