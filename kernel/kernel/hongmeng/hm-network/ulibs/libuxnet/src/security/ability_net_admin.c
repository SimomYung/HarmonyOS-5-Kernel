/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The ability check interface of net_admin
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 21:59:19 2019
 */

#include <hongmeng/errno.h>
#include "security/net_admin.h"
#include <generated/hmsd/hook/net/net_admin.h>
#include "security/ability.h"

SEC_DEFINE_ABILITY_DEFAULT(net_admin, NO_ARGS(), ab_net_admin)

SEC_DEFINE_COMPONENT(net_admin, ability,
		     sec_define_ability_method(net_admin)
		    );
#define ENABLE_SEC_MOD_ABILITY
#include <generated/hmsd/module/net/net_admin.h>
