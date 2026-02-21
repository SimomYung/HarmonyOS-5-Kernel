/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: The ability check interface of as_netd
 * Author: Huawei OS Kernel Lab
 * Create: Tues June 25 11:20:19 2024
 */

#include <hongmeng/errno.h>
#include "security/as_netd.h"
#include <generated/hmsd/hook/net/as_netd.h>
#include "security/ability.h"

SEC_DEFINE_ABILITY_DEFAULT(as_netd, NO_ARGS(), ab_as_lwipd)

SEC_DEFINE_COMPONENT(as_netd, ability,
		     sec_define_ability_method(as_netd)
		    );

#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/net/as_netd.h>
