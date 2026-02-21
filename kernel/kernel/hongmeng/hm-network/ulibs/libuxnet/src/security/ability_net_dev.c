/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The ability check interface of net_dev
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 21:59:19 2019
 */

#include <hongmeng/errno.h>
#include "security/net_dev.h"
#include <generated/hmsd/hook/net/net_dev.h>
#include "security/ability.h"

SEC_DEFINE_ABILITY_DEFAULT(net_dev, NO_ARGS(), ab_net_dev)

SEC_DEFINE_COMPONENT(net_dev, ability,
		     sec_define_ability_method(net_dev)
		    );
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/net/net_dev.h>
