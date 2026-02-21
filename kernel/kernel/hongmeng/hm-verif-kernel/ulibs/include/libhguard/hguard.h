/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Hguard notify interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 10:12:20 2020
 */
#ifndef ULIBS_LIBHGUARD_HGUARD_H
#define ULIBS_LIBHGUARD_HGUARD_H

#define NOTIFY_HEARTBEAT 0U
#define NOTIFY_READY     1U

int hguard_notify(unsigned int type);

struct hguard_exit_data {
	unsigned int pid;
	unsigned int wstatus;
};

int hguard_subscribe_record_exit(void);
int hguard_subscribe_register(int signum);
int hguard_subscribe_clear(int pid);
int hguard_subscribe_fetchall(const struct hguard_exit_data *buf, int buf_size);

int hguard_procfs_get_config(int config_id, char *config_buf, int config_size);
int hguard_procfs_set_config(int config_id, char *config_buf, int config_size);
int hguard_procfs_stop_watchdog_for_reboot(unsigned int timeout_sec);

#endif
