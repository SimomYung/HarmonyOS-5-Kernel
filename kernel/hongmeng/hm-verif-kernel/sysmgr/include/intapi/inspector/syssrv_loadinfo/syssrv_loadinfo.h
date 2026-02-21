/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interface of syssrv_loadinfo
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 13 16:24:46 2024
 */

#ifndef SYSYMGR_INSPECTOR_SYSSRV_LOADINFO_H
#define SYSYMGR_INSPECTOR_SYSSRV_LOADINFO_H

#ifdef CONFIG_SAVE_LOAD_INFO
#include <sysmgr/module_ctx.h>

int syssrv_loadinfo_init(struct sysmgr_module_ctx *ctx);
#endif

#endif
