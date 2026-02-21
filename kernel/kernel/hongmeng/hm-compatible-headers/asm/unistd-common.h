/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Compat for asm/unistd-common.h
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 15 23:04:15 2022
 */
#ifndef _ASM_ARM_UNISTD_COMMON_H
#define _ASM_ARM_UNISTD_COMMON_H

#define __NR_capget 90
#define __NR_capset 91
#define __NR_getuid 174
#define __NR_sendmsg 211
#define __NR_perf_event_open 241
#define __NR_recvmmsg 243

#endif
