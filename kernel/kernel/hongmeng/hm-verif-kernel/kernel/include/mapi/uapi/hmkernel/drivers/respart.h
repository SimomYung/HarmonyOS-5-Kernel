/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Struct definitions of resource partition for user
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 22 10:25:38 2019
 */
#ifndef UAPI_HMKERNEL_RESPART_H
#define UAPI_HMKERNEL_RESPART_H

#include <hmasm/types.h>
#include <hmkernel/cpuset.h>

/*
 * Hi1620 2P board includes 2 sockets (4 numa nodes) and each
 * numa node have 2 MPAM resouce nodes (L3T and HHA).
 *
 * Up to 4 chips are supported for consistent interconnection by a HHA.
 */
#define NR_RES_NODE		16U
#define RES_NAME_SIZE_MAX	64U

#define RES_PARTITION_GET_INFO		0x01
#define RES_PARTITION_SET_INFO		0x02
#define RES_PARTITION_MONITOR_ENABLE	0x03
#define RES_PARTITION_MONITOR_DISABLE	0x04
#define RES_PARTITION_MONITOR_CAPTURE	0x05

/* resource type */
enum __res_type {
	RES_CACHE,	/* cache */
	RES_MEMBW	/* memory bandwidth */
};

/* resource node information */
struct __res_node_info_s {
	__u32 nodeid;			/* resource node ID */
	__u32 num_partid;		/* number of partitions available */
	__u32 num_pmg;			/* number of performance monitoring groups available */
	__u32 num_mon;			/* number of monitor available */
	__u64 capacity;			/* capacity of resouce */
	__u32 type;			/* type of resouece: RES_CACHE or RES_MEMBW */
	char name[RES_NAME_SIZE_MAX];	/* name of resource */
};

/* the partition of resource node information */
struct __res_partition_info_s {
	__u32 nodeid;		/* resource node ID */
	__u32 partid;		/* partition ID */
	__u32 pmg;		/* performance monitoring group */
	__u64 val;		/* cache or mem ctrl values */
	__cpuset_t cpuset;	/* which cpus share this resource partition */
};

#endif
