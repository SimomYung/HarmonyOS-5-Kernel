/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Include security components
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 16:42:03 2020
 */

#ifndef DEVHOST_SECURITY_ABILITY_H
#define DEVHOST_SECURITY_ABILITY_H

#include <libpolicy_api.h>

#include <libsec/server.h>
#include <libsec/ability.h>

extern struct sec_comp sec_comp_ability_common;
extern struct sec_comp sec_comp_ability_net;
extern struct sec_comp sec_comp_ability_net_socket;
extern struct sec_comp sec_comp_ability_iaware_notify;
extern struct sec_comp sec_comp_ability_kstate;
extern struct sec_comp sec_comp_ability_fileguard;
extern struct sec_comp sec_comp_ability_rpc;
extern struct sec_comp sec_comp_ability_fops;
extern struct sec_comp sec_comp_ability_pm;
extern struct sec_comp sec_comp_ability_clock;
extern struct sec_comp sec_comp_ability_entropy_dh;

#endif
