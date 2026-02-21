/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Add linux/if_ppp.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 16 21:33:23 2023
 */
#ifndef _LINUX_IF_PPP_H
#define _LINUX_IF_PPP_H

#define PPPIOCNEWUNIT	_IOWR('t', 62, int)
#define PPPIOCCONNECT	_IOW('t', 58, int)
#define PPPIOCATTCHAN	_IOW('t', 56, int)
#define PPPIOCGCHAN	_IOR('t', 55, int)

#endif

