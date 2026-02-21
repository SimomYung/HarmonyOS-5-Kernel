/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Get user security conf interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 29 12:45:05 2020
 */

#ifndef ULIBS_LIBSECCONF_GETUCENT_H
#define ULIBS_LIBSECCONF_GETUCENT_H

#include <stdio.h>

#include "secconf.h"

void sec_mac_set_userconf_entry(void);
void sec_mac_end_userconf_entry(void);
struct sec_mac_user_conf *sec_mac_fget_userconf_entry(FILE *f);
struct sec_mac_user_conf *sec_mac_get_userconf_entry(void);
int sec_mac_get_userconf_by_name(const char *name,
				 struct sec_mac_user_conf *uc,
				 char *buf,
				 size_t size,
				 struct sec_mac_user_conf **res);

#endif
