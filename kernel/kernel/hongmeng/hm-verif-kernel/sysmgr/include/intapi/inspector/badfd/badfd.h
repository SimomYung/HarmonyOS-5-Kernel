/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: provide interface of badfd
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 9 15:00:00 2024
 */
#ifndef SYSMGR_INSPECTOR_BADFD_H
#define SYSMGR_INSPECTOR_BADFD_H
#include <hmasm/activation/upcall.h>

#ifdef CONFIG_BAD_FD_DSM
void badfd_handle_error(unsigned long credential, union __arch_actv_upcall_args_u *args);
#endif /* CONFIG_BAD_FD_DSM */

#endif