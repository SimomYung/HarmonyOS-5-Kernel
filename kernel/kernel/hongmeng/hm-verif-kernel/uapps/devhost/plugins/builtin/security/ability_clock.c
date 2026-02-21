/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The ability check interface of clock
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 15:01:19 2020
 */

#include <hongmeng/errno.h>

#include "clock.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(clock_gettime, NO_ARGS(), ab_devhost_fops)
SEC_DEFINE_ABILITY_DEFAULT(clock_settime, NO_ARGS(), ab_devhost_fops)
SEC_DEFINE_ABILITY_DEFAULT(clock_getres, NO_ARGS(), ab_devhost_fops)
SEC_DEFINE_ABILITY_DEFAULT(clock_adjtime, NO_ARGS(), ab_devhost_fops)
SEC_DEFINE_ABILITY_DEFAULT(clock_poweroff_alarm, NO_ARGS(), ab_devhost_fops)

SEC_DEFINE_COMPONENT(clock, ability,
	sec_define_ability_method(clock_gettime),
	sec_define_ability_method(clock_settime),
	sec_define_ability_method(clock_getres),
	sec_define_ability_method(clock_adjtime),
	sec_define_ability_method(clock_poweroff_alarm)
);
#define ENABLE_SEC_MOD_ABILITY
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/clock.h>
