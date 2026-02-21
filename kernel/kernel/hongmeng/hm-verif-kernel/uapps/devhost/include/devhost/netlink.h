/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Prototypes for devhost netlink APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 10 10:04:17 2020
 */
#ifndef __DEVHOST_API_NETLINK_H__
#define __DEVHOST_API_NETLINK_H__

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */
struct netlink_rcv_msg {
	const void *msg;
	unsigned long msg_size;
	int protocol;
	unsigned int portid;
};
typedef int (*dh_netlink_rcv_if_t)(struct netlink_rcv_msg *args);
#ifdef CONFIG_DEVHOST_NETLINK
int devhost_netlink_register_entr(dh_netlink_rcv_if_t func);
int devhost_netlink_multicast(const void *msg,
			      unsigned int msg_size, int protocol,
			      unsigned int dst_group);
int devhost_netlink_unicast(const void *msg,
			    unsigned int msg_size, int protocol,
			    unsigned int dst_portid);

int devhost_netlink_register_genl(const char *name, const void *fam_info, unsigned long info_len,
				  unsigned int *fam_id, unsigned int *mcgrp_offset);
int devhost_netlink_unregister_genl(unsigned int fam_id);
int devhost_netlink_register(unsigned int protocol, unsigned int nr_grp);
int devhost_netlink_unregister(unsigned int protocol);
#else
static inline int devhost_netlink_register_entr(dh_netlink_rcv_if_t func)
{
	(void)func;
	return 0;
}
#endif

#endif /* __DEVHOST_API_NETLINK_H__ */
