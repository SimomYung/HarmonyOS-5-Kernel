/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of ability_kstate
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 24 22:54:48 CST 2023
 */
#include <hongmeng/errno.h>

#include "kstate.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(kstate_binderinfo_notify, NO_ARGS(), ab_as_vfs)
SEC_DEFINE_ABILITY_DEFAULT(kstate_killinfo_notify, NO_ARGS(), ab_as_vfs)
SEC_DEFINE_ABILITY_DEFAULT(kstate_freeze_binder_info_notify, NO_ARGS(), ab_as_vfs)
SEC_DEFINE_ABILITY_DEFAULT(kstate_request_anco_thaw, NO_ARGS(), ab_as_vfs)

SEC_DEFINE_COMPONENT(kstate, ability,
	sec_define_ability_method(kstate_binderinfo_notify),
	sec_define_ability_method(kstate_killinfo_notify),
	sec_define_ability_method(kstate_freeze_binder_info_notify),
	sec_define_ability_method(kstate_request_anco_thaw)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/kstate.h>
