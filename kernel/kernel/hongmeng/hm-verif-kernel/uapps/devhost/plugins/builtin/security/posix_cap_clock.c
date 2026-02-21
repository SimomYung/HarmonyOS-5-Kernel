/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The posix_cap check interface of clock
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 16:13:42 2020
 */

#include <hongmeng/errno.h>

#include "clock.h"
#include "posix_cap.h"

SEC_DEFINE_POSIX_CAP_DEFAULT(clock_settime, NO_ARGS(), POSIX_CAP_SYS_TIME)

SEC_DEFINE_COMPONENT(clock, posix_cap,
	sec_define_posix_cap_method(clock_settime)
);
#define ENABLE_SEC_MOD_CAPABILITY
#include <generated/hmsd/module/devhost/clock.h>
