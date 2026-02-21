/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Ability check methods for entropy under MAC access control
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 10 15:40:38 2020
 */

#include "entropy.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(entropy_read, NO_ARGS(), ab_as_crypto)

SEC_DEFINE_COMPONENT(entropy_dh, ability,
	sec_define_ability_method(entropy_read)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/entropy.h>
