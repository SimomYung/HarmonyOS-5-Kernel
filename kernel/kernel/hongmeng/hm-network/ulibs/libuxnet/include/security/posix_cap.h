/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The security template of net posix cap
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 18 15:31:34 2020
 */

#ifndef __HMNETD_SECURITY_POSIX_CAP_H__
#define __HMNETD_SECURITY_POSIX_CAP_H__

#include <libsec/posix_cap.h>
#include <libsec/server.h>

#ifdef CONFIG_POSIX_CAPS
extern struct sec_comp sec_comp_posix_cap_net;
#endif

#endif
