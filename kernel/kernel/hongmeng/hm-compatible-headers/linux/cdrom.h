/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/cdrom.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 11:50:09 2021
 */

#ifndef LINUX_CDROM_H
#define LINUX_CDROM_H

#include <linux/types.h>

#ifndef INT_MAX
#define INT_MAX			0x7fffffff
#endif

#define	EOPNOTSUPP		95
#define EDRIVE_CANT_DO_THIS	EOPNOTSUPP
#define CDROMEJECT		0x5309
#define CDROMMULTISESSION	0x5310

#define CDROMCLOSETRAY		0x5319
#define CDROM_SET_OPTIONS	0x5320
#define CDROM_CLEAR_OPTIONS	0x5321
#define CDROM_SELECT_SPEED	0x5322
#define CDROM_SELECT_DISC	0x5323

#define CDROM_DRIVE_STATUS	0x5326
#define CDROM_LOCKDOOR		0x5329
#define CDROM_GET_CAPABILITY	0x5331

#define CDROM_LAST_WRITTEN	0x5395

struct cdrom_msf0 {
	__u8	minute;
	__u8	second;
	__u8	frame;
};

union cdrom_addr {
	struct cdrom_msf0	msf;
	int			lba;
};

struct cdrom_multisession {
	union	cdrom_addr addr;
	__u8	xa_flag;
	__u8	addr_format;
};

#define CDROM_LBA		0x01

#define CDS_NO_INFO		0
#define CDS_NO_DISC		1
#define CDS_TRAY_OPEN		2
#define CDS_DRIVE_NOT_READY	3
#define CDS_DISC_OK		4

#define CDO_AUTO_EJECT		0x2
#define CDSL_CURRENT		INT_MAX

#endif
