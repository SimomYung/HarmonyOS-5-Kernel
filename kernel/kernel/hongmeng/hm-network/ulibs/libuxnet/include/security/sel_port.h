/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Mapping network ports to labels/SIDs
 * Author: Huawei OS Kernel Lab
 * Create: Thurs Mar 09 17:55:34 2023
 */

#ifndef __HMNETD_SECURITY_SEL_PORT_H__
#define __HMNETD_SECURITY_SEL_PORT_H__

int sel_netport_sid(uint8_t protocol, uint16_t port, hmsel_sid_t *sid);

#endif
