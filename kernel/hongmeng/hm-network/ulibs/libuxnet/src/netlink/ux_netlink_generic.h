/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Netlink generic methods library
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 17 08:23:50 2020
 */

#ifndef __UX_NETLINK_GENERIC_H_
#define __UX_NETLINK_GENERIC_H_

#include "hm_netlink.h"
#include "hm_genetlink.h"
#include "ux_socket.h"

typedef struct ux_netlink_generic_ops {
	uint8_t cmd;
	int (*exec)(ux_sock_t *sk, const struct nlmsghdr *nlh, void *attrs, uint32_t size);
	int (*dump)(ux_sock_t *sk, const struct nlmsghdr *nlh);
} ux_netlink_generic_ops_t;

typedef struct ux_netlink_generic {
	char name[GENL_NAMSIZ];
	const ux_netlink_generic_ops_t *ops;
	uint16_t id;
	uint16_t ops_cnt;
	uint32_t mcgrps_offset;
	uint32_t mcgrps_cnt;
	rref_t dh_ap_rref;
	uint32_t grpnames_len;
	ux_genl_mcgrp_t grpnames[];
} ux_netlink_generic_t;

void ux_netlink_generic_rdlock(void);
void ux_netlink_generic_wrlock(void);
void ux_netlink_generic_unlock(void);
ux_netlink_generic_t *ux_genl_family_lookup_by_id(uint16_t id);
int ux_netlink_register_generic(ux_netlink_generic_t *family);
void ux_netlink_unregister_generic(const ux_netlink_generic_t *family);

int ux_netlink_generic_init(void);
void ux_netlink_generic_exit(void);

#endif
