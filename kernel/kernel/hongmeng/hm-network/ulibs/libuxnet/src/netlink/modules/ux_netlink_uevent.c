/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Netlink uevent protocol implements
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 11 09:36:03 CST 2020
 */

#include <libstrict/strict.h>

#include "ux_netlink_uevent.h"
#include "ux_netlink_core.h"
#include "ux_netlink_table.h"
#include "ux_netlink_proto.h"

static ux_netlink_proto_t netlink_uevent = {
	.rcv = NULL,
	.protocol = NETLINK_KOBJECT_UEVENT,
	.is_dynamic = false,
	.refcnt = __RAW_REFCNT_NR_INITIALIZER(0),
};

int ux_netlink_uevent_init(void)
{
	int ret;

	ret = ux_netlink_create_service(&netlink_uevent);
	if (ret != E_HM_OK) {
		net_warn(HMNET_NETLINK, "uevent: create route service failed\n");
		return ret;
	}

	ret = ux_netlink_proto_register(&netlink_uevent);
	if (ret != E_HM_OK) {
		net_warn(HMNET_NETLINK, "uevent: register uevent proto failed\n");
		return ret;
	}

	return 0;
}

void ux_netlink_uevent_exit(void)
{
	ux_netlink_proto_unregister(&netlink_uevent);
}

int ux_uevent_receive(unsigned int dst_group, const char *msg, const size_t len)
{
	return ux_netlink_broadcast(NETLINK_KOBJECT_UEVENT, dst_group, msg, len);
}
