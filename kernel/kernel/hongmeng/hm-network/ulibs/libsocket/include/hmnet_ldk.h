/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Header file for hmnet ldk
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:41:00 2022
 */

#ifndef __HM_NET_LDK_H_
#define __HM_NET_LDK_H_

#include <string.h>
#include <sys/types.h>

#include <hongmeng/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

int hm_net_ldk_domain_register(int family, unsigned long long dh_ap_cref);

#ifdef __cplusplus
}
#endif

#endif
