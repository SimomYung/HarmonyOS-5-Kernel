/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/keyctl.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 23 14:13:03 2021
 */

#ifndef _LINUX_KEYCTL_H
#define _LINUX_KEYCTL_H

#define KEY_SPEC_THREAD_KEYRING		(-1)
#define KEY_SPEC_PROCESS_KEYRING	(-2)
#define KEY_SPEC_SESSION_KEYRING	(-3)
#define KEY_SPEC_USER_KEYRING		(-4)

#define KEYCTL_UPDATE			2
#define KEYCTL_REVOKE			3
#define KEYCTL_UNLINK			9
#define KEYCTL_READ			11
#define KEYCTL_SET_TIMEOUT		15
#define KEYCTL_RESTRICT_KEYRING		29

#endif
