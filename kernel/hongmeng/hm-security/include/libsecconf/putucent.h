/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Get user security conf interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 29 12:45:05 2020
 */

#ifndef ULIBS_LIBSECCONF_PUTUCENT_H
#define ULIBS_LIBSECCONF_PUTUCENT_H

#include <stdio.h>

#include "secconf.h"

int sec_mac_fput_userconf_entry(const struct sec_mac_user_conf *uc, FILE *f);

#endif
