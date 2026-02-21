/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of kstate
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 21:18:58 CST 2023
 */

#ifndef DEVHOST_LDK_EXT_KSTATE_H
#define DEVHOST_LDK_EXT_KSTATE_H

#include "kstate_api.h"

int devhost_kstate_binderinfo_notify(int calling_pid, int called_pid);
int devhost_kstate_killinfo_notify(int pid, int seg);
int devhost_kstate_freeze_binder_info_notify(struct kstate_binder_info *info);
int devhost_kstate_request_anco_thaw(char *reason, unsigned int size);

#endif /* DEVHOST_LDK_EXT_KSTATE_H */
