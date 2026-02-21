/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The ability check interface of net
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 14:23:19 2020
 */

#include <hongmeng/errno.h>

#include "net.h"
#include "ability.h"

SEC_DEFINE_SRC_CRED_ABILITY_DEFAULT(netd, NO_ARGS(), ab_netd)

SEC_DEFINE_ABILITY_DEFAULT(net_prepare, NO_ARGS(), ab_devhost_net_prepare)
SEC_DEFINE_ABILITY_DEFAULT(net_stat, NO_ARGS(), ab_netd)
SEC_DEFINE_ABILITY_DEFAULT(net_send, NO_ARGS(), ab_netd)
SEC_DEFINE_ABILITY_DEFAULT(net_init, NO_ARGS(), ab_devhost_net_init)

SEC_DEFINE_COMPONENT(net, ability,
	sec_define_ability_method(net_prepare),
	sec_define_ability_method(net_stat),
	sec_define_ability_method(net_send),
	sec_define_chk_xact_method(net_ioctl, sec_chk_ability_netd),
	sec_define_ability_method(net_init)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/net.h>
