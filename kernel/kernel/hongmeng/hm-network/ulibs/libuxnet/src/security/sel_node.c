/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: mapping network nodes to labels/SIDs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 3 10:34:09 2023
 */

#include <libhmseharmony/services.h>

#include "ux_socket.h"
#include "security/sel_node.h"

int sel_netnode_sid(unsigned short family_sa, char *addr, socklen_t addrlen, hmsel_sid_t *sid)
{
	return hmsel_node_sid(family_sa, addr, addrlen, sid);
}
