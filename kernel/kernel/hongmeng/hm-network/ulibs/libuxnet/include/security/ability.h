/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Include security components
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 15:40:43 2019
 */

#ifndef __HMNETD_SECURITY_ABILITY_H__
#define __HMNETD_SECURITY_ABILITY_H__

#include <libsec/ability.h>
#include <libsec/server.h>
#include <libpolicy_api.h>

extern struct sec_comp sec_comp_ability_reserved_port_range;
extern struct sec_comp sec_comp_ability_raw_socket;
extern struct sec_comp sec_comp_ability_net_admin;
extern struct sec_comp sec_comp_ability_net_bind;
extern struct sec_comp sec_comp_ability_net_dev;
extern struct sec_comp sec_comp_ability_net_fs;
extern struct sec_comp sec_comp_ability_vsock;
extern struct sec_comp sec_comp_ability_net_stack_init;
extern struct sec_comp sec_comp_ability_as_devhost;
extern struct sec_comp sec_comp_ability_as_netd;

#endif
