/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The ability check interface of pm
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 15:01:19 2020
 */

#include <hongmeng/errno.h>

#include "pm.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(pm_action, NO_ARGS(), ab_devhost_pm)
SEC_DEFINE_ABILITY_DEFAULT(power_off, NO_ARGS(), ab_devhost_pm)
SEC_DEFINE_ABILITY_DEFAULT(cpuhp_action, NO_ARGS(), ab_devhost_pm)

SEC_DEFINE_COMPONENT(pm, ability,
	sec_define_ability_method(pm_action),
	sec_define_ability_method(power_off),
	sec_define_ability_method(cpuhp_action)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/pm.h>
