/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for hm_ifaddrs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 14:53:17 2019
 */

#ifndef HM_INCLUDE_HM_IFADDRS_H
#define HM_INCLUDE_HM_IFADDRS_H

#include <ifaddrs.h>
#include <net/if.h>
#include <hmasm/types.h>

struct if_stats {
	__u32	packets_received;
	__u32	packets_transmitted;
	__u32	total_bytes_received;
	__u32	total_bytes_transmitted;
	__u32	errors_received;
	__u32	errors_transmit;
	__u32	dropped_recv;
	__u32	dropped_send;
	__u32	multicast;
	__u32	collisions;

	/* extend for further use */
	__u32	buf[13];
};

struct ifaddrs_storage_internal {
	unsigned ifa_flags;
	char name[IFNAMSIZ + 1];
	struct sockaddr ifa_addr;
	struct sockaddr ifa_netmask;
	union {
		struct sockaddr ifu_broadaddr;
		struct sockaddr ifu_dstaddr;
	} ifa_ifu;
	struct if_stats stats;
};

struct ifaddrs_storage {
	struct ifaddrs ifa;
	struct ifaddrs_storage_internal storage;
};

#endif
