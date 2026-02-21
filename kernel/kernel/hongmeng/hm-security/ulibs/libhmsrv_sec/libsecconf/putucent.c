/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of putting user_conf entry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 29 12:28:23 2020
 */
// libc
#include <stdio.h>

// hongmeng
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include "libsecconf/putucent.h"

int sec_mac_fput_userconf_entry(const struct sec_mac_user_conf *uc, FILE *f)
{
	return fprintf(f, "%s:%s\n", uc->name, uc->label) < 0 ? E_HM_POSIX_NOENT : E_HM_OK;
}
