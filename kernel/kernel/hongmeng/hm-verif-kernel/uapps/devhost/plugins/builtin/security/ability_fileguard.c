/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define interface of ability_fileguard
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */
#include <hongmeng/errno.h>

#include "fileguard.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(fileguard_message_notify, NO_ARGS(), ab_as_vfs)

SEC_DEFINE_COMPONENT(fileguard, ability,
	sec_define_ability_method(fileguard_message_notify)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/fileguard.h>
