/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The ability check interface of raw_socket
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 21:59:19 2019
 */

#include <hongmeng/errno.h>
#include "security/raw_socket.h"
#include "security/ability.h"
#include <generated/hmsd/hook/net/raw_socket.h>

SEC_DEFINE_ABILITY_DEFAULT(raw_socket, NO_ARGS(), ab_raw_socket)

SEC_DEFINE_COMPONENT(raw_socket, ability,
		     sec_define_ability_method(raw_socket)
		    );
#define ENABLE_SEC_MOD_ABILITY
#include <generated/hmsd/module/net/raw_socket.h>
