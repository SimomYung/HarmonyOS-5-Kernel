/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Security check of udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 06 08:00:37 2021
 */
#ifndef __UDK_SECURITY_H__
#define __UDK_SECURITY_H__

#include <stdbool.h>

int udk_ability_of_tty(void);
int udk_ability_of_serial_toggle(void);

int udk_capability_of_admin(void);
int udk_capability_of_tty_config(void);

int udk_caller_has_ability(int ability);
int udk_caller_has_capability(int cap);
int udk_caller_has_ab_and_cap(int ability, int cap);

bool udk_capable(int cap);
#endif /* __UDK_SECURITY_H__ */
