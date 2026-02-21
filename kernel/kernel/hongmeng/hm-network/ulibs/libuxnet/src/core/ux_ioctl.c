/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Ioctl functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 17 17:23:33 2020
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <net/if_arp.h>

#include <hmkernel/errno.h>
#include <libhmlog/hmlog.h>

#include "ux_ioctl.h"

int ux_ioctl_cmdlen(int cmd)
{
	int ret;

	switch (cmd) {
	case FIONBIO:
	case FIONREAD:
		ret = (int)sizeof(uint32_t);
		break;
	case SIOCDARP:
	case SIOCGARP:
	case SIOCSARP:
		ret = (int)sizeof(struct arpreq);
		break;
	default:
		ret = E_HM_INVAL;
	}

	return ret;
}
