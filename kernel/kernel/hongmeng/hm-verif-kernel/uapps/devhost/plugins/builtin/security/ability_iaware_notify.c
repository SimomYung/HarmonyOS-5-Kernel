/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The ability check interface of iaware notify
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 10 10:44:26 2023
 */
#include <hongmeng/errno.h>

#include "iaware_notify.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(iaware_notify_thread_msg, NO_ARGS(), ab_as_sysmgr)

SEC_DEFINE_COMPONENT(iaware_notify, ability,
	sec_define_ability_method(iaware_notify_thread_msg)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/iaware_notify.h>
