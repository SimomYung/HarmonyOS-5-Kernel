/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header file for bonding ioctl
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 7 11:00:34 2020
 */

#ifndef _UX_IF_BONDING_H
#define _UX_IF_BONDING_H

#include <sys/ioctl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIOCBONDENSLAVE	0x8990
#define SIOCBONDRELEASE 0x8991
#define SIOCBONDSETHWADDR      0x8992
#define SIOCBONDSLAVEINFOQUERY 0x8993
#define SIOCBONDINFOQUERY      0x8994
#define SIOCBONDCHANGEACTIVE   0x8995

#define BOND_ENSLAVE_OLD		(SIOCDEVPRIVATE)
#define BOND_RELEASE_OLD		(SIOCDEVPRIVATE + 1)
#define BOND_SETHWADDR_OLD		(SIOCDEVPRIVATE + 2)
#define BOND_SLAVE_INFO_QUERY_OLD	(SIOCDEVPRIVATE + 11)
#define BOND_INFO_QUERY_OLD		(SIOCDEVPRIVATE + 12)
#define BOND_CHANGE_ACTIVE_OLD		(SIOCDEVPRIVATE + 13)

#ifdef __cplusplus
}
#endif

#endif /* _UX_IF_BONDING_H */

