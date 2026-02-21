/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: track info implement interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 21 04:36:26 PM 2025
 */

#ifndef HMKERNEL_DRIVERS_TRACK_INFO_H
#define HMKERNEL_DRIVERS_TRACK_INFO_H

#include <hmkernel/errno.h>

#ifdef CONFIG_TRACK_INFO
int track_info_set(bool pause);
#else
static inline int track_info_set(bool pause)
{
	UNUSED(pause);
	return E_HM_NOSYS;
}
#endif /* end of CONFIG_TRACK_INFO */
#endif /* end of HMKERNEL_DRIVERS_TRACK_INFO_H */
