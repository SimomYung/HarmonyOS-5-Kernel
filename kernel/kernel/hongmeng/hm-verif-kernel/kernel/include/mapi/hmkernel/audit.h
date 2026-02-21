/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Export kernel audit.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 29 10:42:32 2021
 */
#ifndef MAPI_HMKERNEL_AUDIT_H
#define MAPI_HMKERNEL_AUDIT_H
#include <uapi/hmkernel/audit.h>
#ifdef CONFIG_AUDIT
bool audit_is_enabled(void);

void audit_set_status(bool status);
#endif
#endif
