/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Header file of ethtool defines for ifenslave
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 7 11:00:34 2020
 */

#ifndef _UX_ETHTOOL_H
#define _UX_ETHTOOL_H

#include <hmkernel/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ETHTOOL_GDRVINFO	0x00000003 /* Get driver info. */

#define ETHTOOL_FWVERS_LEN	32
#define ETHTOOL_BUSINFO_LEN	32
#define ETHTOOL_EROMVERS_LEN	32
#define ETHTOOL_DRVNAME_LEN	32
#define ETHTOOL_DRVVERSION_LEN	32
#define RESERVED_LEN	12
struct ethtool_drvinfo {
	__u32	cmd;
	char	driver[ETHTOOL_DRVNAME_LEN];
	char	version[ETHTOOL_DRVVERSION_LEN];
	char	fw_version[ETHTOOL_FWVERS_LEN];
	char	bus_info[ETHTOOL_BUSINFO_LEN];
	char	erom_version[ETHTOOL_EROMVERS_LEN];
	char	reserved2[RESERVED_LEN];
	__u32	n_priv_flags;
	__u32	n_stats;
	__u32	testinfo_len;
	__u32	eedump_len;
	__u32	regdump_len;
};

#ifdef __cplusplus
}
#endif

#endif
