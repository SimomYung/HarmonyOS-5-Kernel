/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description:  Header file for network can filter
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 19:27:00 2019
 */

#ifndef __UX_CAN_FILTER_H_
#define __UX_CAN_FILTER_H_

#include "hm_can.h"
#include "ux_socket.h"
#include "ux_netdev.h"
#include "ux_pktbuf.h"

int ux_can_filters_register(ux_netdev_t *dev, struct can_filter *filter,
			   int count);
void ux_can_filters_unregister(ux_netdev_t *dev, struct can_filter *filter,
			     int count);
int ux_can_filters_dispatch(ux_netdev_t *dev, ux_pktbuf_t *pkt);

#endif
