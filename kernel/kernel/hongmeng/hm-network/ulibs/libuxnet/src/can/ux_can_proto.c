/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: interface for network can protocol
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#include <libhmsync/raw_mutex.h>

#include "ux_can_proto.h"

static struct raw_mutex ux_can_proto_lock = RAW_MUTEX_INITIALIZER;
static const ux_can_proto_t *ux_can_protos[CAN_PROTO_MAX] = { 0 };

int ux_can_proto_register(const ux_can_proto_t *proto)
{
	int ret = 0;

	if (proto->proto < 0 || proto->proto >= CAN_PROTO_MAX) {
		return E_HM_INVAL;
	}

	raw_mutex_lock(&ux_can_proto_lock);
	if (!ux_can_protos[proto->proto]) {
		ux_can_protos[proto->proto] = proto;
	} else {
		ret = E_HM_OBJEXIST;
	}
	raw_mutex_unlock(&ux_can_proto_lock);

	return ret;
}

void ux_can_proto_unregister(const ux_can_proto_t *proto)
{
	if (proto->proto < 0 || proto->proto >= CAN_PROTO_MAX) {
		return;
	}

	raw_mutex_lock(&ux_can_proto_lock);
	ux_can_protos[proto->proto] = NULL;
	raw_mutex_unlock(&ux_can_proto_lock);
}

const ux_can_proto_t *ux_can_proto_get(int proto)
{
	if (proto < 0 || proto > CAN_PROTO_MAX) {
		return NULL;
	}

	return ux_can_protos[proto];
}
