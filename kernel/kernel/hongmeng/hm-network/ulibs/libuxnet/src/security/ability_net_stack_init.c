/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The ability check interface of net_stack_init
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 21:59:19 2022
 */

#include <hongmeng/errno.h>
#include "security/net_stack_init.h"
#include "security/ability.h"
#include <generated/hmsd/hook/net/net_stack_init.h>

SEC_DEFINE_ABILITY_DEFAULT(net_stack_init, NO_ARGS(), ab_net_stack_init)

SEC_DEFINE_COMPONENT(net_stack_init, ability,
		     sec_define_ability_method(net_stack_init)
		    );
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/net/net_stack_init.h>
