/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Mapping network nodes to labels/SIDs
 * Author: Huawei OS Kernel Lab
 * Create: Thurs Mar 09 17:55:34 2023
 */

#ifndef __HMNETD_SECURITY_SEL_NODE_H__
#define __HMNETD_SECURITY_SEL_NODE_H__

int sel_netnode_sid(unsigned short family_sa, char *addr, socklen_t addrlen, hmsel_sid_t *sid);

#endif
