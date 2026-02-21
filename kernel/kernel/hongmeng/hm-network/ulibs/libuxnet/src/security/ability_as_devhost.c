/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The ability check interface of as_devhost
 * Author: Huawei OS Kernel Lab
 * Create: Thu July 27 21:59:19 2019
 */

#include <hongmeng/errno.h>
#include "security/as_devhost.h"
#include <generated/hmsd/hook/net/as_devhost.h>
#include "security/ability.h"

SEC_DEFINE_ABILITY_DEFAULT(as_devhost, NO_ARGS(), ab_as_devhost)

SEC_DEFINE_COMPONENT(as_devhost, ability,
		     sec_define_ability_method(as_devhost)
		    );

#define ENABLE_SEC_MOD_MAC
#include <generated/hmsd/module/net/as_devhost.h>
