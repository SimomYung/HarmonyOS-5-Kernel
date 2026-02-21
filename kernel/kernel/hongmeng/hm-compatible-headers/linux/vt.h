/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/vt.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 19:23:43 2021
 */

#ifndef LINUX_VT_H
#define LINUX_VT_H

struct vt_stat {
	unsigned short v_active;
	unsigned short v_signal;
	unsigned short v_state;
};
#define VT_GETSTATE	0x5603

#endif
