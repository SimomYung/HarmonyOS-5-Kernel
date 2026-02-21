/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: interface for network can core
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#include <libhmsync/raw_rwlock.h>
#include <libstrict/strict.h>

#include "ux_netlog.h"
#include "ux_domain.h"
#include "ux_socket.h"
#include "ux_can_core.h"
#include "ux_can_proto.h"
#include "ux_can_netdev.h"
#include "ux_can_raw.h"
#include "ux_can_vcan.h"

static int ux_can_create(ux_socket_t *sock, int domain, int type, int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);
	const ux_can_proto_t *proto = NULL;

	if (protocol < 0 || protocol >= CAN_PROTO_MAX) {
		return E_HM_INVAL;
	}

	/* check whether protocol is supported */
	proto = ux_can_proto_get(protocol);
	if (proto == NULL) {
		return E_HM_PROTONOSUPPORT;
	}

	return proto->create(sock, type, protocol);
}

static ux_domain_t can_domain = {
	.family = AF_CAN,
	.create = ux_can_create,
};

int af_can_init(void)
{
	int ret;

	ret = ux_domain_register(&can_domain);
	if (ret) {
		net_error(HMNET_CAN, "register af_can domain failed\n");
		return ret;
	}

	ret = ux_can_netdev_init();
	if (ret) {
		net_error(HMNET_CAN, "init can netdev failed\n");
		goto err_netdev;
	}

	ret = ux_can_raw_socket_init();
	if (ret) {
		net_error(HMNET_CAN, "int can raw socket failed\n");
		goto err_socket;
	}

	ret = ux_can_vcan_init();
	if (ret) {
		net_error(HMNET_CAN, "int vcan device failed\n");
		goto err_vcan;
	}

	return 0;

err_vcan:

err_socket:
	ux_can_netdev_exit();
err_netdev:
	ux_domain_unregister(AF_CAN);
	return ret;
}
