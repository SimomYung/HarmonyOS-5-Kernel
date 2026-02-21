/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: The ability check interface of vsock
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 29 15:11:11 2021
 */

#include <hongmeng/errno.h>
#include <libsec/mac.h>

#include "security/vsock.h"
#include "security/ability.h"

#include <generated/hmsd/hook/net/vsock.h>

SEC_DEFINE_ABILITY_DEFAULT(vsock, NO_ARGS(), ab_vsock)
SEC_DEFINE_ABILITY_DEFAULT(vsock_create, NO_ARGS(), ab_vsock_create)

SEC_DEFINE_COMPONENT(vsock, ability,
		     sec_define_ability_method(vsock),
			 sec_define_ability_method(vsock_create),
		    );
#define ENABLE_SEC_MOD_ABILITY
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/net/vsock.h>
