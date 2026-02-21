/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The ability check interface of net_bind
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 24 10:37:05 2022
 */

#include <hongmeng/errno.h>
#include <libsec/mac.h>

#include "security/net_bind_service.h"
#include "security/ability.h"
#include <generated/hmsd/hook/net/net_bind_service.h>

SEC_DEFINE_ABILITY_DEFAULT(net_bind_service, NO_ARGS(), ab_net_bind)

SEC_DEFINE_COMPONENT(net_bind, ability,
		     sec_define_ability_method(net_bind_service)
		    );
#define ENABLE_SEC_MOD_ABILITY
#include <generated/hmsd/module/net/net_bind_service.h>
