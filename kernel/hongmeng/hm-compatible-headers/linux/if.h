/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/if.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 15:06:38 2021
 */

#ifndef LINUX_IF_H
#define LINUX_IF_H

#include <linux/types.h>
#include <linux/socket.h>
#include <linux/compiler_types.h>

#include <linux/hdlc/ioctl.h>


enum net_device_flags {
	IFF_UP				= 1 << 0,
	IFF_LOOPBACK			= 1 << 3,
	IFF_RUNNING			= 1 << 6,
	IFF_PROMISC			= 1 << 8,
};

struct ifmap {
	unsigned long mem_start;
	unsigned long mem_end;
	unsigned short base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
};

struct if_settings {
	unsigned int type;
	unsigned int size;
	union {
		raw_hdlc_proto		__user *raw_hdlc;
		cisco_proto		__user *cisco;
		fr_proto		__user *fr;
		fr_proto_pvc		__user *fr_pvc;
		fr_proto_pvc_info	__user *fr_pvc_info;
		x25_hdlc_proto		__user *x25;
		sync_serial_settings	__user *sync;
		te1_settings		__user *te1;
	} ifs_ifsu;
};

struct ifreq {
#define IFHWADDRLEN	6
	union {
		char	ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		struct	sockaddr ifru_netmask;
		struct  sockaddr ifru_hwaddr;
		short	ifru_flags;
		int	ifru_ivalue;
		int	ifru_mtu;
		struct  ifmap ifru_map;
		char	ifru_slave[IFNAMSIZ];
		char	ifru_newname[IFNAMSIZ];
		void	__user*	ifru_data;
		struct	if_settings ifru_settings;
	} ifr_ifru;
};

#define ifr_name	ifr_ifrn.ifrn_name
#define ifr_hwaddr	ifr_ifru.ifru_hwaddr
#define ifr_addr	ifr_ifru.ifru_addr
#define ifr_data	ifr_ifru.ifru_data

struct ifconf  {
	int	ifc_len;
	union {
		char __user* ifcu_buf;
		struct ifreq __user *ifcu_req;
	} ifc_ifcu;
};

#define ifc_buf		ifc_ifcu.ifcu_buf
#define ifc_req		ifc_ifcu.ifcu_req
#define	ifr_flags	ifr_ifru.ifru_flags
#define ifr_ifindex	ifr_ifru.ifru_ivalue

#endif
