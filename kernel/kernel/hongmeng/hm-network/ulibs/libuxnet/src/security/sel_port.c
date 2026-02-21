/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: mapping network ports to labels/SIDs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 3 10:34:09 2023
 */

#include <libhmseharmony/services.h>

#include "security/sel_port.h"

int sel_netport_sid(uint8_t protocol, uint16_t port, hmsel_sid_t *sid)
{
	return hmsel_port_sid(protocol, port, sid);
}
