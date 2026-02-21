/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for generic netlink
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 28 09:50:26 2019
 */

#ifndef __HM_GENETLINK_H_
#define __HM_GENETLINK_H_

#include <sys/types.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GENL_NAMSIZ	16
#define GENL_FAMILY_MAX		8

#define MCGRPS_CNT_MAX		32

struct genlmsghdr {
	uint8_t cmd;
	uint8_t version;
	uint16_t reserved;
};

#define GENL_HDRLEN	NLMSG_ALIGN(sizeof(struct genlmsghdr))
#define GENLMSG_DATA(glh)	((void *)((char *)NLMSG_DATA(glh) + GENL_HDRLEN))
#define GENLMSG_PAYLOAD(glh)	(NLMSG_PAYLOAD(glh, 0) - GENL_HDRLEN)
#define GENL_ID_CTRL	NLMSG_MIN_TYPE
#define GENL_ID_VFS_DQUOT       (GENL_ID_CTRL + 1)
#define GENL_ID_PMCRAID         (GENL_ID_CTRL + 2)
#define GENL_START_ALLOC        (GENL_ID_CTRL + 3)

enum {
	CTRL_CMD_UNSPEC = 0,
	CTRL_CMD_NEWFAMILY,
	CTRL_CMD_DELFAMILY,
	CTRL_CMD_GETFAMILY,
};

enum {
	CTRL_ATTR_UNSPEC = 0,
	CTRL_ATTR_FAMILY_ID,
	CTRL_ATTR_FAMILY_NAME,
	CTRL_ATTR_MCAST_GROUPS = 7,
	CTRL_ATTR_FAMILY_MAX,
};

enum {
	CTRL_ATTR_MCAST_GRP_UNSPEC,
	CTRL_ATTR_MCAST_GRP_NAME,
	CTRL_ATTR_MCAST_GRP_ID,
	__CTRL_ATTR_MCAST_GRP_MAX,
};

#define CTRL_ATTR_MCAST_GRP_MAX (__CTRL_ATTR_MCAST_GRP_MAX - 1)

#define VHOST_GENL_FAMILY_NAME		"vhost"

enum {
	UX_VHOST_CMD_UNSPEC = 0,
	UX_VHOST_CMD_REQUEST,
};

typedef struct ux_genl_mcgrp {
	char name[GENL_NAMSIZ];
} ux_genl_mcgrp_t;

typedef struct ux_reg_fam_info {
	char famname[GENL_NAMSIZ];
	uint32_t mcgrps_cnt;
	char grpnames[];
} ux_reg_fam_info_t;

int hm_netlink_register_generic(const void *info, const unsigned int len,
				unsigned long long dh_ap_cref,
				uint32_t *mcgrps_offset, uint16_t *familyid);
int hm_netlink_unregister_generic(uint16_t familyid);

#ifdef __cplusplus
}
#endif

#endif
