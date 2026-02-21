/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Hguard notify interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 10:12:20 2020
 */
#ifndef ULIBS_LIBHGUARD_REQUEST_H
#define ULIBS_LIBHGUARD_REQUEST_H

#include <unistd.h>
#include <hmkernel/capability.h>   /* for rref_t */

struct bunch;

int hguard_process_exit(rref_t rref, pid_t pid, int status);

int hguard_service_start(const char *service);
int hguard_service_stop(const char *service);
int hguard_service_restart(const char *service);
int hguard_service_reload(const char *service);
int hguard_service_status(const char *service, struct bunch *bunch);
int hguard_service_dump_startup_time(const char *service, struct bunch *bunch);
int hguard_daemon_reload(void);
int hguard_reboot(int flag);

int hguard_deliver_start(void);
int hguard_service_socket_all_names_of(char *buf, int buf_size);
int hguard_service_state_of(const char *service_name, char *buf, int buf_size);
int hguard_service_memory_current_of(const char *service_name, char *buf, int buf_size);

#endif
