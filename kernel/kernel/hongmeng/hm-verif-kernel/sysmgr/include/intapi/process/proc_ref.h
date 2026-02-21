/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Proc ref interface declaration
 * Author: Huawei OS Kernel Lab
 * Mon Oct 14 07:38:57 PM CST 2024
 */
#ifndef SYSMGR_INTAPI_PROC_REF_H
#define SYSMGR_INTAPI_PROC_REF_H

uintptr_t process_ref_get_current(void);
void process_ref_put(uintptr_t process_ptr);

#endif
