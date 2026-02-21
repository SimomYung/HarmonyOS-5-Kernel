/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Read procfs node by pid
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 18 15:41:12 2021
 */
#ifndef ULIBS_LIBSEC_UTILITY_H
#define ULIBS_LIBSEC_UTILITY_H

#include <unistd.h> /* for pid_t */
#include <libsec/chk_cred.h>

int sec_log_read_pid_node(const struct sec_chk_cred *cred, pid_t *pid,
			  const char *node_name, char **data);
int sec_log_read_pid_exe(pid_t pid, const char *node_name, char **data);

#endif /* ifndef ULIBS_LIBSEC_UTILITY_H */
