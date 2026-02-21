/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description:  Header file for network can protocol
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#ifndef __UX_CAN_PROTO_H_
#define __UX_CAN_PROTO_H_

#include <stdint.h>

#include "hm_can.h"
#include "ux_socket.h"

#define CAN_PROTO_MAX	CAN_NPROTO

typedef struct ux_can_proto {
	int type;
	int proto;
	int (*create)(ux_socket_t *socket, int type, int protocol);
} ux_can_proto_t;

int ux_can_proto_register(const ux_can_proto_t *proto);
void ux_can_proto_unregister(const ux_can_proto_t *proto);

const ux_can_proto_t *ux_can_proto_get(int proto);

#endif
