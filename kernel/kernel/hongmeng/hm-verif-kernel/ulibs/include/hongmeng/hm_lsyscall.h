/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: hm lsyscall number
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 26 14:24:00 2024
 */

#ifndef ULIBS_HONGMENG_HM_LSYSCALL_H
#define ULIBS_HONGMENG_HM_LSYSCALL_H

#define HM_LSYSCALL(name, nr) __NR_##name = (nr),
enum {
#include <hongmeng/hm_lsyscall.h.in>
};
#undef HM_LSYSCALL

#define HM_LSYSCALL(name, nr) __SYS_##name = (nr),
enum {
#include <hongmeng/hm_lsyscall.h.in>
};
#undef HM_LSYSCALL

#endif
