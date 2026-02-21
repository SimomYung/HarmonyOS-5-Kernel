/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The ability check interface of rpc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 15:01:19 2020
 */

#include <hongmeng/errno.h>

#include "rpc.h"
#include "ability.h"

SEC_DEFINE_SRC_CRED_ABILITY_DEFAULT(rpc_invoke, NO_ARGS(), ab_devhost_rpc_invoke)

SEC_DEFINE_COMPONENT(rpc, ability,
	sec_define_chk_xact_method(rpc_invoke, sec_chk_ability_rpc_invoke)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/rpc.h>
