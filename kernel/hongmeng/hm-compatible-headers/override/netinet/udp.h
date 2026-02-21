/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 18:02:08 2022
 */
#ifndef _OVERRIDE_NETINET_UDP_H
#define _OVERRIDE_NETINET_UDP_H

/*
 * To adapt to GLIBC,
 * for GLIBC doesn't need to define _GNU_SOURCE to use these features,
 * but native MUSL need.
 */
#ifndef _GNU_SOURCE
#define uh_sport source
#define uh_dport dest
#define uh_ulen len
#define uh_sum check
#endif

#include <muslorigin/netinet/udp.h>

#endif
