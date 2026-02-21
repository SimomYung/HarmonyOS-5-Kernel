/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/net_namespace to compat linux header files.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 15:47:18 2021
 */

#ifndef LINUX_NET_NAMESPACE_H
#define LINUX_NET_NAMESPACE_H

enum {
	NETNSA_NONE,
#define NETNSA_NSID_NOT_ASSIGNED (-1)
	NETNSA_NSID,
	NETNSA_PID,
	NETNSA_FD,
	NETNSA_TARGET_NSID,
	NETNSA_CURRENT_NSID,
	__NETNSA_MAX,
};

#define NETNSA_MAX		(__NETNSA_MAX - 1)

#endif
