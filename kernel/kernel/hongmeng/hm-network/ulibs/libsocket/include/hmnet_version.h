/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for hmnet version
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:21:00 2019
 */

#ifndef __HM_NET_VERSION_H_
#define __HM_NET_VERSION_H_

#include <string.h>
#include <sys/types.h>

#include <hongmeng/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hm_net_version {
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
	uint16_t build;
};

int hm_net_version_get(struct hm_net_version *version);
int hm_net_version_check(const struct hm_net_version *expected);

#ifdef __cplusplus
}
#endif

#endif
