/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declaration of xvm lsyscall interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 15:05:30 2024
 */
#ifndef ULIBS_LIBHMPSF_ADAPTER_LSYSCALL_H
#define ULIBS_LIBHMPSF_ADAPTER_LSYSCALL_H

int xvm_use_lsyscall(void);
bool xvm_is_using_lsyscall(void); /* false: use hm actvcall; true: use lsyscall */

#endif
