/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022.
 * Description: Interface of device/driver management utils
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 18 11:33:29 2022
 */
#ifndef __DMCTL_H__
#define __DMCTL_H__

#include <libsysif/devmgr/api.h>

int dmctl_main(int argc, char* argv[]);
int dmctl_insert_mod(char *argv[], int argc, const char *name,
		     struct devmgr_mgmt_target target);
int dmctl_unload_mod(const char *name, struct devmgr_mgmt_target target);

#endif
