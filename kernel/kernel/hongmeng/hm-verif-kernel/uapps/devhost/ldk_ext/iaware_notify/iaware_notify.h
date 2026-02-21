/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of iaware notify interfacesa
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 28 12:15:54 2023
 */

#ifndef DEVHOST_LDK_EXT_IAWARE_NOTIFY_H
#define DEVHOST_LDK_EXT_IAWARE_NOTIFY_H

#include <libhmsrv_sys/hm_event.h>

extern struct devhost_iaware_notify_ops *iaware_notify_ops;
int devhost_iaware_notify_procfs_init(void *arg);
int set_default_whitelist(void);
int devhost_regist_handler_to_iaware(void);
uref_t dh_iaware_notify_uref_of(void);
#endif
