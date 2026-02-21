/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network submod
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 5 16:34:34 2019
 */

#ifndef _UXNET_SUBMODE_
#define _UXNET_SUBMODE_

int af_unix_init(void);
int af_packet_init(void);
int af_inet_init(void);
int af_can_init(void);
int af_netlink_init(void);
int af_vsock_init(void);
int ux_nf_conntrack_init(void);
int ux_vhost_init(void);

#ifdef CONFIG_NET_NS
int ux_ldk_namespace_init(void);
#endif

#ifdef CONFIG_UXNET_TUN
int af_tun_init(void);
#endif

#ifdef CONFIG_UXNET_VISP_PPP
int af_visp_ppp_init(void);
#endif

#endif
