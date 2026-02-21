/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/neighbour.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 24 15:14:59 2021
 */

#ifndef _LINUX_NEIGHBOUR_H
#define _LINUX_NEIGHBOUR_H

enum {
	NDA_UNSPEC,
	NDA_DST,
	NDA_LLADDR,
	NDA_CACHEINFO,
	NDA_PROBES,
	NDA_VLAN,
	NDA_PORT,
	NDA_VNI,
	NDA_IFINDEX,
	NDA_MASTER,
	NDA_LINK_NETNSID,
	NDA_SRC_VNI,
	NDA_PROTOCOL,
	NDA_NH_ID,
	NDA_FDB_EXT_ATTRS,
	__NDA_MAX
};

struct ndmsg {
	__u8 	ndm_family;
	__u8 	ndm_pad1;
	__u16 	ndm_pad2;
	__s32 	ndm_ifindex;
	__u16 	ndm_state;
	__u8 	ndm_flags;
	__u8 	ndm_type;
};

#define NDA_MAX (__NDA_MAX - 1)

/* Neighbor Cache Entry Flags */
#define NTF_USE         0x01
#define NTF_SELF        0x02
#define NTF_MASTER      0x04
#define NTF_PROXY       0x08
#define NTF_EXT_LEARNED 0x10
#define NTF_OFFLOADED   0x20
#define NTF_STICKY      0x40
#define NTF_ROUTER      0x80
/* Neighbor Cache Entry States */
#define NUD_INCOMPLETE  0x01
#define NUD_REACHABLE   0x02
#define NUD_STALE       0x04
#define NUD_DELAY       0x08
#define NUD_PROBE       0x10
#define NUD_FAILED      0x20
#define NUD_NOARP       0x40
#define NUD_PERMANENT   0x80
#define NUD_NONE        0x00

#endif
